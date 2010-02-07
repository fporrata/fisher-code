CXXFLAGS		:= -I ../fd `pkg-config --cflags opencv` \
					`pkg-config --libs opencv`

MAIN		:= edgedetect

$(MAIN): $(MAIN).cpp
	${CXX} ${CXXFLAGS} $< ../fd/EdgeDector.o -o $@


clean:
	rm -rf *.o
