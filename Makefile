COMPILER=g++
OPENSSLF=-lssl -lcrypto
STDVERSN=-std=c++11
FILENAME=-o bond

LDFLAGS="-L/usr/local/opt/openssl@1.1/lib"
CPPFLAGS="-I/usr/local/opt/openssl@1.1/include"

BONDMAIN=src/bond.cpp
BONDFUNC=src/bond_commands/bond_commands.cpp
BONDHELP=src/bond_functions/bond_functions.cpp
CRYPTSSL=src/cryptography/crypt_ssl.cpp
CRYPTAES=src/cryptography/aes_encrypt.cpp

FILEDIRC=$(BONDMAIN) $(BONDFUNC) $(BONDHELP) \
	$(CRYPTSSL) $(CRYPTAES) $(LDFLAGS) $(CPPFLAGS)

all:
	# clear
	$(info cleared terminal) $(info)

	$(COMPILER) $(FILENAME) $(FILEDIRC) $(OPENSSLF) $(STDVERSN)
	
	./bond --f keylist.dat
	
clean:
	bond
