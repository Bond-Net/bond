COMPILER=g++
OPENSSLF=-lssl -lcrypto
STDVERSN=-std=c++11
FILENAME=-o bond
FILEDIRC=bond.cpp pass_read/pass_read.cpp

all:
	clear
	$(info cleared terminal) $(info)

	$(COMPILER) $(FILENAME) $(FILEDIRC) $(OPENSSLF) $(STDVERSN)
	
	./bond 
	
clean:
	bond
