COMPILER=$(shell root-config --cxx)
FLAGS=$(shell root-config --cflags --libs)

ntuple: nanotnp.cxx
		$(COMPILER) -g -O3 -Wall -Wextra -Wpedantic -o nanotnp nanotnp.cxx $(FLAGS)

clean:
		rm -rf core nanotnp *.o
