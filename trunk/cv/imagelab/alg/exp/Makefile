vpath	%.cpp 	../util ../lib ../class
vpath	%.h		../include

CXXFLAGS		:= -I ../include `pkg-config --cflags opencv` \
					`pkg-config --libs opencv` -g

MAIN		:= edgedetect showImage testChamfer batchedges \
			   matchbyedge

all: ${MAIN}

testChamfer: testChamfer.cpp chamfer.o cvboost.o
edgedetect: edgedetect.cpp EdgeDector.o cvboost.o
showImage: showImage.cpp
batchedges: batchedges.cpp EdgeDector.o cvboost.o
matchbyedge: matchbyedge.cpp cvboost.o chamfer.o

chamfer.o: chamfer.cpp
EdgeDector.o: EdgeDector.cpp
cvboost.o: cvboost.cpp

clean:
	rm -rf *.o ${MAIN}
