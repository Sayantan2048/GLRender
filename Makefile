CC = g++
ASAN = -g3 -O0 -fno-omit-frame-pointer -fsanitize=address
CFLAGS = -Wall -fopenmp $(ASAN)
ROOT=$(realpath $(dir $(lastword $(MAKEFILE_LIST))))

INCLUDES = $(ROOT)/include
INCLUDEC = -I$(INCLUDES)
SRC = $(ROOT)/src
OBJ = $(ROOT)/src
LIB = $(ROOT)/libs

#make will look for files in these path apart from current location of makefile
VPATH = $(SRC) $(INCLUDES) $(OBJ) $(LIB)

#-Wl,-R,$(ROOT)/ is to suggest linker to look into $(ROOT) at runtime for loading dynamic libraries.
main: init.o shaderUtil.o window.o libGLEW.a libglfw.so.3 libglfw.so
	$(CC) $(CFLAGS) $(OBJ)/shaderUtil.o $(OBJ)/window.o $(OBJ)/init.o -L$(LIB)/  -lGL -lglfw -lGLEW -Wl,-R,$(ROOT)/ 

init.o: init.cpp
	$(CC) $(CFLAGS) $(INCLUDEC) -O -c $(SRC)/init.cpp -o $(OBJ)/init.o

shaderUtil.o: shaderUtil.cpp shaderUtil.h
	$(CC) $(CFLAGS) $(INCLUDEC) -O -c $(SRC)/shaderUtil.cpp -o $(OBJ)/shaderUtil.o
	
window.o: window.cpp window.h
	$(CC) $(CFLAGS) $(INCLUDEC) -O -c $(SRC)/window.cpp -o $(OBJ)/window.o	

libglfw.so.3: libglfw.so.3.2
	ln -s $(LIB)/libglfw.so.3.2 $(ROOT)/libglfw.so.3

libglfw.so: libglfw.so.3.2	
	ln -s $(LIB)/libglfw.so.3.2 $(LIB)/libglfw.so

.PHONY: clean cleanall

clean:
	cd $(OBJ) && rm *.o
	cd $(ROOT) && rm *.out

cleanall:
	-cd $(OBJ) && rm *.o
	-cd $(LIB)/ && rm *.so
	-cd $(ROOT) && rm *.so.*
	cd $(ROOT) && rm *.out
