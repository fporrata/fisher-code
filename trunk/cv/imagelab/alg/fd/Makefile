CVFILES 	:= EdgeDector.cpp
HEADERS 	:= EdgeDector.h

CXX			= g++
CPPFLAGS	= `pkg-config --cflags opencv`
OUT			:= EdgeDector.o

#${OUT}: ${CVFILES} ${HEADERS}
#${CXX} -c $(CXXFLAGS) $< -o $@

EdgeDector.o: EdgeDector.cpp

clean:
	rm -rf *.o
