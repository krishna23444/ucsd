add wave -ports -hex sim:/test_bench/reset_drive

add wave -divider {Altera_Stripe}
add wave -ports -hex sim:/test_bench/u_stripe/*

add wave -divider {Flash_Memory}
add wave -ports -hex sim:/test_bench/u_flash/*

add wave -divider {SRAM0}
add wave -ports -hex sim:/test_bench/u_stripe/u_spram0/*

add wave -divider {SRAM1}
add wave -ports -hex sim:/test_bench/u_stripe/u_spram1/*

add wave -divider {DPSRAM0}
add wave -ports -hex sim:/test_bench/u_stripe/u_dpram0/*

add wave -divider {DPSRAM1}
add wave -ports -hex sim:/test_bench/u_stripe/u_dpram1/*

add wave -divider {ARM_CORE}
add wave -ports -hex sim:/test_bench/u_stripe/u_ARM920Tr1/*

