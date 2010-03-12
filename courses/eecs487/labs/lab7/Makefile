CC = g++
CFLAGS = -g -Wall -m32
ifeq ($(shell uname), Darwin)
  LIBS = -framework OpenGL -framework GLUT -lm
else
  LIBS = -lGL -lGLU -lglut -lm
endif

SRCS = color.cpp light.cpp main.cpp point.cpp
STUDENT_SRCS = raytrace.cpp sphere.cpp plane.cpp
SOLN_SRCS = raytrace-soln.cpp sphere-soln.cpp plane-soln.cpp

OBJS = $(patsubst %.cpp, %.o, $(SRCS))
STUDENT_OBJS = $(patsubst %.cpp, %.o, $(STUDENT_SRCS))
SOLN_OBJS = $(patsubst %.cpp, %.o, $(SOLN_SRCS))

HEADERS = $(wildcard *.h)

raytrace: $(OBJS) $(STUDENT_OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(STUDENT_OBJS) $(LIBS)

soln: $(OBJS) $(SOLN_OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(SOLN_OBJS) $(LIBS)

tgz: Makefile $(SRCS) $(STUDENT_SRCS) $(HEADERS) lab7.pdf
	tar -czf lab7.tgz Makefile $(SRCS) $(STUDENT_SRCS) $(HEADERS) lab7.pdf

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

.PHONY: clean

clean:
	rm -f *.o *~ *core* raytrace soln lab7.tgz