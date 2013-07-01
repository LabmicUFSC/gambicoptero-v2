# EPOS Compiler Wraper Makefile

include	../../makedefs

all:		install

install:	eposcc
		$(INSTALL) -m 775 eposcc $(BIN)
		sed -e 's/^EPOS=.*$$/EPOS=$(subst /,\/,$(EPOS))/' $(SED_I) $(BIN)/eposcc

clean:
