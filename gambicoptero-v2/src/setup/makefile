# EPOS Setup Makefile

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
