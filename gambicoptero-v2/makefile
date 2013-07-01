# EPOS Main Makefile

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
		find $(TOP) -name "*.h" -print | xargs sed $(SED_I) "1r $(TOP)/LICENSE" 
		find $(TOP) -name "*.cc" -print | xargs sed $(SED_I) "1r $(TOP)/LICENSE" 
		sed -e 's/^\/\//#/' LICENSE > LICENSE.mk
		find $(TOP) -name "makedefs" -print | xargs sed $(SED_I) "1r $(TOP)/LICENSE.mk" 
		find $(TOP) -name "makefile" -print | xargs sed $(SED_I) "1r $(TOP)/LICENSE.mk"
		$(CLEAN) LICENSE.mk 
		sed -e 's/^\/\//#/' LICENSE > LICENSE.as
		find $(TOP) -name "*.S" -print | xargs sed $(SED_I) "1r $(TOP)/LICENSE.as" 
		$(CLEAN) LICENSE.as 

FORCE:
