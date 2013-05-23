#!/bin/bash

cd $EPOS

for FOO in `find ./ -type f -name *.h`; do
  echo "Fetching CVS log for file: " $FOO;
  LOG_FILE=./doc/devel/doxygen/`dirname $FOO`/`basename $FOO .h`_h.log;
  cvs log $FOO > $LOG_FILE; 
done

for FOO in `find ./ -type f -name *.cc`; do
  echo "Fetching CVS log for file: " $FOO;
  LOG_FILE=./doc/devel/doxygen/`dirname $FOO`/`basename $FOO .cc`_cc.log;
  cvs log $FOO > $LOG_FILE;
done
