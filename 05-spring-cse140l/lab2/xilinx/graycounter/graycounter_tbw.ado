source {G:/xilinx/ise71/data/projnav/scripts/VsimStopServer.tcl}
initIPC 1200
-- Auto Generated by Project Navigator to simulate
-- HDL Bencher Annotated Test Bench
--
vlib work
onbreak {resume}
onerror {quit -code 99 -force}
onElabError {resume}
vcom -93 -explicit  graycounter.vhf
vcom -93 -explicit  graycounter_tbw.ant
vsim -t 1ps  -lib work graycounter_tbw
run -all
quit -force
