all: doc compile

compile:
	# compile, save output to '<login>'
	g++ -Wall -pedantic -Wno-long-long -O0 -ggdb -o hajekj29 \
	  ./src/*.h                                              \
	  ./src/*.cpp

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
