CC=g++ -g -lpthread
ELF=server
SRC=$(shell find ./ -name '*.cpp')
OBJ=$(SRC:.cpp=.o)
$(ELF):$(OBJ)
$(OBJ):
clean:
	rm -rf $(OBJ) $(ELF)