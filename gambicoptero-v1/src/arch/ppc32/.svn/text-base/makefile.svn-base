# EPOS PPC32 Architecture Makefile

# This work is licensed under the EPOS Software License v1.0.
# A copy of this license is available at the EPOS system source tree root.
# A copy of this license is also available online at:
# http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
# Note that EPOS Software License applies to both source code and executables.

include ../../../makedefs

OBJS := $(subst .cc,.o,$(shell find *.cc | grep -v _init | grep -v _test))
CRTS := $(subst .S,.o,$(shell find *.S | grep crt))
CRTSI := $(subst .S,.s,$(shell find *.S | grep crt))
INITS := $(subst .cc,.o,$(shell find *.cc | grep _init))
TESTS := $(subst .cc,,$(shell find *.cc | grep _test))

all:		crts $(LIBARCH) $(LIBINIT)

crts:		$(CRTS)
		$(INSTALL) $^ $(LIB)

.INTERMEDIATE:	$(CRTSI)

$(LIBARCH):	$(LIBARCH)($(OBJS))	

$(LIBINIT):	$(LIBINIT)($(INITS))

cpu.o		: cpu.cc
		$(CXX) $(CXXFLAGS) -fomit-frame-pointer $<

test:		$(TESTS)

clean:
		$(CLEAN) *.o *.s *_test
