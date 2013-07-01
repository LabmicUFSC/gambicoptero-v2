# EPOS Application Makefile

# This work is licensed under the EPOS Software License v1.0.
# A copy of this license is available at the EPOS system source tree root.
# A copy of this license is also available online at:
# http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
# Note that EPOS Software License applies to both source code and executables.

include ../makedefs

APP_PATH := $(subst _app,,$(APPLICATION))
ISDIR := $(shell test -d $(APP_PATH) && echo 0)

ifeq ($(ISDIR), 0)
SRC := $(shell find ./$(APP_PATH)/src/ -mindepth 1 -name "*.cc" -type f)
OBJS := $(subst .cc,.do,$(SRC))
else
OBJS :=
endif

# This is used by canneal_app parsec benchmark application
ENABLE_THREADS :=

ifeq ($(APPLICATION),canneal_app)
ENABLE_THREADS := -D ENABLE_THREADS
endif

all:		install

.SUFFIXES: .do .cc

.cc.do :	$(SRC)
		$(CXX) $(CXXFLAGS) -o $@ $< $(ENABLE_THREADS)

$(APPLICATION):	$(APPLICATION).o $(LIB)/* $(OBJS)
		$(ALD) $(ALDFLAGS) -o $@ $(APPLICATION).o $(OBJS)

$(APPLICATION).o: $(APPLICATION).cc $(SRC)
		$(ACC) $(ACCFLAGS) -o $@ $< $(ENABLE_THREADS)

install: 	$(APPLICATION)
ifneq ($(GDB_DEBUG), 1)
		$(STRIP) -o $(APPLICATION).tmp $<
else
		cp $< $(APPLICATION).tmp
endif
		$(INSTALL) $(APPLICATION).tmp $(IMG)/$(APPLICATION)
		$(CLEAN) $(APPLICATION).tmp

clean:
		$(CLEAN) *.o $(APPLICATION) $(OBJS)
