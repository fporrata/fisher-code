VPATH			= ../alg_c
VPATH			= ../alg_cxx 
CXXFLAGS		:= -I ../alg_cxx `pkg-config --cflags opencv` \
					`pkg-config --libs opencv`

MAIN		:= edgedetect 

edgedetect: edgedetect.cpp EdgeDector.o
	${CXX} ${CXXFLAGS} $^ -o $@

showImage: showImage.cpp
	${CXX} ${CXXFLAGS} $^ -o $@

clean:
	rm -rf *.o edgedetect showImage
