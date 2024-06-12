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
import freechips.rocketchip.prci.ClockSinkParameters

// Example parameter class copied from Ariane, not included in documentation but for compile check only
// If you are here for documentation, DO NOT copy MyCoreParams and MyTileParams directly - always figure
// out what parameters you need before you write the parameter class
case class WumingCoreParams(
  bootFreqHz: BigInt = BigInt(1700000000),
  ports: Int = 2,
  instlen: Int = 32,
  xprlen: Int = 64,
  internalTile: WumingInternalTileFactory = Stage5Factory
) extends CoreParams {
  val useVM: Boolean = false
  val useHypervisor: Boolean = false
  val useUser: Boolean = false
  val useSupervisor: Boolean = false
  val useDebug: Boolean = true
  val useAtomics: Boolean = false
  val useAtomicsOnlyForIO: Boolean = false // copied from Rocket
  val useCompressed: Boolean = false
  override val useVector: Boolean = false
  val useSCIE: Boolean = false
  val useRVE: Boolean = false
  val mulDiv: Option[MulDivParams] = None
  val fpu: Option[FPUParams] = None
  val nLocalInterrupts: Int = 0
  val useNMI: Boolean = false
  val nPMPs: Int = 0 // TODO: Check
  val pmpGranularity: Int = 4 // copied from Rocket
  val nBreakpoints: Int = 0 // TODO: Check
  val useBPWatch: Boolean = false
  val mcontextWidth: Int = 0 // TODO: Check
  val scontextWidth: Int = 0 // TODO: Check
  val nPerfCounters: Int = 0
  val haveBasicCounters: Boolean = true
  val haveFSDirty: Boolean = false
  val misaWritable: Boolean = false
  val haveCFlush: Boolean = false
  val nL2TLBEntries: Int = 0 // copied from Rocket
  val nL2TLBWays: Int = 0 // copied from Rocket
  val mtvecInit: Option[BigInt] = Some(BigInt(0)) // copied from Rocket
  val mtvecWritable: Boolean = true // copied from Rocket
  val instBits: Int = if (useCompressed) 16 else 32
  val lrscCycles: Int = 80 // copied from Rocket
  val decodeWidth: Int = 1 // TODO: Check
  val fetchWidth: Int = 1 // TODO: Check
  val retireWidth: Int = 1
  val nPTECacheEntries: Int = 0
  val useBitManip: Boolean = false
  val useBitManipCrypto: Boolean = false
  val useCryptoNIST: Boolean = false
  val useCryptoSM: Boolean = false
  val traceHasWdata: Boolean = false
  val useConditionalZero: Boolean = false
}

// DOC include start: CanAttachTile
case class WumingTileAttachParams(
  tileParams: WumingTileParams,
  crossingParams: RocketCrossingParams
) extends CanAttachTile {
  type TileType = WumingTile
  val lookup = PriorityMuxHartIdFromSeq(Seq(tileParams))
}
// DOC include end: CanAttachTile

case class WumingTileParams(
  name: Option[String] = Some("wuming_tile"),
  tileId: Int = 0,
  trace: Boolean = false,
  val core: WumingCoreParams = WumingCoreParams(),
  val scratchpad: DCacheParams = DCacheParams()
) extends InstantiableTileParams[WumingTile]
{
  val beuAddr: Option[BigInt] = None
  val blockerCtrlAddr: Option[BigInt] = None
  val btb: Option[BTBParams] = None
  val boundaryBuffers: Boolean = false
  val dcache: Option[DCacheParams] = Some(scratchpad)
  val icache: Option[ICacheParams] = None
  val clockSinkParams: ClockSinkParameters = ClockSinkParameters()
  def instantiate(crossing: HierarchicalElementCrossingParamsLike, lookup: LookupByHartIdImpl)(implicit p: Parameters): WumingTile = {
    new WumingTile(this, crossing, lookup)
  }
  val baseName = name.getOrElse("wuming_tile")
  val uniqueName = s"${baseName}_$tileId"
}

