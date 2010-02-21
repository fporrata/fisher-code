vpath %.h ../include
CVFILES 	:= EdgeDector.cpp
HEADERS 	:= EdgeDector.h

CXX			= g++ -I ../include
CPPFLAGS	+= `pkg-config --cflags opencv`
OUT			:= EdgeDector.o

#${OUT}: ${CVFILES} ${HEADERS}
#${CXX} -c $(CXXFLAGS) $< -o $@

EdgeDector.o: EdgeDector.cpp 

.PHONY: clean

clean:
	rm -rf *.o
