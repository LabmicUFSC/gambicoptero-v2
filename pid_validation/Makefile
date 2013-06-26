GXX = g++
OBJS = main.o AeroQuad.o Pid.o
OUTPUT = PidValidation

all: $(OBJS)
	$(GXX) -o $(OUTPUT) $(OBJS)
	./$(OUTPUT)

main.o: main.cpp
	$(GXX) -c main.cpp -o main.o 

Pid.o: Pid.cpp
	$(GXX) -c Pid.cpp -o Pid.o

AeroQuad.o: AeroQuad.cpp
	$(GXX) -c AeroQuad.cpp -o AeroQuad.o	

clean:
	rm -rf $(OBJS) $(OUTPUT)