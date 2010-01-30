CC = gcc
CFLAGS = -g -Wall -m32
SRCS = drawline.c
OBJS = $(SRCS:.c=.o)
ifeq ($(shell uname),Darwin)
  LIBS = -framework GLUT -framework OpenGL
else
  LIBS = -lglut -lGLU -lGL
endif

drawline: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

.o:.c
	$(CC) $(CFLAGS) -c $<

clean: FORCE
	-rm -f *.o *~ drawline *core*

FORCE:
