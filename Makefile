all:
	clear

	g++ -o bond pass_read.cpp bond.cpp

	./bond 
	
clean:
	bond
