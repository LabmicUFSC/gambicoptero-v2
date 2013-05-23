# EPOS Boot Makefile

# This work is licensed under the EPOS Software License v1.0.
# A copy of this license is available at the EPOS system source tree root.
# A copy of this license is also available online at:
# http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
# Note that EPOS Software License applies to both source code and executables.

include ../../makedefs

TARGET := $(if $(shell find $(MACH)*), $(MACH)_install)
ELF_HEADER_OFFSET := sh -c "$(OBJDUMP) -p $(IMG)/$(MACH)_setup | sed -n -e 's/^ *LOAD off *\(0x.*\) vaddr.*$$/\1/ p' | head -1"

all: 		$(TARGET)

pc_boot:        pc_boot.o
		ld86 -0 -s -T $(BOOT_ADDR) -o $@ $<

pc_boot.o:      pc_boot.s
		sed -e 's/^ELF_HDR_SIZE =.*$$/ELF_HDR_SIZE = $(shell $(ELF_HEADER_OFFSET))/' -i $<
		as86 -0 -o $@ $<

pc_install:	pc_boot
		$(DD) if=$< of=$<.tmp ibs=32 skip=1 obs=512 >& /dev/null
		$(INSTALL) $(MACH)_boot.tmp $(IMG)/$(MACH)_boot
		$(CLEAN) $(MACH)_boot.tmp

ml310_boot:	ml310_boot.o
		$(LD) $(LDFLAGS) -L$(CCLIB) --omagic \
		--section-start $(MACH_CODE_NAME)=$(BOOT_ADDR) \
		--entry=_start \
		-o $@ $< -l$(LUTIL) -l$(LSYS) -l$(LINIT) -lgcc -lc -lgcc

ml310_install:	ml310_boot
		$(OBJCPY) -O binary $< $<.tmp
		$(INSTALL) $(MACH)_boot.tmp $(IMG)/$(MACH)_boot
		$(CLEAN) $(MACH)_boot.tmp

test:

clean:
		$(CLEAN) *.s *.o *_boot *.tmp

