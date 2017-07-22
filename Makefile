.PHONY: clean mrproper all

AR = ar
CXX = g++
CXXFLAGS =-std=c++11  -Wall -Wextra -pedantic -g -I$(INC)
INC = include
SRC = src
OBJ = obj
LIB = lib
BIN = bin

# ************ Compilación de módulos ************
all: $(BIN)/conectaN

$(BIN)/conectaN : $(OBJ)/matriz.o $(OBJ)/tablero.o $(OBJ)/jugador.o  $(OBJ)/conectan.o
	$(CXX) -o $(BIN)/conectaN $(OBJ)/matriz.o $(OBJ)/tablero.o $(OBJ)/jugador.o $(OBJ)/conectan.o

$(OBJ)/matriz.o : $(SRC)/matriz.cpp $(INC)/matriz.h
	$(CXX) -c $(CXXFLAGS) $(SRC)/matriz.cpp -o $(OBJ)/matriz.o

$(OBJ)/tablero.o : $(INC)/tablero.h $(SRC)/tablero.cpp
	$(CXX) -c $(CXXFLAGS) -o $(OBJ)/tablero.o $(SRC)/tablero.cpp

$(OBJ)/jugador.o : $(INC)/jugador.h $(SRC)/jugador.cpp
	$(CXX) -c $(CXXFLAGS) -o $(OBJ)/jugador.o $(SRC)/jugador.cpp 

$(OBJ)/conectan.o : $(SRC)/conectaN.cpp
	$(CXX) -c $(CXXFLAGS) -o $(OBJ)/conectan.o $(SRC)/conectaN.cpp


# ************ Generación de documentación ******************
#documentacion:
#	doxygen doc/doxys/Doxyfile

# ************ Limpieza ************
clean :
	-rm $(BIN)/* $(OBJ)/* $(SRC)/*~ $(INC)/*~ ./*~

mrproper : clean
	-rm $(BIN)/*
