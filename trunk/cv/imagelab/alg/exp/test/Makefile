vpath	%.cpp 	../../util ../../lib ../../class
vpath	%.h		../../include

CXXFLAGS		:= -I ../../include `pkg-config --cflags opencv` \
					`pkg-config --libs opencv` -g

MAIN		:= rotate

all: ${MAIN}

rotate: rotate.cpp cvboost.o

chamfer.o: chamfer.cpp
EdgeDector.o: EdgeDector.cpp
cvboost.o: cvboost.cpp

clean:
	rm -rf *.o ${MAIN}
