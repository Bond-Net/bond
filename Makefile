all:
	clear

	g++ -o bond bond.cpp pass_read.cpp sha256.cpp 

	./bond 
	
clean:
	bond
