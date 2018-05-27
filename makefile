CC = g++
LD_FLAGS = -g -o 
FLAGS = --std=c++11
SRC_FILES = $(wildcard *.cpp)
OBJ_FILES = $(notdir $(SRC_FILES:.cpp=.o))
OUT = main 

default: $(OBJ_FILES)
# create main
	$(CC) $(SRC_FILES) $(LD_FLAGS) $(OUT) $(FLAGS) 
	@make cleanobj

clean:
	-rm $(OUT)

cleanobj:
	-rm *.o

run:
	@make
	bin/$(OUT) $(args)
