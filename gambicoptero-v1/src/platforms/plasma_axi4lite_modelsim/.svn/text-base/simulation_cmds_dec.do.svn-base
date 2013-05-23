#clear stuff
dataset clear
profile clear
vdel -all

#create work library
vlib work

#compile vhdl/verilog files for mixed simulation
vcom ../../mach/plasma/mlite_pack.vhd
vcom ../../mach/plasma/mlite_cpu.vhd
vcom ../../mach/plasma/alu.vhd
vcom ../../mach/plasma/bus_mux.vhd
vcom ../../mach/plasma/control.vhd
vcom ../../mach/plasma/mem_ctrl.vhd
vcom ../../mach/plasma/mult.vhd
vcom ../../mach/plasma/pipeline.vhd
vcom ../../mach/plasma/pc_next.vhd
vcom ../../mach/plasma/reg_bank.vhd
vcom ../../mach/plasma/shifter.vhd
vcom ../../mach/plasma/cache.vhd
vcom ../../mach/plasma/plasma_axi4lite_master.vhd

vcom ../../mach/common/coregen/ram_amba_128k.vhd
vcom ../../mach/common/coregen/ram_amba_1024k.vhd

vlog ../../mach/common/amba_wishbone/axi4lite_to_wishbone.v

vlog ../../mach/common/simple_uart/shortfifo.v
vlog ../../mach/common/simple_uart/medfifo.v
vlog ../../mach/common/simple_uart/simple_uart_rx.v
vlog ../../mach/common/simple_uart/simple_uart_tx.v
vlog ../../mach/common/simple_uart/simple_uart.v
vlog ../../mach/common/simple_uart/simple_uart_axi4lite.v
vlog ../../mach/common/simple_uart/uart_rx.v

vlog ../../mach/common/gpio/very_simple_gpio.v
vlog ../../mach/common/gpio/gpio_axi4lite.v

vlog ../../mach/common/simple_pic/priority_enc.v
vlog ../../mach/common/simple_pic/pic.v
vlog ../../mach/common/simple_pic/pic_axi4lite.v

vlog ../../mach/common/simple_timer/timer2.v
vlog ../../mach/common/simple_timer/timer_axi4lite.v

vlog ../../mach/common/amba/amba_mux.v
vlog ../../mach/common/amba/address_decoder.v
vlog ../../mach/common/amba/priority_encoder.v
vlog ../../mach/common/amba/axi4lite_decoder.v

vcom ../../mach/plasma/plasma_axi4lite.vhd

vcom plasma_axi4lite_dec_testbench.vhd


#load/dump/run vhdl design
set StdArithNoWarnings 1
set NumericStdNoWarnings 1

vsim -L unisim -L axi_uartlite_v1_01_a  work.plasma_axi4lite_testbench

#vcd file trace_dec.vcd
#vcd add -r -file trace_dec.vcd /*
#vcd add -r -file trace_dec.vcd /plasma_axi4lite_testbench/plasma/timer/*
#vcd add -r -file trace_dec.vcd /plasma_axi4lite_testbench/plasma/pic/*
#vcd add -file trace_dec.vcd /plasma_axi4lite_testbench/plasma/plasma_amba/*

run -all
