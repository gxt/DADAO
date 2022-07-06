package wuming.common

import chisel3._
import chisel3.util._

import freechips.rocketchip.config._
import freechips.rocketchip.subsystem._
import freechips.rocketchip.devices.tilelink._
import freechips.rocketchip.diplomacy._
import freechips.rocketchip.diplomaticobjectmodel.logicaltree.{LogicalTreeNode}
import freechips.rocketchip.rocket._
import freechips.rocketchip.subsystem.{RocketCrossingParams}
import freechips.rocketchip.tilelink._
import freechips.rocketchip.interrupts._
import freechips.rocketchip.util._
import freechips.rocketchip.tile._
import freechips.rocketchip.amba.axi4._

// Event sets for core CSR
// This object simply disable all performance counters
object CSREvents {
  val events = new EventSets(Seq(new EventSet((mask, hit) => false.B, Seq(("placeholder", () => false.B)))))
}

// Abstract core and tile base class for all cores
abstract class AbstractCore extends Module {
  val mem_ports: Seq[MemPortIo]
  val interrupt: CoreInterrupts
  val hartid: UInt
  val reset_vector: UInt
  val io: Data
}
abstract class AbstractInternalTile(ports: Int)(implicit val p: Parameters, val conf: WumingCoreParams) extends Module {
  val io = IO(new Bundle {
    val debug_port = Flipped(new MemPortIo(data_width = conf.xprlen))
    val master_port = Vec(ports, new MemPortIo(data_width = conf.xprlen))
    val interrupt = Input(new CoreInterrupts())
    val hartid = Input(UInt())
    val reset_vector = Input(UInt())
  })
}

// Cores and internal tiles constructors
trait WumingCoreFactory {
  def nMemPorts: Int
  def instantiate(implicit p: Parameters, conf: WumingCoreParams): AbstractCore
}

trait WumingInternalTileFactory {
  def nMemPorts: Int
  def instantiate(range: AddressSet)(implicit p: Parameters, conf: WumingCoreParams): AbstractInternalTile
}

// Original wuming tile in this repo.
// This tile is only for 3-stage core since it has a special structure (SyncMem and possibly one memory port).
class WumingInternalTileStage3(range: AddressSet, ports: Int)(implicit p: Parameters, conf: WumingCoreParams)
  extends AbstractInternalTile(ports)
{
  // Core memory port
  val core   = Module(new wuming.stage3.Core())
  core.io := DontCare
  val core_ports = Wire(Vec(2, new MemPortIo(data_width = conf.xprlen)))
  core.io.imem <> core_ports(1)
  core.io.dmem <> core_ports(0)

  // scratchpad memory port
  val memory = Module(new SyncScratchPadMemory(num_core_ports = ports))
  val mem_ports = Wire(Vec(2, new MemPortIo(data_width = conf.xprlen)))
  // master memory port
  val master_ports = Wire(Vec(2, new MemPortIo(data_width = conf.xprlen)))

  // Connect ports
  ((mem_ports zip core_ports) zip master_ports).foreach({ case ((mem_port, core_port), master_port) => {
    val router = Module(new WumingRequestRouter(range))
    router.io.corePort <> core_port
    router.io.scratchPort <> mem_port
    router.io.masterPort <> master_port
    // For sync memory, use the request address from the previous cycle
    val reg_resp_address = Reg(UInt(conf.xprlen.W))
    when (core_port.req.fire) { reg_resp_address := core_port.req.bits.addr }
    router.io.respAddress := reg_resp_address
  }})

  // If we only use one port, arbitrate
  if (ports == 1)
  {
    // Arbitrate scratchpad
    val scratchpad_arbiter = Module(new wuming.stage3.WumingMemArbiter)
    mem_ports(1) <> scratchpad_arbiter.io.imem
    mem_ports(0) <> scratchpad_arbiter.io.dmem
    scratchpad_arbiter.io.mem <> memory.io.core_ports(0)

    // Arbitrate master
    val master_buffer = Module(new SameCycleRequestBuffer)
    val master_arbiter = Module(new wuming.stage3.WumingMemArbiter)
    master_ports(1) <> master_arbiter.io.imem
    master_ports(0) <> master_arbiter.io.dmem
    master_arbiter.io.mem <> master_buffer.io.in
    master_buffer.io.out <> io.master_port(0)
  }
  else
  {
    mem_ports(1) <> memory.io.core_ports(1)
    mem_ports(0) <> memory.io.core_ports(0)
    master_ports(1) <> io.master_port(1)
    master_ports(0) <> io.master_port(0)
  }

  memory.io.debug_port <> io.debug_port

  core.interrupt <> io.interrupt
  core.hartid := io.hartid
  core.reset_vector := io.reset_vector
}

