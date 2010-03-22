# PA2 for EECS 487 Fall 2009

CC = g++

ifeq ($(shell uname), Darwin)
  INCLUDES = -I../../common/include 
  LIBS = -lexpat -framework OpenGL -framework GLUT
else
  INCLUDES = -I../../common/include 
  LIBS = -lglut -lexpat
endif

CPPFLAGS = -g $(INCLUDES)

OBJS = main.o shaders.o ltga.o ../../common/src/glew.o

%.o : %.c
	gcc -c $(CPPFLAGS) $< -o $@

all : pa2

clean : 
	rm -f pa2 *.o ../../common/src/glew.o

main.o: main.cpp shaders.h ltga.h

shaders.o: shaders.cpp shaders.h 

ltga.o: ltga.cpp ltga.h

glew.o: glew.c

pa2 : $(OBJS)
	$(CC) -o $@ $(CPPFLAGS) $(OBJS) $(LIBS)
