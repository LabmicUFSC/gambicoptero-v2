# eposldr Makefile

include	../../makedefs

all		: install

eposldr		: eposldr.cc
		$(TCXX) $(TCXXFLAGS) $<
		$(TLD) $(TLDFLAGS) -o $@ eposldr.o

install		: eposldr
		$(INSTALL) $^ $(BIN)

clean		:
		$(CLEAN) *.o eposldr
