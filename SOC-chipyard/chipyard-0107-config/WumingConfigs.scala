package chipyard

import chisel3._

import freechips.rocketchip.config.{Config}

class Wuming1StageConfig extends Config(
  // Create a Wuming 1-stage core
  new wuming.common.WithNWumingCores(1, internalTile = wuming.common.Stage1Factory) ++
  new testchipip.WithSerialTLWidth(32) ++
  new testchipip.WithSerialPBusMem ++
  new freechips.rocketchip.subsystem.WithScratchpadsOnly ++    // use wuming tile-internal scratchpad
  new freechips.rocketchip.subsystem.WithNoMemPort ++          // use no external memory
  new freechips.rocketchip.subsystem.WithNBanks(0) ++
  new chipyard.config.AbstractConfig)

class Wuming5StageConfig extends Config(
  // Create a Wuming 5-stage core
  new wuming.common.WithNWumingCores(1, internalTile = wuming.common.Stage5Factory) ++
  new testchipip.WithSerialTLWidth(32) ++
  new testchipip.WithSerialPBusMem ++
  new freechips.rocketchip.subsystem.WithScratchpadsOnly ++    // use wuming tile-internal scratchpad
  new freechips.rocketchip.subsystem.WithNoMemPort ++          // use no external memory
  new freechips.rocketchip.subsystem.WithNBanks(0) ++
  new chipyard.config.AbstractConfig)
