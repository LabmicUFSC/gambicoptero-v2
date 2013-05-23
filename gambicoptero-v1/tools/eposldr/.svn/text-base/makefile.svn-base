# eposldr Makefile

# This work is licensed under the EPOS Software License v1.0.
# A copy of this license is available at the EPOS system source tree root.
# A copy of this license is also available online at:
# http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
# Note that EPOS Software License applies to both source code and executables.

include	../../makedefs

all		: install

eposldr		: eposldr.cc
		$(TCXX) $(TCXXFLAGS) $<
		$(TLD) $(TLDFLAGS) -o $@ eposldr.o

install		: eposldr
		$(INSTALL) $^ $(BIN)

clean		:
		$(CLEAN) *.o eposldr
