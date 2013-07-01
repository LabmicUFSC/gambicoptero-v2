# EPOS Init Makefile

include ../../makedefs

OBJS := $(subst .cc,.o,$(shell find *.cc))

all:		install

$(MACH)_init_builtin: $(SRC)/system/$(MACH)_system $(OBJS) $(LIBINIT)
		$(LD) $(LDFLAGS) -L$(CCLIB) --omagic \
		--section-start .init=$(INIT_ADDR) \
		--entry=_init -o $(MACH)_init \
		$(LIB)/$(ARCH)_crti.o \
		$(LIB)/$(ARCH)_crtbegin.o \
		init_first.o init_system.o \
		$(LIB)/$(ARCH)_crtend.o \
		$(LIB)/$(ARCH)_crtn.o \
		-l$(LINIT) \
		-R $(SRC)/system/$(MACH)_system \
		-l$(LUTIL) -lgcc -lc -lgcc
		$(INSTALL) init_application.o $(MACH)_init_application.o

$(MACH)_init_library: $(OBJS)
		$(INSTALL) init_first.o $(MACH)_init_first.o
		$(INSTALL) init_system.o $(MACH)_init_system.o
		$(INSTALL) init_application.o $(MACH)_init_application.o


$(MACH)_install_builtin: $(MACH)_init_builtin
		$(STRIP) -o $(MACH)_init.tmp $(MACH)_init
		$(INSTALL) $(MACH)_init.tmp $(IMG)/$(MACH)_init
		$(CLEAN) $(MACH)_init.tmp
		$(INSTALL) $(MACH)_init_application.o $(LIB)

$(MACH)_install_library: $(MACH)_init_library
		$(INSTALL) $(MACH)_init_first.o $(LIB)
		$(INSTALL) $(MACH)_init_system.o $(LIB)
		$(INSTALL) $(MACH)_init_application.o $(LIB)


install:        $(MACH)_install_$(MODE)

test:

clean:
		$(CLEAN) *.o *_init

FORCE:
