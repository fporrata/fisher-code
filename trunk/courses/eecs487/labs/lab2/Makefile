CC = gcc
CFLAGS = -g -Wall -m32
ifeq ($(shell uname), Darwin)
  LIBS = -framework OpenGL -framework GLUT -lm
else
  LIBS = -lGL -lGLU -lglut -lm
endif

drawtriangle: drawtriangle.o
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

soln: drawtriangle-soln.o
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

tgz: lab2.pdf Makefile drawtriangle.c
	tar czf lab2.tgz lab2.pdf Makefile drawtriangle.c

.PHONY: clean

clean:
	-rm -f *.o *~ drawtriangle *core*
