CVFILES 	:= EdgeDector.cpp
HEADERS 	:= EdgeDector.h

CXX			:= g++
CXXFLAGS	:= `pkg-config --cflags opencv`
OUT			:= EdgeDector.o

${OUT}: ${CVFILES} ${HEADERS}
	${CXX} -c $(CXXFLAGS) $< -o $@

clean:
	rm -rf *.o
