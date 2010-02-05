CXX = g++
CFLAGS = -g -Wall -m32

ifeq ($(shell uname), Darwin)
  LIBS = -framework OpenGL -framework GLUT -lm
else
  LIBS = -lGL -lGLU -lglut -lm
endif

viewLights: viewLights.o
	$(CXX) $(CFLAGS) -o $@ viewLights.o $(LIBS)

soln: viewLights-soln.o
	$(CXX) $(CFLAGS) -o $@ viewLights-soln.o $(LIBS)

tgz: lab4.pdf Makefile viewLights.cpp xvec.h xmat.h fakeGL.h
	tar czf lab4.tgz lab4.pdf Makefile viewLights.cpp fakeGL.h xmat.h xvec.h

test: testCase.o
	$(CXX) $(CFLAGS) -o $@ viewLights.o $(LIBS)

%.o: %.cpp fakeGL.h
	$(CC) $(CFLAGS) -c $<

.PHONY: clean

clean:
	-rm -f *.o *~ viewLights soln lab4.tgz *core*
