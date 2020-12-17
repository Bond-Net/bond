#ifndef BOND_HELP_H
#define BOND_HELP_H

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <ctype.h>

#include <sys/stat.h>
#include <unistd.h>
#include <fstream> //File I/O
#include <iomanip> //Convenient spacing
#include <cstring> //Manipulate c-strings
#include <getopt.h>

#include <termios.h>

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
} binary_reg;

FILE *file_open(std::string file, std::string type);
bool file_exists(const std::string &name);
bool arg_int(int argc, char **argv, std::string *filename, bool *verbose);
int getch(void);
std::string get_pass(bool show_asterisk);

#endif // BOND_HELP_H