SHELL=/bin/bash

CPP = g++
CCFLAGS = -O3 -pedantic -Wall -Wextra -std=c++1z
LDLINK= -L/usr/local/opt/openssl@1.1/lib 
CPPLINK= -I/usr/local/opt/openssl@1.1/include 
CCLINK = -lssl -lcrypto
CCNAME = -o BOND

ifeq ($(OS), Windows_NT)
	CCFLAGS += -D WIN32 -fopenmp
	ifeq ($(PROCESSOR_ARCHITEW6432),AMD64)
		CCFLAGS += -D AMD64
	else
		ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
			CCFLAGS += -D AMD64
		endif
		ifeq ($(PROCESSOR_ARCHITECTURE),x86)
			CCFLAGS += -D IA32
		endif
	endif
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S), Linux)
		CCFLAGS += -D LINUX -fopenmp
	endif
	ifeq ($(UNAME_S), Darwin)
		CCFLAGS += -D OSX -Xclang -fopenmp 
		CCLINK += -lomp
	endif
	UNAME_P := $(shell uname -p)
	ifeq ($(UNAME_P), x86_64)
		CCFLAGS += -D AMD64
	endif
	ifneq ($(filter %86,$(UNAME_P)),)
		CCFLAGS += -D IA32
	endif
	ifneq ($(filter arm%,$(UNAME_P)),)
		CCFLAGS += -D ARM
	endif
endif

OBJ_MAIN = src/bond.o
OBJ_FUNC = src/bond_commands/bond_commands.o \
	src/bond_functions/bond_functions.o \
	src/cryptography/aes_encrypt.o \
	src/cryptography/crypt_ssl.o
HDR_MDRZ = src/bond_commands/bond_commands.hpp \
	src/bond_functions/bond_functions.hpp \
	src/cryptography/aes_encrypt.hpp \
	src/cryptography/crypt_ssl.hpp

all: bond

bond: $(OBJ_MAIN) $(OBJ_FUNC)
	$(CPP) $(CCFLAGS) $(CCNAME) \
	$(OBJ_MAIN) $(OBJ_FUNC) \
	$(LDLINK) $(CPPLINK) $(CCLINK)

test:
	mkdir .output
	./mediarizer -i img -o .output -D -r -p
	rm -rf .output

clean:
	rm -f BOND src/*.o src/bond_commands/*.o src/bond_functions/*.o \
	src/archive/*.o src/cryptography/*.o

%.o: %.cpp $(HDR_MDRZ)
	$(CPP) $(CCFLAGS) $(LDLINK) $(CPPLINK) $(CCLINK) -o $@ -c $<