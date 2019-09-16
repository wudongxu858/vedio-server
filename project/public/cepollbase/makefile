CC=g++ -g -lpthread -std=c++11
CXX=g++
CXXFLAGS= -std=c++11  -lpthread  -g 
ELF=main
SRC=$(shell find ./ -name '*.cpp')
OBJ=$(SRC:.cpp=.o)
$(ELF):$(OBJ)
$(OBJ):
clean:
	rm -rf $(OBJ) $(ELF)