class WumingTile(
  val wumingParams: WumingTileParams,
  crossing: ClockCrossingType,
  lookup: LookupByHartIdImpl,
  q: Parameters)
  extends BaseTile(wumingParams, crossing, lookup, q)
  with SinksExternalInterrupts
  with SourcesExternalNotifications
{
  // Private constructor ensures altered LazyModule.p is used implicitly
  def this(params: WumingTileParams, crossing: HierarchicalElementCrossingParamsLike, lookup: LookupByHartIdImpl)(implicit p: Parameters) =
    this(params, crossing.crossingType, lookup, p)

  // Require TileLink nodes
  val intOutwardNode = Some(IntIdentityNode())
  val masterNode = visibilityNode
  val slaveNode = TLIdentityNode()

  // Connect node to crossbar switches (bus)
  tlOtherMastersNode := tlMasterXbar.node
  masterNode :=* tlOtherMastersNode
  DisableMonitors { implicit p => tlSlaveXbar.node :*= slaveNode }

  // Slave port adapter
  val coreParams = {
    class C(implicit val p: Parameters) extends HasCoreParameters
    new C
  }
  val dtim_address = tileParams.dcache.flatMap { d => d.scratch.map { s =>
    AddressSet.misaligned(s, d.dataScratchpadBytes)
  }}
  val dtim_adapter = dtim_address.map { addr =>
    LazyModule(new ScratchpadSlavePort(addr, coreParams.coreDataBytes, false))
  }
  dtim_adapter.foreach(lm => connectTLSlave(lm.node, lm.node.portParams.head.beatBytes))

  val dtimProperty = dtim_adapter.map(d => Map(
    "ucb-bar,dtim" -> d.device.asProperty)).getOrElse(Nil)

  // Wuming master port adapter
  val imaster_adapter = if (wumingParams.core.ports == 2) Some(LazyModule(new WumingMasterAdapter()(p, wumingParams.core))) else None
  if (wumingParams.core.ports == 2) tlMasterXbar.node := imaster_adapter.get.node
  val dmaster_adapter = LazyModule(new WumingMasterAdapter()(p, wumingParams.core))
  tlMasterXbar.node := dmaster_adapter.node

  // Implementation class (See below)
  override lazy val module = new WumingTileModuleImp(this)

  // Required entry of CPU device in the device tree for interrupt purpose
  val cpuDevice: SimpleDevice = new SimpleDevice("cpu", Seq("ucb-bar,wuming", "riscv")) {
    override def parent = Some(ResourceAnchors.cpus)
    override def describe(resources: ResourceBindings): Description = {
      val Description(name, mapping) = super.describe(resources)
      Description(name, mapping ++
                        cpuProperties ++
                        nextLevelCacheProperty ++
                        tileProperties ++
                        dtimProperty)
    }
  }

  ResourceBinding {
    Resource(cpuDevice, "reg").bind(ResourceAddress(tileId))
  }

  override def makeMasterBoundaryBuffers(crossing: ClockCrossingType)(implicit p: Parameters) = {
    if (!wumingParams.boundaryBuffers) super.makeMasterBoundaryBuffers(crossing)
    else TLBuffer(BufferParams.none, BufferParams.flow, BufferParams.none, BufferParams.flow, BufferParams(1))
  }

  override def makeSlaveBoundaryBuffers(crossing: ClockCrossingType)(implicit p: Parameters) = {
    if (!wumingParams.boundaryBuffers) super.makeSlaveBoundaryBuffers(crossing)
    else TLBuffer(BufferParams.flow, BufferParams.none, BufferParams.none, BufferParams.none, BufferParams.none)
  }

}

class WumingTileModuleImp(outer: WumingTile) extends BaseTileModuleImp(outer){
  // annotate the parameters
  Annotated.params(this, outer.wumingParams)

  // Wuming core parameters
  implicit val conf = outer.wumingParams.core

  // Scratchpad checking
  require(outer.dtim_adapter.isDefined, "Wuming core must have a scratchpad: make sure that tileParams.dcache.scratch is defined.")
  require(outer.dtim_address.get.length == 1, "Wuming core can only have one scratchpad.")

  // Tile
  val tile = Module(outer.wumingParams.core.internalTile.instantiate(outer.dtim_address.get.apply(0)))

  // Add scratchpad adapter
  val scratchpadAdapter = Module(new WumingScratchpadAdapter()(outer.p, conf))
  scratchpadAdapter.io.slavePort <> outer.dtim_adapter.get.module.io.dmem

  // Connect tile
  tile.io.debug_port <> scratchpadAdapter.io.memPort
  tile.io.master_port(0) <> outer.dmaster_adapter.module.io.dport
  if (outer.wumingParams.core.ports == 2) tile.io.master_port(1) <> outer.imaster_adapter.get.module.io.dport

  // Connect interrupts
  outer.decodeCoreInterrupts(tile.io.interrupt)

  // Connect constants
  tile.io.hartid := outer.hartIdSinkNode.bundle
  tile.io.reset_vector := outer.resetVectorSinkNode.bundle
}

class WithNWumingCores(
  n: Int = 1,
  internalTile: WumingInternalTileFactory = Stage5Factory
) extends Config((site, here, up) => {
  case TilesLocated(InSubsystem) => {
    // Calculate the next available hart ID (since hart ID cannot be duplicated)
    val prev = up(TilesLocated(InSubsystem), site)
    require(prev.length == 0, "Wuming doesn't support multiple core.")
    val idOffset = up(NumTiles)
    // Create TileAttachParams for every core to be instantiated
    (0 until n).map { i =>
      WumingTileAttachParams(
        tileParams = WumingTileParams(
          tileId = i + idOffset,
          scratchpad = DCacheParams(
            nSets = 4096, // Very large so we have enough SPAD for bmark tests
            nWays = 1,
            nMSHRs = 0,
            scratch = Some(0x80000000L)
          ),
          core = WumingCoreParams(
            ports = internalTile.nMemPorts,
            internalTile = internalTile
          )
        ),
        crossingParams = RocketCrossingParams()
      )
    } ++ prev
  }
  // Configurate # of bytes in one memory / IO transaction. For RV64, one load/store instruction can transfer 8 bytes at most.
  case SystemBusKey => up(SystemBusKey, site).copy(beatBytes = 4)
  // The # of instruction bits. Use maximum # of bits if your core supports both 32 and 64 bits.
  case XLen => 32
  case NumTiles => up(NumTiles) + n
}) {
  require(n == 1, "Wuming doesn't support multiple core.")
}
