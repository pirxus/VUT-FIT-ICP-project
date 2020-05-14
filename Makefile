CPPFLAGS= -std=c++17 -Wall -Wextra -pedantic -g
SRC= src/*.cpp
HEAD= src/*.h

all: run

.PHONY: clean

doxygen:
	doxygen ./src/icp/Doxyfile

run:
	cd src/icp; qmake -project; qmake -gui -widgets; \
	make ; ./icp
clean:
	rm -rf doc/*
