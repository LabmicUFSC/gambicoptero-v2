# EPOS Image Makefile

# This work is licensed under the EPOS Software License v1.0.
# A copy of this license is available at the EPOS system source tree root.
# A copy of this license is also available online at:
# http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
# Note that EPOS Software License applies to both source code and executables.

include	../makedefs

IMAGE := $(addsuffix .img,$(APPLICATION))
OUTPUT := $(addsuffix .out,$(APPLICATION))

TESTS := $(shell find *_test 2> /dev/null)
TEST_IMAGES := $(addsuffix .img,$(TESTS))
TEST_OUTPUTS := $(addsuffix .out,$(TESTS))

all:		$(IMAGE)

test:		$(TEST_IMAGES)

run:		$(OUTPUT)

runtest:	$(TEST_OUTPUTS)

clean:
		$(CLEAN) *_boot *_setup *_init *_system $(APPLICATION) $(TESTS)
