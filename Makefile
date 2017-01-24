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
exec:	ex1_triangle.out ex2_triangle.out ex3_ibo.out

ex1_triangle.out: ex1_triangle.o shaderUtil.o window.o libGLEW.a libglfw.so.3 libglfw.so
	$(CC) $(CFLAGS) $(OBJ)/shaderUtil.o $(OBJ)/window.o $(OBJ)/ex1_triangle.o -o ex1_triangle.out -L$(LIB)/  -lGL -lglfw -lGLEW -Wl,-R,$(ROOT)/ 
	
ex2_triangle.out: ex2_triangle.o shaderUtil.o window.o libGLEW.a libglfw.so.3 libglfw.so
	$(CC) $(CFLAGS) $(OBJ)/shaderUtil.o $(OBJ)/window.o $(OBJ)/ex2_triangle.o -o ex2_triangle.out -L$(LIB)/  -lGL -lglfw -lGLEW -Wl,-R,$(ROOT)/
	
ex3_ibo.out: ex3_ibo.o shaderUtil.o window.o libGLEW.a libglfw.so.3 libglfw.so
	$(CC) $(CFLAGS) $(OBJ)/shaderUtil.o $(OBJ)/window.o $(OBJ)/ex3_ibo.o -o ex3_ibo.out -L$(LIB)/  -lGL -lglfw -lGLEW -Wl,-R,$(ROOT)/ 	

ex1_triangle.o: ex1_triangle.cpp
	$(CC) $(CFLAGS) $(INCLUDEC) -O -c $(SRC)/ex1_triangle.cpp -o $(OBJ)/ex1_triangle.o
	
ex2_triangle.o: ex2_triangle.cpp
	$(CC) $(CFLAGS) $(INCLUDEC) -O -c $(SRC)/ex2_triangle.cpp -o $(OBJ)/ex2_triangle.o
	
ex3_ibo.o: ex3_ibo.cpp
	$(CC) $(CFLAGS) $(INCLUDEC) -O -c $(SRC)/ex3_ibo.cpp -o $(OBJ)/ex3_ibo.o	

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
