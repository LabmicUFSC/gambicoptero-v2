# EPOS Main Makefile

# This work is licensed under the EPOS Software License v1.0.
# A copy of this license is available at the EPOS system source tree root.
# A copy of this license is also available online at:
# http://epos.lisha.ufsc.br/EPOS+Software+License+v1.0
# Note that EPOS Software License applies to both source code and executables.

include makedefs

SUBDIRS := etc tools src app img

all:	$(SUBDIRS)

$(SUBDIRS):	FORCE
		(cd $@ && $(MAKE))

run:		FORCE
		(cd img && $(MAKE) run)

runtest:	test
		(cd img && $(MAKE) runtest)

test:		$(SUBDIRS)
		(cd src && $(MAKETEST))
		(cd img && $(MAKETEST))

clean:		FORCE
		(cd src && $(MAKECLEAN))
		find $(LIB) -maxdepth 1 -type f -exec $(CLEAN) {} \;

veryclean:
		make MAKE:="$(MAKECLEAN)" $(SUBDIRS)
		find $(LIB) -maxdepth 1 -type f -exec $(CLEAN) {} \;
		find $(BIN) -maxdepth 1 -type f -exec $(CLEAN) {} \;
		find $(APP) -maxdepth 1 -type f -perm +111 -exec $(CLEAN) {} \;
		find $(IMG) -name "*.img" -exec $(CLEAN) {} \;
		find $(IMG) -name "*.out" -exec $(CLEAN) {} \;
		find $(TOP) -name "*~" -exec $(CLEAN) {} \; 

dist:		veryclean
		find $(TOP) -name ".*project" -exec $(CLEAN) {} \;
		find $(TOP) -name CVS -type d -print | xargs $(CLEANDIR)
		find $(TOP) -name .svn -type d -print | xargs $(CLEANDIR)
		find $(TOP) -name "*.h" -print | xargs sed -i "1r $(TOP)/LICENSE" 
		find $(TOP) -name "*.cc" -print | xargs sed -i "1r $(TOP)/LICENSE" 
		sed -e 's/^\/\//#/' LICENSE > LICENSE.mk
		find $(TOP) -name "makedefs" -print | xargs sed -i "1r $(TOP)/LICENSE.mk" 
		find $(TOP) -name "makefile" -print | xargs sed -i "1r $(TOP)/LICENSE.mk"
		$(CLEAN) LICENSE.mk 
		sed -e 's/^\/\//#/' LICENSE > LICENSE.as
		find $(TOP) -name "*.S" -print | xargs sed -i "1r $(TOP)/LICENSE.as" 
		$(CLEAN) LICENSE.as 

install:
	/usr/local/arm/gcc/bin/arm-objcopy\
		-I elf32-little\
		-O binary\
		img/$(APPLICATION).img\
		img/$(APPLICATION).bin
	python bin/red-bsl.py\
		-t /dev/ttyUSB0\
		-f img/$(APPLICATION).bin	
FORCE:
