HEADER_FILES = xvec.h xmat.h interpolator.h

OBJS =  animation.o

CC = g++
CFLAGS = -g -Wall -m32
ifeq ($(shell uname), Darwin)
  LIBS = -framework OpenGL -framework GLUT -lm
else
  LIBS = -lGL -lGLU -lglut -lm
endif

animation: $(OBJS) $(HEADER_FILES)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $?

.PHONY: clean
clean:
	-rm -f -r *.o *~ *core* animation  

