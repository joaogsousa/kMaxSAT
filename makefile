all: parser.o main.o solve.o
	g++ main.o parser.o solve.o -std=c++11 -Wall -o exe

parser.o: src/parser.cpp src/parser.hpp
	g++ src/parser.cpp -W -c -o parser.o

main.o: src/main.cpp
	g++ src/main.cpp -c -o main.o

solve.o: src/solve.cpp src/solve.hpp
	g++ src/solve.cpp -c -o solve.o
