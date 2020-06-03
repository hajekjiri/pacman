CXX = g++

LD = g++

CXXFLAGS = -Wall -pedantic -Wno-long-long -O0 -ggdb -std=c++11

HEADERFILES = ./src/Game.h ./src/MenuElement.h ./src/Menu.h             \
              ./src/MovingGameObject.h ./src/GameObject.h ./src/Map.h   \
              ./src/BfsPathFinder.h ./src/Portal.h ./src/MyException.h  \
              ./src/CommonFunctions.h ./src/Setting.h

OBJECTFILES = ./src/main.o ./src/Game.o ./src/MenuElement.o ./src/Menu.o \
              ./src/MovingGameObject.o ./src/GameObject.o ./src/Map.o    \
              ./src/BfsPathFinder.o ./src/Portal.o ./src/MyException.o   \
              ./src/CommonFunctions.o ./src/Setting.o

all: doc compile

pacman: $(OBJECTFILES)
	# link, save output to 'pacman'
	$(LD) -o pacman $^ -lncurses

compile: pacman

run: pacman
	# run with sample settings
	./pacman ./examples/settings.cfg

clean:
	# remove generated files
	rm -rvf doc
	rm -vf pacman ./src/*.o ./src/*.gch

doc: ./Doxyfile $(HEADERFILES) ./introduction.dox
	# generate documentation
	doxygen ./Doxyfile

lines:
	wc -l ./src/*.h ./src/*.cpp

lines2:
	gcc -fpreprocessed -dD -E ./src/*.h ./src/*.cpp | sed '/^\s*$$/d' | wc -l

# get .o from .cpp and .h
%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

