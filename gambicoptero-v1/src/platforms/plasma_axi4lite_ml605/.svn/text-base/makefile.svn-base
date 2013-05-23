##################################################

# This work is licensed under the EPOS Software License v1.0.
# A copy of this license is available at the EPOS system source tree root.
# A copy of this license is also available online at:
# http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
# Note that EPOS Software License applies to both source code and executables.
# Project Setup
##################################################
TOP_MODULE = plasma_axi4lite_ml605
BUILD_DIR = build

##################################################
# Include other makefiles 
##################################################

# common definitions
include ../../../tools/xilinx/Makefile.common

# coregen sources definitions
include ../../../src/mach/common/coregen/hw.srcs

# other IP sources definitions
include ../../../src/mach/axi4lite/hw.srcs
include ../../../src/mach/common/amba/hw.srcs
include ../../../src/mach/common/amba_wishbone/hw.srcs
include ../../../src/mach/common/simple_uart/hw.srcs
include ../../../src/mach/common/gpio/hw.srcs
include ../../../src/mach/common/simple_pic/hw.srcs
include ../../../src/mach/common/simple_timer/hw.srcs

##################################################
# Sources
##################################################

# Top level sources
TOP_SRCS = \
plasma_axi4lite_ml605.vhd \
plasma_axi4lite_ml605.ucf


# Coregen cores
coregen_generate: \
clk_xlnx_100M_diff \
ram_amba_128k \
ram_amba_1024k
#axi4_ddr3_contrl

coregen_clean:
	cd $(COREGEN_DIR); \
	rm -rf \
	clk_xlnx_100M_diff \
	axi4_ddr3_contrl \
	ram_amba_128k ; \
	rm -f coregen.cgc; \
	rm -rf tmp _xmsgs; \
	rm -f *.xise *.gise *.ncf *.tcl *.txt *.ejp *.log *.ngc *~ *.v *.veo *.vhd *.vho *.mif



# IP sources
SOURCES = $(abspath $(TOP_SRCS)) \
$(AMBA_SRCS) \
$(AXI_WB_SRCS) \
$(SIMPLE_UART_SRCS) \
$(SIMPLE_PIC_SRCS) \
$(SIMPLE_TIMER_SRCS) \
$(COREGEN_SRCS) \
$(PLASMA_SRCS) \
$(GPIO_SRCS)

##################################################
# Project Properties
##################################################
PROJECT_PROPERTIES = \
family Virtex6 \
device xc6vlx240t \
package ff1156 \
speed -1 \
top_level_module_type "HDL" \
synthesis_tool "XST (VHDL/Verilog)" \
simulator "Modelsim-SE Mixed" \
"Preferred Language" "Verilog" \
"Enable Message Filtering" FALSE \
"Display Incremental Messages" FALSE 


##################################################
# Process Properties
##################################################
SYNTHESIZE_PROPERTIES = ""

TRANSLATE_PROPERTIES = ""

MAP_PROPERTIES = \
"Generate Detailed Map Report" TRUE \
"Enable Multi-Threading" 2

PLACE_ROUTE_PROPERTIES = \
"Enable Multi-Threading" 2

STATIC_TIMING_PROPERTIES = ""

GEN_PROG_FILE_PROPERTIES = ""

SIM_MODEL_PROPERTIES = ""
