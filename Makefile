COMPILER=$(shell root-config --cxx)
FLAGS=$(shell root-config --cflags --libs)

skim: tnpper.cxx
		$(COMPILER) -g -O3 -Wall -Wextra -Wpedantic -o tnpper tnpper.cxx $(FLAGS)

clean:
		rm -rf core tnpper *.o
