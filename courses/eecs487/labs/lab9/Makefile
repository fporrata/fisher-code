CXX = g++
CFLAGS = -g -Wall -m32
ifeq ($(shell uname), Darwin)
  LIBS = -framework OpenGL -framework GLUT -lm
else
  LIBS = -lGL -lGLU -lglut -lm
endif

shadows: shadows.cpp
	$(CXX) $(CFLAGS) -o $@ shadows.cpp $(LIBS)

all: shadows

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $<

.PHONY: clean
clean:
	-rm -f -r *.o *~ *core* shadows 

