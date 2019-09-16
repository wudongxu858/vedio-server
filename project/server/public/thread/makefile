CC=g++  
CFLAGS=-lpthread -std=c++11
CXX=g++   -lpthread  -std=c++11
ELF=main

SRC=$(shell find ./ -name '*.cpp')
OBJ=$(SRC:.cpp=.o)
$(ELF):$(OBJ)
	$(CC) -o $(ELF) $(OBJ) $(CFLAGS)
$(OBJ):
clean:
	rm -rf $(OBJ) $(ELF)