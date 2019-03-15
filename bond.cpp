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

// typedef unsigned char uchar;

// // Fill in actual key here
// static const uchar ckey[] = "893428042894";

// void
// Encrypt32(uchar *in, uchar *out)
// {
// 	////////////////////////////////////
// 	std::string ctext;

// 	EVP_CIPHER_CTX_free_ptr ctx(EVP_CIPHER_CTX_new(), ::EVP_CIPHER_CTX_free);
// 	int rc = EVP_EncryptInit_ex(ctx.get(), EVP_aes_256_cbc(), NULL, ckey, ckey);
// 	if (rc != 1) throw std::runtime_error("EVP_EncryptInit_ex failed");

// 	// Recovered text expands upto BLOCK_SIZE
// 	ctext.resize(2 * AES_BLOCK_SIZE+BLOCK_SIZE);
// 	int out_len1 = (int) ctext.size();

// 	rc = EVP_EncryptUpdate(ctx.get(), (byte*)&ctext[0], &out_len1, 
// 		(const byte*)&in[0], (int)2 * AES_BLOCK_SIZE);
// 	if (rc != 1) throw std::runtime_error("EVP_EncryptUpdate failed");

// 	int out_len2 = (int)ctext.size() - out_len1;
// 	rc = EVP_EncryptFinal_ex(ctx.get(), (byte*)&ctext[0]+out_len1, &out_len2);
// 	if (rc != 1) throw std::runtime_error("EVP_EncryptFinal_ex failed");

// 	// Set cipher text size now that we know it
// 	ctext.resize(out_len1 + out_len2);

// 	std::cout << "ctext: " << ctext << std::endl;

// 	/////////////////////////////////////

//     AES_KEY encryptKey;
//     AES_set_encrypt_key(ckey, 256, &encryptKey);

//     AES_ecb_encrypt(in, out, &encryptKey, AES_ENCRYPT);
//     AES_ecb_encrypt(&in[AES_BLOCK_SIZE], &out[AES_BLOCK_SIZE], &encryptKey, AES_ENCRYPT);
// }

// void 
// Decrypt32(uchar *in, uchar *out)
// {
// 	////////////////////////////////////
// 	std::string rtext;

// 	EVP_CIPHER_CTX_free_ptr ctx(EVP_CIPHER_CTX_new(), ::EVP_CIPHER_CTX_free);
// 	int rc = EVP_DecryptInit_ex(ctx.get(), EVP_aes_256_cbc(), NULL, ckey, ckey);
// 	if (rc != 1) throw std::runtime_error("EVP_DecryptInit_ex failed");

// 	// Recovered text contracts upto BLOCK_SIZE
// 	rtext.resize(2 * AES_BLOCK_SIZE);
// 	int out_len1 = (int)rtext.size();

// 	rc = EVP_DecryptUpdate(ctx.get(), (byte*)&rtext[0], &out_len1, 
// 		(const byte*)&in[0], (int)2 * AES_BLOCK_SIZE);
// 	if (rc != 1) throw std::runtime_error("EVP_DecryptUpdate failed");

// 	int out_len2 = (int)rtext.size() - out_len1;
// 	rc = EVP_DecryptFinal_ex(ctx.get(), (byte*)&rtext[0]+out_len1, &out_len2);
// 	if (rc != 1) throw std::runtime_error("EVP_DecryptFinal_ex failed");

// 	// Set recovered text size now that we know it
// 	rtext.resize(out_len1 + out_len2);

// 	std::cout << "rtext: " << rtext << std::endl;

// 	////////////////////////////////////

//     AES_KEY decryptKey;
// 	AES_set_decrypt_key(ckey, 256, &decryptKey);

//     AES_ecb_encrypt(in, out, &decryptKey, AES_DECRYPT);
//     AES_ecb_encrypt(&in[AES_BLOCK_SIZE], &out[AES_BLOCK_SIZE], &decryptKey, AES_DECRYPT);
// }

