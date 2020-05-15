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
	cd src/ && /home/pirx/Qt5.9.9/5.9.9/gcc_64/bin/qmake;

make: qmake
	cd src/ && make

pack: clean
	zip -r xsedla1h_xlipka02.zip src README.txt examples Makefile doc
clean:
	rm -rf doc/*
	cd src/ && rm -f *.o && rm -f icp && rm -f moc_[a-zA-Z]*.[a-zA-Z]* &&\
	rm -f ui_[a-zA-Z]*.[a-zA-Z]* && rm -f Makefile && rm -f *.pro.user
