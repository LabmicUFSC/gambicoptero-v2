# EPOS Utilities Makefile

# This work is licensed under the EPOS Software License v1.0.
# A copy of this license is available at the EPOS system source tree root.
# A copy of this license is also available online at:
# http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
# Note that EPOS Software License applies to both source code and executables.

include ../../makedefs

OBJS := $(subst .cc,.o,$(shell find *.cc | grep -v test))
TESTS := $(subst .cc,,$(shell find *.cc | grep test))

all:		$(LIBUTIL)

$(LIBUTIL):	$(LIBUTIL)($(OBJS))	


test:		$(TESTS)

clean:
		$(CLEAN) *.o *_test
