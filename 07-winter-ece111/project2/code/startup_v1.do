view wave
vlog *.v
vsim testbench_wep_encrypt_v1
add wave sim:/testbench_wep_encrypt_v1/wep_inst/*
radix unsigned
restart -f
run -all
