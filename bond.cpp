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

#include "pass_gen/pass_gen.hpp"
#include "key_encryption/encrypt.h"
#include "pass_read/pass_read.hpp"
#include "sha256/sha256.hpp"

#define bold_on		"\033[1m"
#define bold_off	"\033[22m"

inline bool file_exists(const std::string& name)
{
	struct stat buffer;
	return (stat (name.c_str(), &buffer) == 0);
}

struct binary_reg
{
	char identity[32];
	char username[32];
	char password[32];

	struct binary_reg *next;
};

void change(struct binary_reg *a, struct binary_reg *b)
{
	struct binary_reg temp;

	strcpy (temp.identity, a->identity);
	strcpy (temp.username, a->username);
	strcpy (temp.password, a->password);

	strcpy (a->identity, b->identity);
	strcpy (a->username, b->username);
	strcpy (a->password, b->password);

	strcpy (b->identity, temp.identity);
	strcpy (b->username, temp.username);
	strcpy (b->password, temp.password);
}

void check(struct binary_reg *start)
{
	int
		swapped;
	struct binary_reg
		*current = NULL,
		*next_one = NULL;

	do
	{
		swapped = 0;
		current = start;

		while (current->next != next_one)
		{
			if (strcmp(current->identity, current->next->identity) > 0)
			{
				change(current, current->next);
				swapped = 1;
			}
			current = current->next;
		}
		next_one = current;
	}
	while (swapped);
}

void
sort(struct binary_reg *head)
{
	struct binary_reg *reader = head;
	check(reader);
}

FILE *
file_open(std::string file, std::string type)
{
	FILE *file_db = fopen(file.c_str(), type.c_str());
	if(file_db == NULL)
	{
		printf("Error opening keylist.dat\n");
		exit(1);
	}
	return file_db;
}

void
encrypt(struct binary_reg *head, std::string name, std::string sha256_key, std::string master_key)
{
	if(head == NULL) return;

	std::string
		msg;
	struct binary_reg
		*reader = head;
	FILE
		*new_file_db = file_open("new_keylist.dat", "w");

	fwrite(sha256_key.c_str(), sizeof(char) * 70, 1, new_file_db);

	while(reader != NULL)
	{
		//decrypt
		strcpy(reader->identity, encrypt(msg=reader->identity, master_key).c_str());
		strcpy(reader->username, encrypt(msg=reader->username, master_key).c_str());
		strcpy(reader->password, encrypt(msg=reader->password, master_key).c_str());

		fwrite(reader, sizeof(struct binary_reg), 1, new_file_db);
		reader = reader->next;
	}

	fclose(new_file_db);

	remove(name.c_str());
	rename("new_keylist.dat", name.c_str());
}

void
decrypt(struct binary_reg **head, struct binary_reg **tail, std::string master_key, FILE *file_db)
{
	std::string
		msg;
	struct binary_reg
		*row_from_db_prev = NULL,
		*row_from_db = NULL;
	int
		not_gone_through = 1;

	row_from_db = (binary_reg *) malloc (sizeof(struct binary_reg));

	*head = row_from_db;
	*tail = row_from_db;

	while (fread(row_from_db, sizeof(struct binary_reg), 1, file_db) != 0)
	{
		//decrypt
		strcpy(row_from_db->identity, decrypt(msg=row_from_db->identity, master_key).c_str());
		strcpy(row_from_db->username, decrypt(msg=row_from_db->username, master_key).c_str());
		strcpy(row_from_db->password, decrypt(msg=row_from_db->password, master_key).c_str());
		
		row_from_db->next = (binary_reg *) malloc (sizeof(struct binary_reg));
		row_from_db_prev = row_from_db;
		row_from_db = row_from_db->next;

		not_gone_through = 0;
	}
	*tail = row_from_db_prev;
	
	if(not_gone_through) *head = NULL;
	else row_from_db_prev->next = NULL;
}

