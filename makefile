DIR_INC = ./include
DIR_SRC = ./src
DIR_OBJ = ./build
DIR_BIN = ./bin

CC = g++
TARGET = ${DIR_BIN}/verification_rcgn
#TARGET = verification_rcgn
SRC = $(wildcard ${DIR_SRC}/*.cpp)
OBJ = $(patsubst ${DIR_SRC}/%.cpp,${DIR_OBJ}/%.o,$(SRC))
LIB = `pkg-config --cflags --libs opencv`

$(TARGET) : $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LIB)

$(DIR_OBJ)/%.o : $(DIR_SRC)/%.cpp
	$(CC) -c $< -o $@
	@echo "Compling " $@ "..."

.PHONY : clean
clean:
	rm -rf ${DIR_OBJ}/*.o $(TARGET) ${DIR_BIN}/*
