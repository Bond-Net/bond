all:
	clear

	g++ -std=c++11 -o bond bond.cpp pass_read/pass_read.cpp sha256/sha256.cpp 

	./bond 
	
clean:
	bond
