#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <ctype.h>

#include <sys/stat.h>
#include <unistd.h>
#include <fstream>	//File I/O
#include <iomanip>	//Convenient spacing
#include <cstring>	//Manipulate c-strings

#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/aes.h>
#include <openssl/sha.h>

#include "bond_func/bond_func.hpp"
#include "bond_help/bond_help.hpp"
#include "crypt_ssl/crypt_ssl.hpp"
#include "pass_read/pass_read.hpp"

int
main(int argc, char *argv[])
{
	std::string msg, usr_msg1, usr_msg2, usr_msg3, usr_msg4, filename;
	std::string master_key, sha256_key;

	char sha256_key_c[96];
	struct binary_reg *head = NULL, *tail = NULL, *reader = NULL, 
		*prev = NULL, *row_from_db_prev = NULL, *row_from_db = NULL;

	int not_gone_through = 1;
	bool verbose = false;

	arg_int(argc, argv, &filename, &verbose);

	if(file_exists(filename))
	{
		std::cout	<< "Enter the master key in order to open the vault: "
					<< std::endl << std::endl;

		std::ifstream file_db(filename, std::ios::in | std::ios::binary);
		if (!file_db.is_open())
		{
			std::cout << "failed to open " << filename << '\n';
		}
		else
		{
			file_db.read(reinterpret_cast<char *>(&sha256_key_c), sizeof(char) * 96);
		
			sha256_key = sha256_key_c;

			while(true)
			{
				std::cout << "> enter master key:\t";

				if(sha256(master_key = getpass()) != sha256_key)
				{
					std::cout	<< bold_on << "incorrect password, try again"
								<< bold_re << std::endl << std::endl;
					continue;
				}

				std::cout	<< "code is:   " << master_key << std::endl
							<< "sha256 is: " << sha256_key << std::endl
							<< "Correct password you have unlocked the vault !"
							<< std::endl << std::endl;
				break;
			}
		}

		list_decrypt(&head, &tail, master_key, &file_db);

		file_db.close();
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
							<< bold_re  << std::endl << std::endl;
				continue;
			}

			std::ofstream file_db(filename, std::ios::binary);
			file_db.write(sha256_key.c_str(), sizeof(char) * 96);
			file_db.close();

			break;
		}
	}

	while(true)
	{
		std::cout << "\n" bold_on "command: " bold_re ;
		std::cin >> usr_msg1;

		if(usr_msg1 == "exit" || usr_msg1 == "quit" || usr_msg1 == "q")
		{
			list_encrypt(head, filename, sha256_key, master_key);
			break;
		}
		else if(usr_msg1 == "reset")
		{
			if(file_exists(filename))
			{
				std::ofstream file_db(filename, std::ios::binary);
				file_db.write(sha256_key_c, sizeof(char) * 96);
				file_db.close();
			}
			else
			{
				std::cout << "you do not have a key list" << std::endl;
			}
		}
		else if(usr_msg1 == "delete-file")
		{
			if(file_exists(filename))
			{
				remove(filename.c_str());
				head = NULL;
				tail = NULL;
			}
		}
		else if(usr_msg1 == "insert" || usr_msg1 == "i")
		{
			insert(&head, &tail);
		}
		else if(usr_msg1 == "delete-pass")
		{
			if(file_exists(filename))
			{
				delete_pass(head, tail);
			}
			else
			{
				std::cout << "you do not have a key list" << std::endl;
			}
		}
		else if(usr_msg1 == "list-all" || usr_msg1 == "ls")
		{
			if(file_exists(filename))
			{
				list_all(head);
			}
			else
			{
				std::cout << "you do not have a key list" << std::endl;
			}
		}
		else if(usr_msg1 == "list-from")
		{
			if(file_exists(filename))
			{
				list_from(head);
			}
			else
			{
				std::cout << "you do not have a key list" << std::endl;
			}
		}
		else if(usr_msg1 == "edit")
		{
			if(file_exists(filename))
			{
				edit(head);
			}
			else
			{
				std::cout << "you do not have a key list" << std::endl;
			}
		}
		else if(usr_msg1 == "sort")
		{
			if(head == NULL)
			{
				std::cout << "your key list is empty" << std::endl;
			}
			else
			{
				if(file_exists(filename))
				{
					reader = head;
					sort(reader);
				}
				else
				{
					std::cout << "you do not have a key list" << std::endl;
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

	exit(EXIT_SUCCESS);
}