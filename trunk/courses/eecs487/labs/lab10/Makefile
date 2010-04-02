HEADER_FILES = main.h matvec.h spline.h

OBJS = main.o matvec.o spline.o draw.o

CC = g++
CFLAGS = -g -Wall -m32
ifeq ($(shell uname), Darwin)
  LIBS = -framework OpenGL -framework GLUT -lm
else
  LIBS = -lGL -lGLU -lglut -lm
endif

splines: $(OBJS) $(HEADER_FILES)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	-rm -f -r *.o *~ *core* splines  

