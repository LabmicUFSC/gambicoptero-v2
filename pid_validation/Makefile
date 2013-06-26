GXX = g++
OBJS = Kinematics.o main.o AeroQuad.o Pid.o
OUTPUT = PidValidation

all: $(OBJS)
	$(GXX) -o $(OUTPUT) $(OBJS)
	./$(OUTPUT)
	bash generateChart.sh

main.o: main.cpp
	$(GXX) -c main.cpp -o main.o  -std=c++11

Pid.o: Pid.cpp
	$(GXX) -c Pid.cpp -o Pid.o

AeroQuad.o: AeroQuad.cpp
	$(GXX) -c AeroQuad.cpp -o AeroQuad.o	

Kinematics.o: Kinematics.cpp
	$(GXX) -c Kinematics.cpp -o Kinematics.o	

clean:
	rm -rf $(OBJS) $(OUTPUT)
