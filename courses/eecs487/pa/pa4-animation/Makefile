PA = pa4
PA_NAME = animation
STUDENT_SRC_FILES = main.cpp x3reader.h x3reader.cpp \
	 x3.h x3.cpp image_io.h image_io.cpp image.h image.cpp xvec.h
STUDENT_OTHER_FILES = ./data ./common

CC = g++
# INCLUDES = ./common/include
CFLAGS = -g -Wall -m32 $(INCLUDES)
ifeq ($(shell uname), Darwin)
  LIBS = -framework OpenGL -framework GLUT -lm -lexpat -lpng -ljpeg
else
  LIBS = -lGL -lGLU -lglut -lm -lexpat -lpng -ljpeg
endif

#	this is to make the executable. $(1) may be '-D _SOLUTION'
build-executable = \
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(1) $(LIBS)

all: $(PA)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

main.o: main.cpp x3reader.h x3.h
x3.o: x3.cpp x3.h xvec.h image_io.h
x3reader.o: x3reader.cpp x3reader.h x3.h xvec.h
image.o: image.cpp image.h
image_io.o: image_io.cpp image_io.h

OBJS = main.o x3.o x3reader.o image.o image_io.o

.PHONY: clean
clean:
	-rm -f -r *.o *~ *core* $(PA) soln $(PA).tgz

soln: $(OBJS)
	$(call build-executable,-D _SOLUTION)

$(PA): $(OBJS)
	$(call build-executable,)

tgz: $(STUDENT_SRC_FILES) Makefile
	rm -r -f $(PA)-$(PA_NAME)
	mkdir $(PA)-$(PA_NAME)
	cp -f -r $(STUDENT_SRC_FILES) $(STUDENT_OTHER_FILES) \
		Makefile $(PA)-$(PA_NAME)
	for file in $(STUDENT_SRC_FILES); do \
		sed -e '/#ifdef _SOLUTION/,/#endif \/\/ _SOLUTION/ d' \
			-e '/#ifndef _SOLUTION/ d' \
			-e '/#endif \/\/ _SOLUTION/ d' \
	 		<$$file >$(PA)-$(PA_NAME)/$$file; \
	done
	sed -e '/PA = /,/^$$/d' \
		-e '/soln:/,//d' \
		-e 's/soln //' \
		-e 's/$$(LAB).tgz//' \
		-e 's/build-executable = \\/$$(PA): $$(OBJS)/g' \
		-e $(subst src,$(OBJS),'s/$$(OBJS)/src/g') \
		-e $(subst name,$(PA),'s/$$(PA)/name/g') \
		-e 's/$$(1)[ ]*//' \
		-e '/#/d' \
		 <Makefile>$(PA)-$(PA_NAME)/Makefile
	tar cvfz $(PA).tgz $(PA)-$(PA_NAME)
	rm -r -f $(PA)-$(PA_NAME)
