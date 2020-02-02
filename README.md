# Pacman
## About
This project is an implementation of the Pacman game in C++ using ncurses for the Programming and Algorithmics 2 course at the [FIT, Czech Technical University in Prague](https://fit.cvut.cz/en).

## Getting started
### Requirements
* [gcc](https://gcc.gnu.org/)
* [ncurses](https://invisible-island.net/ncurses/)
* [make](https://www.gnu.org/software/make/)
* [Doxygen](http://www.doxygen.nl/)

### Setup
Clone the repository.
```
git clone https://github.com/hajekjiri/pacman.git
```

### Building the project
Use
```
make compile
```
to compile and link all files.

### Running the project
```
./hajekj29 <path-to-config>
```

An example configuration file can be found at [`examples/settings.cfg`](examples/settings.cfg). You can run the game with this example using
```
make run
```
which is equivalent to
```
./hajekj29 ./examples/settings.cfg
```

### Documentation
Documentation can be generated using [Doxygen](http://www.doxygen.nl/) and then accessed at `doc/index.html`.
```
make doc
```

### Examples
Examples are located in the [`examples`](examples) folder. Currently, there is one config file and a couple of maps that should give you an idea on how to build maps. Refer to the Doxygen-generated documentation for more information.

### Cleaning up
You can use
```
make clean
```
to clean up all unnecessary files, e.g. objects files, documentation, binaries etc.