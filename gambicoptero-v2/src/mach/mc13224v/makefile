# EPOS-- ATMega128 Machine Makefile

include ../../../makedefs

OBJS := $(subst .cc,.o,$(shell find *.cc | grep -v test | grep -v init))
INITS := $(subst .cc,.o,$(shell find *.cc | grep init))
TESTS := $(subst .cc,.o,$(shell find *.cc | grep test))

all:		$(LIBMACH) $(LIBINIT)

$(LIBMACH):	$(LIBMACH)($(OBJS))

$(LIBINIT):	$(LIBINIT)($(INITS))

test:		$(TESTS)

clean:
		$(CLEAN) *.o *_test
