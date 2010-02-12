CC = g++
CFLAGS = -O3 -Wall -I./GL
ifeq ($(shell uname), Darwin)
  LIBS = -framework OpenGL -framework GLUT -lm
else
  LIBS = -lGL -lGLU -lglut -lm
endif
SRCS = main.cpp shaders.cpp
OBJS = $(patsubst %.cpp, %.o, $(SRCS)) glew.o
SOLN = main-soln.cpp shaders-soln.cpp
SOBJ = $(patsubst %.cpp, %.o, $(SOLN)) glew.o

lab5: $(OBJS)
	$(CC) -o $@ $(CFLAGS) $(OBJS) $(LIBS)

soln: $(SOBJ)
	$(CC) -o $@ $(CFLAGS) $(SOBJ) $(LIBS)

.o:.c
	$(CC) -c $(CFLAGS) $<

%.o: %.cpp
	$(CC) -c $(CFLAGS) $<

tgz: Makefile glew.c main.cpp shaders.cpp shaders.h timer.h lab5.pdf
	tar czf lab5.tgz Makefile glew.c main.cpp shaders.cpp shaders.h timer.h lab5.pdf GL

.PHONY: clean

clean:
	-rm -f *.o *~ lab5.tgz lab5 soln *core*
