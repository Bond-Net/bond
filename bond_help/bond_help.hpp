#ifndef BOND_HELP_H
#define BOND_HELP_H

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
#include <getopt.h>

#define a_c_r		"\x1b[31m"
#define a_c_g		"\x1b[32m"
#define a_c_y		"\x1b[33m"
#define a_c_b		"\x1b[34m"
#define a_c_m		"\x1b[35m"
#define a_c_c		"\x1b[36m"
#define a_c_re		"\x1b[0m"
#define under		"\e[4m"
#define under_re	"\e[0m"
#define italic		"\e[3m"
#define italic_re	"\e[0m"
#define bold_on	 	"\e[1m"
#define bold_re		"\e[0m"

#define prnt_inf(MSG) \
{ \
	fprintf(stderr, "MESSG: " a_c_y "%-60s" a_c_re " | [%-20s, %-5d]\n", \
		std::string(MSG).c_str(), __FILE__, __LINE__); \
}

typedef struct binary_reg
{
	char identity[128];
	char username[128];
	char password[128];

	int identity_len;
	int username_len;
	int password_len;
	
	struct binary_reg *next;
	struct binary_reg *prev;
}
binary_reg;

bool
file_exists(const std::string& name);

FILE *
file_open(std::string file, std::string type);

bool
arg_int(int argc, char** argv, std::string *filename, bool *verbose);

#endif // BOND_HELP_H