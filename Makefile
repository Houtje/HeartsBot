# Makefile for the Hearts program
all: hearts

hearts: main.o heartsbot.o heartsfield.o
	g++ main.o heartsbot.o heartsfield.o -o hearts

main.o: main.cc
	g++ -c main.cc

heartsbot.o: heartsbot.cc
	g++ -c heartsbot.cc

heartsfield.o: heartsfield.cc
	g++ -c heartsfield.cc

clean:
	rm *o hearts
