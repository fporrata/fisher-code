CC = g++
CFLAGS = -O3 -Wall
ifeq ($(shell uname), Darwin)
  LIBS = -framework OpenGL -framework GLUT -lm
else
  LIBS = -lGL -lGLU -lglut -lm
endif

SRCS = globe.cpp ltga.cpp
OBJS = $(patsubst %.cpp, %.o, $(SRCS))
SOLN = globe-soln.cpp ltga.cpp
SOBJ = $(patsubst %.cpp, %.o, $(SOLN))

lab6: $(OBJS)
	$(CC) -o $@ $(CFLAGS) $(OBJS) $(LIBS)

soln: $(SOBJ)
	$(CC) -o $@ $(CFLAGS) $(SOBJ) $(LIBS)

.o:.c
	$(CC) -c $(CFLAGS) $<

%.o: %.cpp
	$(CC) -c $(CFLAGS) $<

tgz: Makefile globe.cpp ltga.cpp ltga.h images lab6.pdf
	tar czf lab6.tgz Makefile globe.cpp ltga.cpp ltga.h images lab6.pdf

.PHONY: clean

clean:
	-rm -f *.o *~ lab6.tgz lab6 soln *core*
