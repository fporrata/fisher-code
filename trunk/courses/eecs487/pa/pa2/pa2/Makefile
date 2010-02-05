# PA2 for EECS 487 Fall 2009

CC = g++

ifeq ($(shell uname), Darwin)
  INCLUDES = -I../common/include 
  LIBS = -lexpat -framework OpenGL -framework GLUT
else
  INCLUDES = -I../common/include 
  LIBS = -lglut -lexpat
endif

CPPFLAGS = -g -m32 $(INCLUDES)

OBJS = main.o x3.o x3reader.o shaders.o ../common/src/glew.o

%.o : %.c
	gcc -c $(CPPFLAGS) $< -o $@

all : pa2

clean : 
	rm -f pa2 *.o ../common/src/glew.o

main.o: main.cpp x3reader.h x3.h shaders.h

x3.o: x3.cpp x3.h xvec.h

x3reader.o: x3reader.cpp x3reader.h x3.h xvec.h

shaders.o: shaders.cpp shaders.h 

glew.o: glew.c

pa2 : $(OBJS)
	$(CC) -o $@ $(CPPFLAGS) $(OBJS) $(LIBS)
