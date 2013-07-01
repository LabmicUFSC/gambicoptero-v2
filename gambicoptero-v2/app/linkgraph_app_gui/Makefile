#============================================================================
# Name        : emote2-demo.cpp
# Author      : Arliones Hoeller Jr.
# Copyright   : Copyright by LISHA/UFSC.
#============================================================================

EXEC_NAME = emote2-demo

# Dependencies
#     Headers
BOOST_INC = -I/usr/include
LIBSERIAL_INC = -I/usr/include
UBIGRAPH_INC = -I/usr/include
#     Libs
BOOST_LIB = -L/usr/lib/
LIBSERIAL_LIB = -L/usr/lib
UBIGRAPH_LIB = -L/usr/lib
XMLRPC_LIB = 

LINK =  -lboost_thread -lserial -lubigraphclient -lxmlrpc -lxmlrpc_client -lxmlrpc_util -lxmlrpc_xmlparse -lxmlrpc_xmltok

GPP = g++
LD = g++

INCLUDE = $(BOOST_INC) $(LIBSERIAL_INC) $(UBIGRAPH_INC)
LIBS = $(BOOST_LIB) $(LIBSERIAL_LIB) $(UBIGRAPH_LIB) $(XMLRPC_LIB)

GPP_FLAGS = -O0 -g3 -Wall
LD_FLAGS = 

OBJS = emote2-demo.o CommandMonitor.o CommandParser.o Serial.o

all: $(EXEC_NAME)

$(EXEC_NAME): $(OBJS)
	$(LD) $(LD_FLAGS) -o $(EXEC_NAME) $(OBJS) $(LIBS) $(LINK)

%.o: %.cpp
	$(GPP) $(INCLUDE) $(GPP_FLAGS) -c -o $@ $<

clean:
	rm *.o $(EXEC_NAME)
