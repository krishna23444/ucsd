source {G:/xilinx/ise71/data/projnav/scripts/VsimStopServer.tcl}
initIPC 1200
-- Auto Generated by Project Navigator to simulate
-- HDL Bencher Annotated Test Bench
--
vlib work
onbreak {resume}
onerror {quit -code 99 -force}
onElabError {resume}
vcom -93 -explicit  asyncounter.vhf
vcom -93 -explicit  asyncounter_tbw.ant
vsim -t 1ps  -lib work asyncounter_tbw
run -all
quit -force
