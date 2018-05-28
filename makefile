CC = g++
CC_FLAGS = -c -Wall -o
LD_FLAGS = -g -o 
FLAGS = --std=c++11
SRC_FILES = $(wildcard src/*.cpp)
TEST_FILES = $(wildcard test/*.cpp)
OBJ_FILES = $(addprefix obj/, $(notdir $(SRC_FILES:.cpp=.o)))
OUT = main 

default: $(OBJ_FILES)
# create main
	$(CC) $(SRC_FILES) $(LD_FLAGS) $(OUT) $(FLAGS) 
	@make cleanobj

obj/%.o : src/%.cpp	
	$(CC) $(CC_FLAGS) $@ $^ --std=c++11

clean:
	-rm obj/*.o	
	-rm $(OUT)

test: $(OBJ_FILES)
	$(CC) $(TEST_FILES) -Isrc/ -o test.out --std=c++11
	./test.out
	@rm test.out