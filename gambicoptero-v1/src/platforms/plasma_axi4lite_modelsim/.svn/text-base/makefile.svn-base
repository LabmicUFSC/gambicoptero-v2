IMPLEMENTATION_DIR=../plasma_axi4lite_ml605

# This work is licensed under the EPOS Software License v1.0.
# A copy of this license is available at the EPOS system source tree root.
# A copy of this license is also available online at:
# http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
# Note that EPOS Software License applies to both source code and executables.

run:
	make -f $(IMPLEMENTATION_DIR)/makefile coregen_generate
	cp ../../mach/common/coregen/ram_amba_128k.mif .
	vsim < simulation_cmds.do
	
run_dec:
	make -f $(IMPLEMENTATION_DIR)/makefile coregen_generate
	cp ../../mach/common/coregen/ram_amba_128k.mif .
	vsim < simulation_cmds_dec.do

clean:
	rm -rf *.vcd *.lxt *.log transcript work *.wlf *.mif *~
