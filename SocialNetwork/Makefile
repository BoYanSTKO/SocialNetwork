# CXX=clang++
CXX=g++

default: all

main.o: main.cpp
	${CXX} -c main.cpp

Util.o: Util.cpp
	${CXX} -c Util.cpp

BTree.o: BTree.cpp
	${CXX} -c BTree.cpp

FriendshipGraph.o: FriendshipGraph.cpp
	${CXX} -c FriendshipGraph.cpp

all: main.o Util.o BTree.o FriendshipGraph.o
	${CXX} main.o Util.o BTree.o FriendshipGraph.o -o prog3

clean: 
	/bin/rm -f *.o main.o Util.o BTree.o FriendshipGraph.o prog3
