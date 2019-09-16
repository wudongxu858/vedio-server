CC=g++  
CFLAGS=-lpthread -lsqlite3 -std=c++11
CXX=g++   -lpthread -lsqlite3 -std=c++11
ELF=main

SRC=$(shell find ./ -name '*.cpp')
OBJ=$(SRC:.cpp=.o)
$(ELF):$(OBJ)
	$(CC) -o $(ELF) $(OBJ) $(CFLAGS)
$(OBJ):
clean:
	rm -rf $(OBJ) $(ELF)