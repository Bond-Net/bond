#ifndef BOND_FUNC_H
#define BOND_FUNC_H

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

bool
insert(struct binary_reg **head, struct binary_reg **tail);

bool
delete_pass(struct binary_reg **head, struct binary_reg **tail);

bool
list_all(struct binary_reg *head);

bool
list_from(struct binary_reg *head);

bool
edit(struct binary_reg *head);

#endif // BOND_HUNC_H