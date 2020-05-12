CPPFLAGS= -std=c++17 -Wall -Wextra -pedantic -g
SRC= src/*.cpp
HEAD= src/*.h

all: test

test: $(HEAD) $(SRC)
	g++ $(CPPFLAGS) $(SRC) -o main


.PHONY: clean

doxygen:
	$(MAKE) -C src

clean:
	rm -rf doc/*