int
main(int argc, char *argv[])
{
	std::string
		msg,
		usr_msg1,
		usr_msg2,
		usr_msg3,
		usr_msg4,
		master_key,
		sha256_key;
	char
		sha256_key_c[64];
	struct binary_reg
		*head = NULL,
		*tail = NULL,
		*reader = NULL,
		*prev = NULL,
		*row_from_db_prev = NULL,
		*row_from_db = NULL;
	int
		not_gone_through = 1;
	FILE
		*file_db;
	std::vector
		<std::string> tokens;

	if(file_exists("keylist.dat"))
	{
		std::cout	<< "Enter the master key in order to open the vault: "
					<< std::endl << std::endl;

		file_db = file_open("keylist.dat", "r");
		if(fread(sha256_key_c, sizeof(char) * 70, 1, file_db) != 0)
		{
			sha256_key = sha256_key_c;

			while(true)
			{
				std::cout << "> enter master key:\t";

				if(sha256(master_key = getpass()) != sha256_key)
				{
					std::cout	<< bold_on << "incorrect password, try again"
								<< bold_off<< std::endl << std::endl;
					continue;
				}

				std::cout	<< "code is:   " << master_key << std::endl
							<< "sha256 is: " << sha256_key_c << std::endl
							<< "Correct password you have unlocked the vault !"
							<< std::endl << std::endl;
				break;
			}
		}

		decrypt(&head, &tail, master_key, file_db);

		fclose(file_db);
	}
	else
	{
		std::cout	<< "It seems like you have no key list." << std::endl
					<< "Enter your new master key (remember to write it down)" << std::endl;
					
		while(true)
		{
			std::cout << "> enter master key:\t";
			sha256_key = sha256((master_key=getpass()));

			std::cout	<< "> re-enter master key:\t";
			if(sha256(getpass()) != sha256_key)
			{
				std::cout	<< bold_on << "(your passwords did not match, please retry)"
							<< bold_off<< std::endl << std::endl;
				continue;
			}

			file_db = fopen("keylist.dat", "ab");
			if(file_db == NULL)
			{
				printf("Error opening keylist.dat\n");
				exit(1);
			}
			strcpy(sha256_key_c, sha256_key.c_str());
			fwrite(sha256_key_c, sizeof(char) * 70, 1, file_db);

			fclose(file_db);
			break;
		}
	}

	while(true)
	{
		cout << "\n" bold_on "command: " bold_off;
		cin >> usr_msg1;
		if(usr_msg1 == "exit") 
		{
			encrypt(head, "keylist.dat", sha256_key, master_key);
			break;
		}

		if(usr_msg1 == "reset")
		{
			if(file_exists("keylist.dat"))
			{
				FILE *file_db = file_open("keylist.dat", "wb");
				fwrite(sha256_key_c, sizeof(char) * 70, 1, file_db);
				fclose(file_db);
			}
			else
			{
				cout << "you do not have a key list" << std::endl;
			}
		}
		else if(usr_msg1 == "delete-file")
		{
			// deleting the file
			remove("keylist.dat");
			head= NULL;
			tail = NULL;
		}
		else if(usr_msg1 == "insert")
		{
			cout << "\n" bold_on "enter <new identity> <new username> <new password>: " bold_off;
			cin >> usr_msg2 >> usr_msg3 >> usr_msg4;

			if(head == NULL)
			{
				head = (binary_reg *) malloc (sizeof(struct binary_reg));

				strcpy(head->identity, usr_msg2.c_str());
				strcpy(head->username, usr_msg3.c_str());
				strcpy(head->password, usr_msg4.c_str());
				
				tail = head;
			}
			else
			{
				tail->next = (binary_reg *) malloc (sizeof(struct binary_reg));
				tail->next->next = NULL;

				strcpy(tail->next->identity, usr_msg2.c_str());
				strcpy(tail->next->username, usr_msg3.c_str());
				strcpy(tail->next->password, usr_msg4.c_str());
				
				tail = tail->next;
			}
		}
		else if(usr_msg1 == "delete-pass")
		{
			if(file_exists("keylist.dat"))
			{
				if(head == NULL)
				{
					cout << "your key list is empty" << std::endl;
				}
				else
				{
					cout << "\n" bold_on "enter <identity> <username> of the password you want to delete: " bold_off;
					cin >> usr_msg2 >> usr_msg3;

					reader = head;
					prev = NULL;
					while(reader != NULL)
					{
						if((strcmp(reader->identity, usr_msg2.c_str()) +
							strcmp(reader->username, usr_msg3.c_str())) == 0)
						{
							if(prev == NULL)
							{
								head = head->next;
							}
							else if(reader->next != NULL)
							{
								prev->next = reader->next;
							}
							else
							{
								prev->next = NULL;
								tail = prev;
							}

							free(reader);
							break;
						}

						prev = reader;
						reader = reader->next;
					}

					if(reader == NULL) cout << "Did not find entry" << std::endl;
				}
			}
			else
			{
				cout << "you do not have a key list" << std::endl;
			}
		}
		else if(usr_msg1 == "list-all")
		{
			if(file_exists("keylist.dat"))
			{
				if(head == NULL)
				{
					cout << "your key list is empty" << std::endl;
				}
				else
				{
					reader = head;
					while(reader != NULL)
					{
						printf(
							"identity: %-20s username: %-20s password: %-20s\n",
							reader->identity ,reader->username ,reader->password
						);

						reader = reader->next;
					}
				}
			}
			else
			{
				cout << "you do not have a key list" << std::endl;
			}
		}
		else if(usr_msg1 == "list-from")
		{
			if(file_exists("keylist.dat"))
			{
				if(head == NULL)
				{
					cout << "your key list is empty" << std::endl;
				}
				else
				{
					cout << "\n" bold_on "enter <identity> you want to see credentials: " bold_off;
					cin >> usr_msg2;

					reader = head;
					while(reader != NULL)
					{
						if(!strcmp(reader->identity, usr_msg2.c_str()))
						{
							printf(
								"identity: %-20s username: %-20s password: %-20s\n",
								reader->identity ,reader->username ,reader->password
							);
						}
						reader = reader->next;
					}
				}
			}
			else
			{
				cout << "you do not have a key list" << std::endl;
			}
		}
		else if(usr_msg1 == "edit")
		{
			if(file_exists("keylist.dat"))
			{
				if(head == NULL)
				{
					cout << "your key list is empty" << std::endl;
				}
				else
				{
					cout << "\n" bold_on "enter <identity> <username> <new password> you want to edit: " bold_off;
					cin >> usr_msg2 >> usr_msg3 >> usr_msg4;

					reader = head;
					while(reader != NULL)
					{
						if((strcmp(reader->identity, usr_msg2.c_str()) +
							strcmp(reader->username, usr_msg3.c_str())) == 0)
						{
							strcpy(reader->password, usr_msg4.c_str());
							break;
						}

						reader = reader->next;
					}
				}
			}
			else
			{
				cout << "you do not have a key list" << std::endl;
			}
		}
		else if(usr_msg1 == "sort")
		{
			if(head == NULL)
			{
				cout << "your key list is empty" << std::endl;
			}
			else
			{
				if(file_exists("keylist.dat"))
				{
					reader = head;
					sort(reader);
				}
				else
				{
					cout << "you do not have a key list" << std::endl;
				}
			}
		}
		else
		{
			printf(
				"Usage: ./bond <command>, commands are:\n"
				"\tinsert\n"
				"\tdelete-pass\n"
				"\tdelete-file\n"
				"\tedit\n"
				"\tlist-all\n"
				"\tlist-from\n"
				"\tsort (alphabetically)\n"
				"\treset\n"
			);
		}

	}

	return 0;
}