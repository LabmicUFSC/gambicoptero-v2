IMPLEMENTATION_DIR=../plasma_ml605

# This work is licensed under the EPOS Software License v1.0.
# A copy of this license is available at the EPOS system source tree root.
# A copy of this license is also available online at:
# http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
# Note that EPOS Software License applies to both source code and executables.

run:
	vsim < simulation_cmds.do

run_fakeram:
	make -f $(IMPLEMENTATION_DIR)/makefile coregen_generate
	vsim < simulation_cmds_fakeram.do
	
clean:
	rm -rf *.vcd *.lxt *.log transcript work *.wlf *~
