# EPOS Image Makefile

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
