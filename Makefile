COMPILER=g++
OPENSSLF=-lssl -lcrypto
STDVERSN=-std=c++11
FILENAME=-o bond

BONDMAIN=bond.cpp
BONDFUNC=bond_func/bond_func.cpp
BONDHELP=bond_help/bond_help.cpp
CRYPTSSL=crypt_ssl/crypt_ssl.cpp
CRYPTAES=crypt_ssl/aes_encrypt.cpp
PASSREAD=pass_read/pass_read.cpp

FILEDIRC=$(BONDMAIN) $(BONDFUNC) $(BONDHELP) $(PASSREAD) $(CRYPTSSL) $(CRYPTAES)

all:
	# clear
	$(info cleared terminal) $(info)

	$(COMPILER) $(FILENAME) $(FILEDIRC) $(OPENSSLF) $(STDVERSN)
	
	./bond --f keylist.dat
	
clean:
	bond
