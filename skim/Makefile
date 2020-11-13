COMPILER=$(shell root-config --cxx)
FLAGS=$(shell root-config --cflags --libs)

tnptreeSkim: skim.cxx
		$(COMPILER) -g -O3 -Wall -Wextra -Wpedantic -o skim skim.cxx $(FLAGS) -lTMVA -lXMLIO -lquadmath

clean:
		rm -rf core  *.o skim
