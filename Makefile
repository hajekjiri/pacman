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

# get .o from .cpp
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

./src/CommonFunctions.o: ./src/CommonFunctions.cpp ./src/CommonFunctions.h
./src/Game.o: ./src/Game.cpp ./src/Game.h
./src/GameObject.o: ./src/GameObject.cpp ./src/GameObject.h
./src/main.o: ./src/main.cpp
./src/Map.o: ./src/Map.cpp ./src/Map.h
./src/MenuElement.o: ./src/MenuElement.cpp ./src/MenuElement.h
./src/Menu.o: ./src/Menu.cpp ./src/Menu.h
./src/MovingGameObject.o: ./src/MovingGameObject.cpp ./src/MovingGameObject.h
./src/Portal.o: ./src/Portal.cpp ./src/Portal.h
./src/MyException.o: ./src/MyException.cpp ./src/MyException.h
./src/BfsPathFinder.o: ./src/BfsPathFinder.cpp ./src/BfsPathFinder.h
./src/Setting.o: ./src/Setting.cpp ./src/Setting.h
