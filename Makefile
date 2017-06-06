all: dpt

main.o: main.cpp LdnsQuery.hpp QueryRunner.hpp RandomStringGenerator.hpp
	g++ -c -std=c++11 main.cpp

dpt: main.o
	g++ -o dpt main.o -lldns -lmysqlpp
