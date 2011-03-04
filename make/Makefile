objects = utest.o main.o

.PHONY: run clean

main : $(objects)
	g++ -o tests $(objects)

utest.o : ../src/utest.cpp ../src/utest.h Makefile
	g++ -c ../src/utest.cpp

main.o  : ../tests/main.cpp ../src/utest.h Makefile
	g++ -c ../tests/main.cpp

run :
	@./tests

clean :
	-rm tests $(objects)
