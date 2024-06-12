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

class WumingMasterAdapter(implicit p: Parameters, val conf: WumingCoreParams) extends LazyModule {
  // The node exposed to the crossbar
  val node = TLIdentityNode()

  // The client node (only one inflight request supported for Wuming)
  // This node handles core requests to addresses not managed in the tile-local scratchpad
  val masterNode = TLClientNode(Seq(TLMasterPortParameters.v1(
    clients = Seq(TLMasterParameters.v1(
      name = "wuming-mmio-master",
      sourceId = IdRange(0, 1)
    ))
  )))

  // Connect nodes
  (node := TLBuffer() := masterNode)

  lazy val module = new WumingMasterAdapterImp(this)
}

class WumingMasterAdapterImp(outer: WumingMasterAdapter) extends LazyModuleImp(outer) {
  implicit val conf = outer.conf

  val io = IO(new Bundle() {
    val dport = Flipped(new MemPortIo(data_width = conf.xprlen))
  })

  val (tl_out, edge) = outer.masterNode.out(0)

  // Register
  // State
  val s_ready :: s_active :: s_inflight :: Nil = Enum(3)
  val state = RegInit(s_ready)
  // Address and signedness of the request to be used by LoadGen
  val a_address_reg = Reg(UInt(io.dport.req.bits.addr.getWidth.W))
  val a_signed_reg = Reg(Bool())
  // Request register - store the request when the input port fires to avoid value changes when sending the TileLink request
  val req_address_reg = Reg(UInt(io.dport.req.bits.addr.getWidth.W))
  val req_size_reg = Reg(UInt(2.W))
  val req_data_reg = Reg(UInt(io.dport.req.bits.data.getWidth.W))
  // Sign and size
  val a_signed = io.dport.req.bits.getTLSigned
  val a_size = io.dport.req.bits.getTLSize

  // State logic
  when (state === s_ready && io.dport.req.valid) {
    state := s_active
    req_address_reg := io.dport.req.bits.addr
    req_size_reg := a_size
    req_data_reg := io.dport.req.bits.data
  }
  when (state === s_active && tl_out.a.fire) {
    state := s_inflight
  }
  when (state === s_inflight && tl_out.d.fire) {
    state := s_ready
  }
  tl_out.a.valid := state === s_active
  tl_out.d.ready := true.B
  io.dport.req.ready := state === s_ready
  io.dport.resp.valid := tl_out.d.valid

  // Bookkeeping
  when (tl_out.a.fire) {
    a_address_reg := io.dport.req.bits.addr
    a_signed_reg := a_size
  }

  // Build "Get" message
  val (legal_get, get_bundle) = edge.Get(0.U, req_address_reg, req_size_reg)
  // Build "Put" message
  val (legal_put, put_bundle) = edge.Put(0.U, req_address_reg, req_size_reg, req_data_reg)

  // Connect Channel A bundle
  tl_out.a.bits := Mux(io.dport.req.bits.fcn === M_XRD, get_bundle, put_bundle)

  // Connect Channel D bundle (read result)
  io.dport.resp.bits.data := new LoadGen(tl_out.d.bits.size, a_signed_reg, a_address_reg, tl_out.d.bits.data, false.B, conf.instlen / 8).data

  // Handle error
  val legal_op = Mux(io.dport.req.bits.fcn === M_XRD, legal_get, legal_put)
  val resp_xp = tl_out.d.bits.corrupt | tl_out.d.bits.denied
  // Since the core doesn't have an external exception port, we have to kill it
  assert(legal_op | !tl_out.a.valid, "Illegal operation legal_op: %d, tl_out.a.valid: %d\n", legal_op, tl_out.a.valid)
  assert(!resp_xp | !tl_out.d.valid, "Responds exception")

  // Tie off unused channels
  tl_out.b.valid := false.B
  tl_out.c.ready := true.B
  tl_out.e.ready := true.B
}

// This class allows the next memory request to be sent when the response of the previous request come back.
class SameCycleRequestBuffer(implicit val conf: WumingCoreParams) extends Module {
  val io = IO(new Bundle() {
    val in = Flipped(new MemPortIo(data_width = conf.xprlen))
    val out = new MemPortIo(data_width = conf.xprlen)
  })

  io.out.req <> io.in.req
  io.in.resp := Pipe(io.out.resp)
}
