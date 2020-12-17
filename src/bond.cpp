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

#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/aes.h>
#include <openssl/sha.h>

#include "bond_commands/bond_commands.hpp"
#include "bond_functions/bond_functions.hpp"
#include "cryptography/crypt_ssl.hpp"

int main(int argc, char *argv[])
{
	std::string msg, filename, master_key, master_iv, sha256_key, sha256_iv;

	char sha256_key_c[128], sha256_iv_c[128];
	struct binary_reg *head = NULL, *tail = NULL;
	bool verbose = false;

	arg_int(argc, argv, &filename, &verbose);

	if (file_exists(filename))
	{
		std::cout << "Enter the master key in order to open the vault: "
				  << std::endl
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
					std::cout << bold_on << "incorrect password, try again"
							  << bold_re << std::endl
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
					std::cout << bold_on << "incorrect iv, try again"
							  << bold_re << std::endl
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
	else
	{
		std::cout << "It seems like you have no key list." << std::endl
				  << "Enter your new master key (remember to write it down)"
				  << std::endl;

		while (true)
		{
			std::cout << "> enter master key:\t";
			sha256_key = sha256(get_pass(true));

			std::cout << "> re-enter master key:\t";
			if (sha256((master_key = get_pass(true))) != sha256_key)
			{
				std::cout << bold_on << "(your passwords did not match, please retry)"
						  << bold_re << std::endl
						  << std::endl;
				continue;
			}

			break;
		}

		while (true)
		{
			std::cout << "> enter master iv:\t";
			sha256_iv = sha256(master_iv = get_pass(true));
			if (sha256_key == sha256_iv)
			{
				std::cout << bold_on << "(your iv is the same with key, please retry)"
						  << bold_re << std::endl
						  << std::endl;
				continue;
			}

			std::cout << "> re-enter master iv:\t";
			if (sha256((master_iv = get_pass(true))) != sha256_iv)
			{
				std::cout << bold_on << "(your ivs did not match, please retry)"
						  << bold_re << std::endl
						  << std::endl;
				continue;
			}

			break;
		}
	}

	while (true)
	{
		std::cout << "\n" bold_on "command: " bold_re;
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
			if (file_exists(filename))
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
				"\treset\n");
		}
	}

	exit(EXIT_SUCCESS);
}