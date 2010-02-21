vpath %.h ../include

CPPFLAGS 	= `pkg-config --cflags opencv` -I ../include `pkg-config --libs opencv`

main: main.cpp cvboost.o
cvboost.o : cvboost.cpp
