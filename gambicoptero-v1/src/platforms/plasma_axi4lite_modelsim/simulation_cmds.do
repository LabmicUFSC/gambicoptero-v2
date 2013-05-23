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

vcom plasma_axi4lite_testbench.vhd


#load/dump/run vhdl design
vsim -L unisim work.plasma_axi4lite_testbench
vcd file trace.vcd

vcd add -r -file trace.vcd /*

run -all
