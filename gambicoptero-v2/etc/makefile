# EPOS Configuration Makefile

include	../makedefs

all: eposcc.conf eposmkbi.conf $(INCLUDE)/system/config.h
		sed -e 's/^#define ARCH.*$$/#define ARCH $(ARCH)/' $(SED_I) $(INCLUDE)/system/config.h
		sed -e 's/^#define MACH.*$$/#define MACH $(MACH)/' $(SED_I) $(INCLUDE)/system/config.h

eposcc.conf:
		@echo "ARCH=$(ARCH)" > $@
		@echo "MACH=$(MACH)" >> $@
		@echo "TOOLS_PREFIX=$(COMP_PREFIX)" >> $@
		@echo "APP_CODE=$(APP_CODE_ADDR)" >> $@
		@echo "APP_DATA=$(APP_DATA_ADDR)" >> $@
		@echo "MACH_CC_FLAGS='$(MACH_CC_FLAGS)'" >> $@
		@echo "MACH_LD_FLAGS='$(MACH_LD_FLAGS)'" >> $@
		@echo "MACH_CODE_NAME='$(MACH_CODE_NAME)'" >> $@
		@echo "MACH_DATA_NAME='$(MACH_DATA_NAME)'" >> $@

eposmkbi.conf:	
		@echo "MODE=$(MODE)" > $@
		@echo "MACH=$(MACH)" >> $@
		@echo "ARCH=$(ARCH)" >> $@
		@echo "CLOCK=$(ARCH_CLOCK)" >> $@
		@echo "WORD_SIZE=$(ARCH_WORD_SIZE)" >> $@
		@echo "ENDIANESS=$(ARCH_ENDIANESS)" >> $@
		@echo "MEM_BASE=$(MEM_BASE)" >> $@
		@echo "MEM_TOP=$(MEM_TOP)" >> $@
		@echo "BOOT_LENGTH_MIN=$(BOOT_LENGTH_MIN)" >> $@
		@echo "BOOT_LENGTH_MAX=$(BOOT_LENGTH_MAX)" >> $@

clean:
		$(CLEAN) *.conf

		
