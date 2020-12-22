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
#include <regex>

#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/aes.h>
#include <openssl/sha.h>

#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#include "bond_commands/bond_commands.hpp"
#include "bond_functions/bond_functions.hpp"
#include "cryptography/crypt_ssl.hpp"

void my_handler(int s)
{
	std::string overwrite;
	std::cout << "\nbond> any unsaved data will be lost, do you want to quit? [y/n]" << std::endl;
	std::cin >> overwrite;
	if (overwrite == "y")
		exit(EXIT_SUCCESS);
	else
		std::cin.ignore(INT_MAX);
}

bool is_bat(std::string file_name)
{
	std::regex rx("^.*\\.dat$");
	if (std::regex_match(file_name, rx))
	{
		return true;
	}
	return false;
}

int main(int argc, char *argv[])
{
	// struct sigaction sigIntHandler;
	std::string msg, filename, master_key, master_iv, sha256_key, sha256_iv;
	char sha256_key_c[128], sha256_iv_c[128];
	struct binary_reg *head = NULL, *tail = NULL;
	bool verbose = false;

	arg_int(argc, argv, &filename, &verbose);

	if (file_exists(filename))
	{
		if (!is_bat(filename))
		{
			std::cout << "key-list is always .dat file"
					  << std::endl;
			exit(EXIT_FAILURE);
		}
		std::cout << "Enter the master key in order to open the vault: "
				  << std::endl;

		std::ifstream file_db(filename, std::ios::in | std::ios::binary);
		if (!file_db.is_open())
		{
			std::cout << "failed to open " << filename << '\n';
		}
		else
		{
			file_db.read(reinterpret_cast<char *>(&sha256_key_c), sizeof(char) * 128);
			file_db.read(reinterpret_cast<char *>(&sha256_iv_c), sizeof(char) * 128);

			sha256_key = sha256_key_c;
			sha256_iv = sha256_iv_c;

			while (true)
			{
				std::cout << "> enter master key:\t";

				if (sha256(master_key = get_pass(true)) != sha256_key)
				{
					std::cout << "incorrect password, try again"
							  << std::endl
							  << std::endl;
					continue;
				}

				break;
			}

			while (true)
			{
				std::cout << "> enter master iv:\t";

				if (sha256(master_iv = get_pass(true)) != sha256_iv)
				{
					std::cout << "incorrect iv, try again"
							  << std::endl
							  << std::endl;
					continue;
				}

				std::cout << "key:       " << master_key << std::endl
						  << "iv:        " << master_iv << std::endl
						  << "key hash:  " << sha256_key << std::endl
						  << "iv  hash:  " << sha256_iv << std::endl
						  << "Correct password you have unlocked the vault !"
						  << std::endl
						  << std::endl;
				break;
			}
		}

		list_decrypt(&head, &tail, master_key, master_iv, &file_db);

		file_db.close();
	}
	else if (filename.size())
	{
		std::cout << "key-list " << filename << ", does not exist" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
	{
		std::cout << "1. sha256_key:\t"
				  << "|" << sha256_key << "|" << std::endl;
		std::cout << "1. sha256_iv:\t"
				  << "|" << sha256_iv << "|" << std::endl;

		get_master(sha256_key, sha256_iv);
		// while (true)
		// {
		// 	std::cout << "enter master key:\t";
		// 	sha256_key = sha256(get_pass(true));

		// 	std::cout << "re-enter master key:\t";
		// 	if (sha256((master_key = get_pass(true))) != sha256_key)
		// 	{
		// 		std::cout << "(your passwords did not match, please retry)"
		// 				  << std::endl
		// 				  << std::endl;

		// 		continue;
		// 	}

		// 	break;
		// }

		// while (true)
		// {
		// 	std::cout << "enter master iv:\t";
		// 	sha256_iv = sha256(master_iv = get_pass(true));
		// 	if (sha256_key == sha256_iv)
		// 	{
		// 		std::cout << "(your iv is the same with key, please retry)"
		// 				  << std::endl
		// 				  << std::endl;

		// 		continue;
		// 	}

		// 	std::cout << "re-enter master iv:\t";
		// 	if (sha256((master_iv = get_pass(true))) != sha256_iv)
		// 	{
		// 		std::cout << "(your ivs did not match, please retry)"
		// 				  << std::endl
		// 				  << std::endl;

		// 		continue;
		// 	}

		// 	break;
		// }

		std::cout << "2. sha256_key:\t"
				  << "|" << sha256_key << "|" << std::endl;
		std::cout << "2. sha256_iv:\t"
				  << "|" << sha256_iv << "|" << std::endl;

		std::cout << "3. (char *)sha256_key.c_str():\t"
				  << "|" << (char *)sha256_key.c_str() << "|" << std::endl;
		std::cout << "3. (char *)sha256_iv.c_str():\t"
				  << "|" << (char *)sha256_iv.c_str() << "|" << std::endl;
	}

	// sigIntHandler.sa_handler = my_handler;
	// sigemptyset(&sigIntHandler.sa_mask);
	// sigIntHandler.sa_flags = 0;
	// sigaction(SIGSYS, &sigIntHandler, NULL);

	while (true)
	{
		std::cout << "bond> ";
		std::cin >> msg;

		if (msg == "exit" || msg == "quit" || msg == "q")
		{
			list_encrypt(head, filename, (char *)sha256_key.c_str(),
						 (char *)sha256_iv.c_str(), master_key, master_iv);
			break;
		}
		else if (msg == "reset" || msg == "rst")
		{
			if (file_exists(filename))
			{
				list_encrypt(head, filename, NULL, NULL, NULL, NULL);
			}
			else
			{
				std::cout << "you do not have a key list" << std::endl;
			}
		}
		else if (msg == "delete-file")
		{
			if (file_exists(filename))
			{
				remove(filename.c_str());
				head = NULL;
				tail = NULL;
			}
		}
		else if (msg == "insert" || msg == "i")
		{
			// insert
			insert(&head, &tail);
		}
		else if (msg == "delete-pass" || msg == "dl")
		{
			if (file_exists(filename))
			{
				delete_pass(&head, &tail);
			}
			else
			{
				std::cout << "you do not have a key list" << std::endl;
			}
		}
		else if (msg == "list-all" || msg == "ls")
		{
			if (head != NULL)
			{
				list_all(head);
			}
			else
			{
				std::cout << "you do not have a key list" << std::endl;
			}
		}
		else if (msg == "list-from" || msg == "lsf")
		{
			if (head != NULL)
			{
				list_from(head);
			}
			else
			{
				std::cout << "you do not have a key list" << std::endl;
			}
		}
		else if (msg == "edit")
		{
			if (file_exists(filename))
			{
				edit(head);
			}
			else
			{
				std::cout << "you do not have a key list" << std::endl;
			}
		}
		else if (msg == "clear")
		{
			std::cout << "clearing" << std::flush;
			std::cout.flush();
		}
		else if (msg == "change-master")
		{
			std::cout << "1 sha256_key: " << sha256_key << std::endl;
			std::cout << "1 sha256_iv: " << sha256_iv << std::endl;
			change_master(sha256_key, sha256_iv);
			std::cout << "2 sha256_key: " << sha256_key << std::endl;
			std::cout << "2 sha256_iv: " << sha256_iv << std::endl;
		}
		else if (msg == "help" || msg == "h")
		{
			printf(
				"Usage: ./bond <command>, commands are:\n"
				"\tinsert\n"
				"\tdelete-pass\n"
				"\tdelete-file\n"
				"\tedit\n"
				"\tlist-all\n"
				"\tlist-from\n"
				"\tchange-master\n"
				"\treset\n");
		}
		else
		{
			printf("unknown command, type help for more information\n");
		}
	}
	exit(EXIT_SUCCESS);
}