CPPFLAGS= -std=c++17 -Wall -Wextra -pedantic -g
SRC= src/*.cpp
HEAD= src/*.h

all: run

.PHONY: clean

doxygen:
	doxygen ./src/icp/Doxyfile

run:
	cd src/icp; qmake; \
	make; ./icp
pack:
	zip -r xsedla1h_xlipka02.zip * 
clean:
	rm -rf doc/*
	cd src/icp; rm *.o; rm icp; rm moc_[a-zA-Z]*.[a-zA-Z]*; rm ui_[a-zA-Z]*.[a-zA-Z]*; 
