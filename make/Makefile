objects = main.o

.PHONY: run clean

main : $(objects)
	g++ -o tests $(objects)

main.o  : ../tests/main.cpp ../src/utest.h Makefile
	g++ -c ../tests/main.cpp

run :
	@./tests

clean :
	-rm tests $(objects)
