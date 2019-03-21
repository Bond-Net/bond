#ifndef CRYPT_SSL_H
#define CRYPT_SSL_H

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <ctype.h>

#include <sys/stat.h>
#include <unistd.h>
#include <fstream>		//File I/O
#include <iomanip>		//Convenient spacing
#include <cstring>		//Manipulate c-strings
#include <sstream>

#include "../crypt_ssl/aes_encrypt.hpp"
#include "../bond_help/bond_help.hpp"

std::string
sha256(const std::string str);

char *
sha256(char *str);

void
list_encrypt(struct binary_reg *head, std::string filename,
	char *sha256_key, char *sha256_iv, std::string master_key, std::string master_iv);

void
list_decrypt(struct binary_reg **head, struct binary_reg **tail,
	std::string master_key, std::string master_iv, std::ifstream *file_db);

#endif // CRYPT_SSL_H