# EPOS Setup Makefile

# This work is licensed under the EPOS Software License v1.0.
# A copy of this license is available at the EPOS system source tree root.
# A copy of this license is also available online at:
# http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
# Note that EPOS Software License applies to both source code and executables.

include ../../makedefs

TARGET := $(if $(shell find $(MACH)*), install)

all:		$(TARGET)

$(MACH)_setup:	$(MACH)_setup.o
		$(LD) $(LDFLAGS) -L$(CCLIB) --omagic -Ttext=$(SETUP_ADDR) -o $@ $^ -l$(LINIT) -l$(LMACH) -l$(LARCH) -l$(LUTIL) -lgcc

install:	$(MACH)_setup
ifneq ($(GDB_DEBUG), 1)
		$(STRIP) -o $<.tmp $<
else
		cp $< $<.tmp
endif
		$(INSTALL) $<.tmp $(IMG)/$<
		$(CLEAN) $<.tmp

test:

clean:
		$(CLEAN) *.o *_setup
