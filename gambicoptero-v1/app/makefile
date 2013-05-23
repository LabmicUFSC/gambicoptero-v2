# EPOS Application Makefile

# This work is licensed under the EPOS Software License v1.0.
# A copy of this license is available at the EPOS system source tree root.
# A copy of this license is also available online at:
# http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
# Note that EPOS Software License applies to both source code and executables.

include ../makedefs

all:		install


$(APPLICATION):	$(APPLICATION).o $(LIB)/*
		$(ALD) $(ALDFLAGS) -o $@ $<

$(APPLICATION).o: $(APPLICATION).cc 
		$(ACC) $(ACCFLAGS) -o $@ $<

install: 	$(APPLICATION)
ifneq ($(GDB_DEBUG), 1)
		$(STRIP) -o $(APPLICATION).tmp $<
else
		cp $< $(APPLICATION).tmp
endif
		$(INSTALL) $(APPLICATION).tmp $(IMG)/$(APPLICATION)
		$(CLEAN) $(APPLICATION).tmp

clean:
		$(CLEAN) *.o $(APPLICATION)
