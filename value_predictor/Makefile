GXX = g++
OBJS = main.o Predictor.o
OUTPUT = Predictor 

all: $(OBJS)
	$(GXX) -o $(OUTPUT) $(OBJS)
	./$(OUTPUT)
	bash generateChart.sh

Predictor.o: Predictor.cpp
	$(GXX) -c Predictor.cpp -o Predictor.o	

clean:
	rm -rf $(OBJS) $(OUTPUT)
