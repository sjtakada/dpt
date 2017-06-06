all: dpt

main.o: main.cpp LdnsQuery.hpp QueryRunner.hpp RandomStringGenerator.hpp StatsStore.hpp
	g++ -c -Wno-deprecated -std=c++11 -I/usr/include/mysql -I/usr/include/mysql++ main.cpp

dpt: main.o
	g++ -o dpt main.o -lldns -lmysqlpp
