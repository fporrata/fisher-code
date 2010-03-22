CC = g++

CPPFLAGS = -O3 $(INCLUDES) -Wall -L/opt/local/lib -I/opt/local/include -g

LIBS = -lpng

OBJS = srtmain.o raytracer.o scene.o loadscene.o

all : pa3

clean : 
	rm -f pa3 *.o

loadscene.o: mesh.h ball.h triangle.h camera.h cylinder.h

srtmain.o: srtmain.cpp scene.h raytracer.h

raytracer.o: raytracer.cpp raytracer.h camera.h scene.h

pa3 : $(OBJS)
	$(CC) $(CPPFLAGS) -o $@ $(COPTS) $(OBJS) $(LIBS) 

