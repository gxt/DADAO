package wuming.common

import chisel3._
import chisel3.util._

import org.chipsalliance.cde.config._
import freechips.rocketchip.subsystem._
import freechips.rocketchip.devices.tilelink._
import freechips.rocketchip.diplomacy._
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
    val interrupt = Input(new CoreInterrupts(false))
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

case object Stage5Factory extends WumingInternalTileFactory {
  case object Stage5CoreFactory extends WumingCoreFactory {
    val nMemPorts = 2
    def instantiate(implicit p: Parameters, conf: WumingCoreParams) = new wuming.stage5.Core()
  }
  def nMemPorts = Stage5CoreFactory.nMemPorts
  def instantiate(range: AddressSet)(implicit p: Parameters, conf: WumingCoreParams) = new WumingInternalTile(range, Stage5CoreFactory)
}
