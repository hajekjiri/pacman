CXX=g++
CXXFLAGS=-Wall -pedantic -Wno-long-long -O0 -ggdb -std=c++14

all: doc compile

compile: ./src/main.o ./src/Blank.o ./src/Game.o ./src/Ghost.o \
         ./src/MenuElement.o ./src/MovingGameObject.o ./src/Portal.o \
         ./src/Wall.o ./src/Coin.o ./src/GameObject.o ./src/Map.o \
         ./src/Menu.o ./src/Pacman.o ./src/SolidGameObject.o \
         ./src/MyException.o ./src/BonusCoin.o
	# link, save output to '<login>'
	g++ -o hajekj29 ./src/main.o ./src/Blank.o ./src/Game.o ./src/Ghost.o \
	                ./src/MenuElement.o ./src/MovingGameObject.o ./src/Portal.o \
	                ./src/Wall.o ./src/Coin.o ./src/GameObject.o ./src/Map.o \
	                ./src/Menu.o ./src/Pacman.o ./src/SolidGameObject.o \
	                ./src/MyException.o ./src/BonusCoin.o -lncurses

run:
	# run
	./hajekj29

clean:
	# remove generated files
	rm -rvf doc
	rm -vf hajekj29 ./src/*.o ./src/*.gch

doc:
	# generate documentation
	doxygen ./src/Doxyfile

lines:
	# count number of lines
	wc -l ./src/*h ./src/*.cpp

r: compile run
	# custom target for simplicity, delete later

%o: %cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<


./src/Blank.o: ./src/Blank.cpp ./src/Blank.h
./src/Coin.o: ./src/Coin.cpp ./src/Coin.h
./src/Game.o: ./src/Game.cpp ./src/Game.h
./src/GameObject.o: ./src/GameObject.cpp ./src/GameObject.h
./src/Ghost.o: ./src/Ghost.cpp ./src/Ghost.h
./src/main.o: ./src/main.cpp
./src/Map.o: ./src/Map.cpp ./src/Map.h
./src/MenuElement.o: ./src/MenuElement.cpp ./src/MenuElement.h
./src/Menu.o: ./src/Menu.cpp ./src/Menu.h
./src/MovingGameObject.o: ./src/MovingGameObject.cpp ./src/MovingGameObject.h
./src/Pacman.o: ./src/Pacman.cpp ./src/Pacman.h
./src/Portal.o: ./src/Portal.cpp ./src/Portal.h
./src/SolidGameObject.o: ./src/SolidGameObject.cpp ./src/SolidGameObject.h
./src/Wall.o: ./src/Wall.cpp ./src/Wall.h
./src/MyException.o: ./src/MyException.cpp ./src/MyException.h
./src/BonusCoin.o: ./src/BonusCoin.cpp ./src/BonusCoin.h