// // Test client
// int main(int argc, char **argv) 
// {
//     // Sample input
//     char texter[2 * AES_BLOCK_SIZE] = {'a', 'b', 'c', 'd'};

//     uchar txt[2 * AES_BLOCK_SIZE] = "";
//     uchar enc[2 * AES_BLOCK_SIZE] = "";
//     uchar dec[2 * AES_BLOCK_SIZE] = "";

//     memcpy(txt, texter, 2 * AES_BLOCK_SIZE);

//     printf("txt:\t%s\n", txt);
//     printf("enc:\t%s\n", enc);
//     printf("dec:\t%s\n", dec);
//     printf("-----------------------------\n\n");
    
//     Encrypt32(txt, enc);
    
//     printf("txt:\t%s\n", txt);
//     printf("enc:\t%s\n", enc);
//     printf("dec:\t%s\n", dec);
//     printf("-----------------------------\n\n");

// 	Decrypt32(enc, dec);

//     printf("txt:\t%s\n", txt);
//     printf("enc:\t%s\n", enc);
//     printf("dec:\t%s\n", dec);
//     printf("-----------------------------\n\n");

//     return 0;
// }

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

	return 0;
}

// #include <iostream>
// #include <string>
// #include <memory>
// #include <limits>
// #include <stdexcept>

// #include <openssl/evp.h>
// #include <openssl/rand.h>

// static const unsigned int KEY_SIZE = 32;
// static const unsigned int BLOCK_SIZE = 16;

// unsigned char key_in[KEY_SIZE] = {'a', '4', 'a', 'a', 'a', 'a', 'a', 'a', 'a',
// 	'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a',
// 	'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a'};
// unsigned char iv_in[BLOCK_SIZE] = {'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a',
// 	'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a'};

// typedef unsigned char byte;
// using EVP_CIPHER_CTX_free_ptr = std::unique_ptr<EVP_CIPHER_CTX, decltype(&::EVP_CIPHER_CTX_free)>;

// std::string
// aes_encrypt(const byte key[KEY_SIZE], const byte iv[BLOCK_SIZE],
// 	const std::string& ptext)
// {
// 	std::string ctext;

// 	EVP_CIPHER_CTX_free_ptr ctx(EVP_CIPHER_CTX_new(), ::EVP_CIPHER_CTX_free);
// 	int rc = EVP_EncryptInit_ex(ctx.get(), EVP_aes_256_cbc(), NULL, key_in, iv_in);
// 	if (rc != 1)
// 	throw std::runtime_error("EVP_EncryptInit_ex failed");

// 	// Recovered text expands upto BLOCK_SIZE
// 	ctext.resize(ptext.size()+BLOCK_SIZE);
// 	int out_len1 = (int)ctext.size();

// 	rc = EVP_EncryptUpdate(ctx.get(), (byte*)&ctext[0], &out_len1, 
// 	(const byte*)&ptext[0], (int)ptext.size());
// 	if (rc != 1)
// 	throw std::runtime_error("EVP_EncryptUpdate failed");

// 	int out_len2 = (int)ctext.size() - out_len1;
// 	rc = EVP_EncryptFinal_ex(ctx.get(), (byte*)&ctext[0]+out_len1, &out_len2);
// 	if (rc != 1)
// 	throw std::runtime_error("EVP_EncryptFinal_ex failed");

// 	// Set cipher text size now that we know it
// 	ctext.resize(out_len1 + out_len2);

// 	return ctext;
// }

// std::string
// aes_decrypt(const byte key[KEY_SIZE], const byte iv[BLOCK_SIZE], 
// 	const std::string& ctext)
// {
// 	std::string rtext;

// 	EVP_CIPHER_CTX_free_ptr ctx(EVP_CIPHER_CTX_new(), ::EVP_CIPHER_CTX_free);
// 	int rc = EVP_DecryptInit_ex(ctx.get(), EVP_aes_256_cbc(), NULL, key_in, iv_in);
// 	if (rc != 1)
// 	throw std::runtime_error("EVP_DecryptInit_ex failed");

