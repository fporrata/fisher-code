CC = g++
CFLAGS = -g -Wall -m32
SRCS = main.cpp canvas.cpp colorpicker.cpp
OBJS = $(patsubst %.cpp, %.o, $(SRCS))
ifeq ($(shell uname), Darwin)
  LIBS = -framework OpenGL -framework GLUT -lm
else
  LIBS = -lGL -lGLU -lglut -lm
endif

pa1: $(OBJS) rasterizer.o
	$(CC) $(CFLAGS) -o $@ $(OBJS) rasterizer.o $(LIBS)

soln: $(OBJS) rasterizer-soln.o
	$(CC) $(CFLAGS) -o $@ $(OBJS) rasterizer-soln.o $(LIBS)

tgz: Makefile canvas.h colorpicker.h rasterizer.h targa.h xvec.h $(SRCS) rasterizer.cpp
	tar czf pa1.tgz Makefile canvas.h colorpicker.h rasterizer.h targa.h xvec.h $(SRCS) rasterizer.cpp

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

.PHONY: clean

clean:
	-rm -f *.o *~ *core* pa1 soln pa1.tgz
