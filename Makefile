CXX = g++

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

compile: $(OBJECTFILES)
	# link, save output to 'hajekj29'
	g++ -o hajekj29 $(OBJECTFILES) -lncurses

run: compile hajekj29
	# run with sample settings
	./hajekj29 ./examples/settings.cfg

clean:
	# remove generated files
	rm -rvf doc
	rm -vf hajekj29 ./src/*.o ./src/*.gch

doc: ./Doxyfile $(HEADERFILES) ./tutorial.dox
	# generate documentation
	doxygen ./Doxyfile

# get .o from .cpp
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

./src/CommonFunctions.o: ./src/CommonFunctions.cpp
./src/Game.o: ./src/Game.cpp
./src/GameObject.o: ./src/GameObject.cpp
./src/main.o: ./src/main.cpp
./src/Map.o: ./src/Map.cpp
./src/MenuElement.o: ./src/MenuElement.cpp
./src/Menu.o: ./src/Menu.cpp
./src/MovingGameObject.o: ./src/MovingGameObject.cpp
./src/Portal.o: ./src/Portal.cpp
./src/MyException.o: ./src/MyException.cpp
./src/BfsPathFinder.o: ./src/BfsPathFinder.cpp
./src/Setting.o: ./src/Setting.cpp