// 	// Recovered text contracts upto BLOCK_SIZE
// 	rtext.resize(ctext.size());
// 	int out_len1 = (int)rtext.size();

// 	rc = EVP_DecryptUpdate(ctx.get(), (byte*)&rtext[0], &out_len1, 
// 	(const byte*)&ctext[0], (int)ctext.size());
// 	if (rc != 1)
// 	throw std::runtime_error("EVP_DecryptUpdate failed");

// 	int out_len2 = (int)rtext.size() - out_len1;
// 	rc = EVP_DecryptFinal_ex(ctx.get(), (byte*)&rtext[0]+out_len1, &out_len2);
// 	if (rc != 1)
// 	throw std::runtime_error("EVP_DecryptFinal_ex failed");

// 	// Set recovered text size now that we know it
// 	rtext.resize(out_len1 + out_len2);

// 	return rtext;
// }

// // g++ -Wall -std=c++11 evp-encrypt.cxx -o evp-encrypt.exe -lcrypto

// // g++ -Wall -std=c++11 bond.cpp -o evp-encrypt.exe -lcrypto
// // ./evp-encrypt.exe

// struct file_io
// {
// 	char identity[96];
// 	char username[96];
// 	char password[96];
// };

// int
// main(int argc, char* argv[])
// {
// 	// Load the necessary cipher
// 	EVP_add_cipher(EVP_aes_256_cbc());

// 	// plaintext, ciphertext, recovered text
// 	std::string ptext = "Now is the time for all good men to come to the aid of their country";
// 	std::string ctext, rtext;

// 	byte key[KEY_SIZE], iv[BLOCK_SIZE];

// 	ctext = aes_encrypt(key, iv, ptext);

// 	std::cout << "Original message:\t" << ptext << std::endl;
// 	std::cout << "Cyphertext message:\t" << ctext << std::endl;
// 	std::cout << "Recovered message:\t" << rtext << std::endl << std::endl;

// 	struct file_io putin;

// 	strcpy(putin.identity, ctext.c_str());
// 	strcpy(putin.username, ctext.c_str());
// 	strcpy(putin.password, ctext.c_str());

// 	std::ofstream write_file("new_keylist.dat", std::ios::out | std::ios::binary);
// 	write_file.write((char *) &putin, sizeof(struct file_io));
// 	write_file.close();

// 	////////

// 	struct file_io entry;

// 	std::ifstream read_file("new_keylist.dat", std::ios::in | std::ios::binary);
// 	read_file.is_open();
// 	read_file.read((char *) &entry, sizeof(struct file_io));
// 	read_file.close();

// 	std::string ctext2 = entry.identity;
// 	std::string ctext3 = entry.username;
// 	std::string ctext4 = entry.password;

// 	rtext = aes_decrypt(key, iv, ctext2);

// 	std::cout << "Original message:\t" << ptext << std::endl;
// 	std::cout << "Cyphertext message:\t" << ctext2 << std::endl;
// 	std::cout << "Recovered message:\t" << rtext << std::endl << std::endl;

// 	rtext = aes_decrypt(key, iv, ctext3);

// 	std::cout << "Original message:\t" << ptext << std::endl;
// 	std::cout << "Cyphertext message:\t" << ctext3 << std::endl;
// 	std::cout << "Recovered message:\t" << rtext << std::endl << std::endl;

// 	rtext = aes_decrypt(key, iv, ctext4);

// 	std::cout << "Original message:\t" << ptext << std::endl;
// 	std::cout << "Cyphertext message:\t" << ctext4 << std::endl;
// 	std::cout << "Recovered message:\t" << rtext << std::endl << std::endl;

// 	OPENSSL_cleanse(key, KEY_SIZE);
// 	OPENSSL_cleanse(iv, BLOCK_SIZE);

// 	return 0;
// }