// The general Wuming tile for all cores other than 3-stage
class WumingInternalTile(range: AddressSet, coreCtor: WumingCoreFactory)(implicit p: Parameters, conf: WumingCoreParams)
  extends AbstractInternalTile(coreCtor.nMemPorts)
{
  val core   = Module(coreCtor.instantiate)
  core.io := DontCare
  val memory = Module(new AsyncScratchPadMemory(num_core_ports = coreCtor.nMemPorts))

  val nMemPorts = coreCtor.nMemPorts
  ((memory.io.core_ports zip core.mem_ports) zip io.master_port).foreach({ case ((mem_port, core_port), master_port) => {
    val router = Module(new WumingRequestRouter(range))
    router.io.corePort <> core_port
    router.io.scratchPort <> mem_port
    router.io.masterPort <> master_port
    // For async memory, simply use the current request address
    router.io.respAddress := core_port.req.bits.addr
  }})

  io.debug_port <> memory.io.debug_port

  core.interrupt <> io.interrupt
  core.hartid := io.hartid
  core.reset_vector := io.reset_vector
}

// Tile constructor
case object Stage1Factory extends WumingInternalTileFactory {
  case object Stage1CoreFactory extends WumingCoreFactory {
    val nMemPorts = 2
    def instantiate(implicit p: Parameters, conf: WumingCoreParams) = new wuming.stage1.Core()
  }
  def nMemPorts = Stage1CoreFactory.nMemPorts
  def instantiate(range: AddressSet)(implicit p: Parameters, conf: WumingCoreParams) = new WumingInternalTile(range, Stage1CoreFactory)
}

case object Stage2Factory extends WumingInternalTileFactory {
  case object Stage2CoreFactory extends WumingCoreFactory {
    val nMemPorts = 2
    def instantiate(implicit p: Parameters, conf: WumingCoreParams) = new wuming.stage2.Core()
  }
  def nMemPorts = Stage2CoreFactory.nMemPorts
  def instantiate(range: AddressSet)(implicit p: Parameters, conf: WumingCoreParams) = new WumingInternalTile(range, Stage2CoreFactory)
}

case class Stage3Factory(ports: Int = 2) extends WumingInternalTileFactory {
  def nMemPorts = ports
  def instantiate(range: AddressSet)(implicit p: Parameters, conf: WumingCoreParams) = new WumingInternalTileStage3(range, ports)
}

case object Stage5Factory extends WumingInternalTileFactory {
  case object Stage5CoreFactory extends WumingCoreFactory {
    val nMemPorts = 2
    def instantiate(implicit p: Parameters, conf: WumingCoreParams) = new wuming.stage5.Core()
  }
  def nMemPorts = Stage5CoreFactory.nMemPorts
  def instantiate(range: AddressSet)(implicit p: Parameters, conf: WumingCoreParams) = new WumingInternalTile(range, Stage5CoreFactory)
}

case object UCodeFactory extends WumingInternalTileFactory {
  case object UCodeCoreFactory extends WumingCoreFactory {
    val nMemPorts = 1
    def instantiate(implicit p: Parameters, conf: WumingCoreParams) = new wuming.ucode.Core()
  }
  def nMemPorts = UCodeCoreFactory.nMemPorts
  def instantiate(range: AddressSet)(implicit p: Parameters, conf: WumingCoreParams) = new WumingInternalTile(range, UCodeCoreFactory)
}
