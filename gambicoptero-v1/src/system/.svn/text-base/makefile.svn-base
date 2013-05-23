# EPOS System Makefile

# This work is licensed under the EPOS Software License v1.0.
# A copy of this license is available at the EPOS system source tree root.
# A copy of this license is also available online at:
# http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
# Note that EPOS Software License applies to both source code and executables.

include ../../makedefs

OBJS := $(subst .cc,.o,$(shell find *.cc))

all:		install

$(MACH)_system_builtin: $(OBJS) $(LIBSYS)
		$(LD) $(LDFLAGS) -L$(CCLIB) --nmagic \
		--section-start $(MACH_CODE_NAME)=$(SYS_CODE_ADDR) \
		--section-start $(MACH_DATA_NAME)=$(SYS_DATA_ADDR) \
		--entry=__do_global_ctors_aux -o $(MACH)_system \
		$(LIB)/$(ARCH)_crti.o \
		$(LIB)/$(ARCH)_crtbegin.o \
		system_scaffold.o \
		$(LIB)/$(ARCH)_crtend.o \
		$(LIB)/$(ARCH)_crtn.o \
		--whole-archive \
		-l$(LSYS) -l$(LMACH) -l$(LARCH) \
		--no-whole-archive \
		-l$(LUTIL) -lgcc -lc -lgcc
		$(INSTALL) application_scaffold.o $(MACH)_application.o

$(MACH)_system_library: $(OBJS) $(LIBSYS)
		$(INSTALL) system_scaffold.o $(MACH)_system.o
		$(INSTALL) application_scaffold.o $(MACH)_application.o


$(MACH)_install_builtin: $(MACH)_system_builtin
ifneq ($(GDB_DEBUG), 1)
		$(STRIP) -o $(MACH)_system.tmp $(MACH)_system
else
		cp $(MACH)_system $(MACH)_system.tmp
endif
		$(INSTALL) $(MACH)_system.tmp $(IMG)/$(MACH)_system
		$(CLEAN) $(MACH)_system.tmp
		$(INSTALL) $(MACH)_application.o $(LIB)

$(MACH)_install_library: $(MACH)_system_library
		$(INSTALL) $(MACH)_system.o $(LIB)
		$(INSTALL) $(MACH)_application.o $(LIB)


install:        $(MACH)_install_$(MODE)

test:

clean:
		$(CLEAN) *.o *_system

FORCE:
