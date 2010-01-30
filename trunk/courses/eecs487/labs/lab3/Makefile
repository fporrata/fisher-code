CC = gcc
CFLAGS = -g -Wall -m32
ifeq ($(shell uname), Darwin)
  LIBS = -framework OpenGL -framework GLUT -lm
else
  LIBS = -lGL -lGLU -lglut -lm
endif

transforms: transforms.o
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

soln: transforms-soln.o
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

tgz: lab3.pdf Makefile transforms.c
	tar czf lab3.tgz lab3.pdf Makefile transforms.c

.PHONY: clean

clean:
	-rm -f *.o *~ transforms soln lab3.tgz *core*
