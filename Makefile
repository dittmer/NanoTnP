COMPILER=$(shell root-config --cxx)
FLAGS=$(shell root-config --cflags --libs)

skim: skim.cxx
		$(COMPILER) -g -O3 -Wall -Wextra -Wpedantic -o skim skim.cxx $(FLAGS)

clean:
		rm -rf core skim *.o
