# Doxygen's generation makefile

all: 
	doxygen doxygen.conf

logs:
	./fetch_cvslog.sh

clean:
	rm -rf *~
	find ./ -type f -name *.log -exec rm {} \;
	rm -rf ../html/*
