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

#define bold_on		"\033[1m"
#define bold_off	"\033[22m"

struct binary_reg
{
	char identity[32];
	char username[32];
	char password[32];

	struct binary_reg *next;
};

inline bool 
file_exists(const std::string& name);

void 
change(struct binary_reg *a, struct binary_reg *b);

void 
check(struct binary_reg *start);

void
sort(struct binary_reg *head);

FILE *
file_open(std::string name);

void
encrypt(struct binary_reg *head, char *name, 
	char *sha256_key_c, std::string master_key);

struct binary_reg *
decrypt(std::string master_key, FILE *file_db);