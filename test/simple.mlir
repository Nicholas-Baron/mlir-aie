module {
  %00 = AIE.core(0, 0)
  %11 = AIE.core(1, 1)
  %01 = AIE.core(0, 1)
  AIE.flow(%00, "DMA" : 0, %11, "ME" : 1)
  AIE.packet_flow(0x10) {
    AIE.packet_source < %00, "ME" : 0>
	 AIE.packet_dest < %11, "ME" : 0>
	 AIE.packet_dest < %01, "DMA" : 1>
  }
}
