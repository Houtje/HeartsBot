# Makefile for the Hearts program
all: hearts

hearts: main.o heartsbot.o heartsfield.o
	g++ -O2 main.o heartsbot.o heartsfield.o -o hearts

main.o: main.cc
	g++ -O2 -c main.cc

heartsbot.o: heartsbot.cc
	g++ -O2 -c heartsbot.cc

heartsfield.o: heartsfield.cc
	g++ -O2 -c heartsfield.cc

clean:
	rm *o hearts
