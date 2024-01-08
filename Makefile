execute: main.o
	g++ main.o -o binary
main.o: 
	g++ -c assignment3.cpp	-I./ -o main.o
clean:
	rm *.o binary