vpath	%.cpp 	../util ../lib ../class
vpath	%.h		../include

CXXFLAGS		:= -I ../include `pkg-config --cflags opencv` \
					`pkg-config --libs opencv` -g

MAIN		:= edgedetect showImage testChamfer

all: ${MAIN}

testChamfer: testChamfer.cpp chamfer.o cvboost.o
edgedetect: edgedetect.cpp EdgeDector.o
showImage: showImage.cpp

chamfer.o: chamfer.cpp
EdgeDector.o: EdgeDector.cpp
cvboost.o: cvboost.cpp

clean:
	rm -rf *.o ${MAIN}
