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
	int swapped;
	struct binary_reg *current;
	struct binary_reg *next_one = NULL;

	if (current == NULL) return;

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
encrypt(struct binary_reg *head, char *name, char *sha256_key_c, std::string master_key)
{
	std::string
		msg;
	struct binary_reg
		*reader = head;
	FILE
		*new_file_db = file_open("new_keylist.dat", "w");

	fwrite(sha256_key_c, sizeof(char) * 70, 1, new_file_db);

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

	remove(name);
	rename("new_keylist.dat", name);
}

struct binary_reg *
decrypt(std::string master_key, FILE *file_db)
{
	std::string
		msg;
	struct binary_reg
		*head = NULL,
		*row_from_db_prev = NULL,
		*row_from_db = NULL;
	int
		not_gone_through = 1;

	row_from_db = (binary_reg *) malloc (sizeof(struct binary_reg));
	head = row_from_db;

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

	if(not_gone_through) head = NULL;
	else row_from_db_prev->next = NULL;

	return head;
}

int
main(int argc, char *argv[])
{
	std::string
		msg,
		master_key,
		sha256_key;
	char
		sha256_key_c[64];
	struct binary_reg
		*head = NULL,
		*reader = NULL,
		*row_from_db_prev = NULL,
		*row_from_db = NULL;
	int
		not_gone_through = 1;
	FILE
		*file_db;


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

		head = decrypt(master_key, file_db);

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

	if(argc >= 2)
	{
		if(strcmp(argv[1], "--reset") == 0)
		{
			if(file_exists("keylist.dat"))
			{
				FILE *file_db = file_open("keylist.dat", "wb");
				fwrite(sha256_key_c, sizeof(char) * 70, 1, file_db);
				fclose(file_db);
			}
		}
		else if(strcmp(argv[1], "--delete-file") == 0)
		{
			// deleting the file
			remove("keylist.dat");
		}
		else if(strcmp(argv[1], "--insert") == 0)
		{
			std::string msg;
			struct binary_reg append_movie;

			FILE *new_file_db = file_open("new_keylist.dat", "ab");
			fwrite(sha256_key_c, sizeof(char) * 70, 1, new_file_db);

			reader = head;
			while(reader != NULL)
			{
				fwrite(reader, sizeof(struct binary_reg), 1, new_file_db);
				reader = reader->next;
			}

			strcpy(append_movie.identity, encrypt(msg=argv[2], master_key).c_str());
			strcpy(append_movie.username, encrypt(msg=argv[3], master_key).c_str());
			strcpy(append_movie.password, encrypt(msg=argv[4], master_key).c_str());

			append_movie.next = NULL;
		
			fwrite(&append_movie, sizeof(struct binary_reg), 1, new_file_db);

			fclose(new_file_db);

			remove("keylist.dat");
			rename("new_keylist.dat", "keylist.dat");
		}
		else if(strcmp(argv[1], "--delete-pass") == 0)
		{
			if(file_exists("keylist.dat"))
			{
				FILE *new_file_db = file_open("new_keylist.dat", "w");
				fwrite(sha256_key_c, sizeof(char) * 70, 1, new_file_db);

				reader = head;
				while(reader != NULL)
				{
					if((strcmp(reader->identity, argv[2]) +
						strcmp(reader->username, argv[3])) != 0)
							fwrite(reader, sizeof(struct binary_reg), 1, new_file_db);

					reader = reader->next;
				}

				fclose(new_file_db);

				remove("keylist.dat");
				rename("new_keylist.dat", "keylist.dat");
			}
		}
		else if(strcmp(argv[1], "--list-all") == 0)
		{
			if(file_exists("keylist.dat"))
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
		else if(strcmp(argv[1], "--list-from") == 0)
		{
			if(file_exists("keylist.dat"))
			{
				reader = head;
				while(reader != NULL)
				{
					if(!strcmp(reader->identity, argv[2]))
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
		else if(strcmp(argv[1], "--edit") == 0)
		{
			if(file_exists("keylist.dat"))
			{
				FILE *new_file_db = file_open("new_keylist.dat", "w");
				fwrite(sha256_key_c, sizeof(char) * 70, 1, new_file_db);

				reader = head;
				while(reader != NULL)
				{
					if((strcmp(reader->identity, argv[2]) + strcmp(reader->username, argv[3])) != 0)
					{
						fwrite(reader, sizeof(struct binary_reg), 1, new_file_db);
					}
					else
					{
						strcpy(reader->password, argv[4]);
						fwrite(reader, sizeof(struct binary_reg), 1, new_file_db);
					}

					reader = reader->next;
				}

				fclose(new_file_db);

				remove("keylist.dat");
				rename("new_keylist.dat", "keylist.dat");
			}
		}
		else if(strcmp(argv[1], "--sort") == 0)
		{
			if(file_exists("keylist.dat"))
			{
				FILE *new_file_db = file_open("new_keylist.dat", "w");
				fwrite(sha256_key_c, sizeof(char) * 70, 1, new_file_db);

				reader = head;
				sort(reader);
				
				reader = head;
				while(reader != NULL)
				{
					fwrite(reader, sizeof(struct binary_reg), 1, new_file_db);
					reader = reader->next;
				}

				fclose(new_file_db);

				remove("keylist.dat");
				rename("new_keylist.dat", "keylist.dat");
			}
		}
		else
		{
			printf(
				"Usage: %s <command>, commands are:\n"
				"\t--insert <new identity> <new username> <new password>\n"
				"\t--delete-pass <identity> <username>\n"
				"\t--delete-file\n"
				"\t--edit <identity> <username> <new password>\n"
				"\t--list-all\n"
				"\t--list-from <identity>\n"
				"\t--sort (alphabetically)\n"
				"\t--reset\n"
				,
				argv[1]
			);
		}
	}
	else
	{
		printf(
			"Usage: %s <command>, commands are:\n"
			"\t--insert <new identity> <new username> <new password>\n"
			"\t--delete-pass <identity> <username>\n"
			"\t--delete-file\n"
			"\t--edit <identity> <username> <new password>\n"
			"\t--list-all\n"
			"\t--list-from <identity>\n"
			"\t--sort (alphabetically)\n"
			"\t--reset\n"
			,
			argv[0]
		);
	}

	return 0;
}