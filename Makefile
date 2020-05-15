## Makefile
# Authors: xsedla1h, xlipka00
#

all: make

.PHONY: clean

doxygen:
	doxygen ./src/Doxyfile

run: make
	./src/icp
qmake:
	cd src/ && qmake;

make: qmake
	cd src/ && make

pack: clean
	zip -r xsedla1h-xlipka02.zip src README.txt examples Makefile doc
clean:
	rm -rf doc/*
	cd src/ && rm -f *.o && rm -f icp && rm -f moc_[a-zA-Z]*.[a-zA-Z]* &&\
	rm -f ui_[a-zA-Z]*.[a-zA-Z]* && rm -f Makefile && rm -f *.pro.user
