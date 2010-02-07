CVFILES 	:= EdgeDector.cpp
HEADERS 	:= EdgeDector.h

CXX			:= g++
CXXFLAGS	:= -bind_at_load `pkg-config --cflags opencv`
OUT			:= EdgeDector.o

${OUT}: ${CVFILES} ${HEADERS}
	${CXX} -c $(CXXFLAGS) $< -o $@

