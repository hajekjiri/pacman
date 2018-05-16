all: doc compile

compile:
	# compile, save output to '<login>'
	g++ -Wall -pedantic -Wno-long-long -O0 -ggdb -std=c++14 -o hajekj29  \
	  ./src/*.h                                                          \
	  ./src/*.cpp                                                        \
	  -lncurses

run:
	# run
	./hajekj29

clean:
	# remove generated files
	rm -rvf doc
	rm -vf hajekj29

doc:
	# generate documentation
	doxygen ./src/Doxyfile

lines:
	# count number of lines
	wc -l ./src/*h ./src/*.cpp

r: compile run
	# custom target for simplicity, delete later
