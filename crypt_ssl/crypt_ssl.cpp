#include "crypt_ssl.hpp"
#include <openssl/sha.h>
#include <fstream>

std::string
sha256(const std::string str)
{
	unsigned char hash[SHA256_DIGEST_LENGTH];
	SHA256_CTX sha256;
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, str.c_str(), str.size());
	SHA256_Final(hash, &sha256);
	std::stringstream ss;
	for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
	{
		ss << std::hex << std::setw(2) << std::setfill('0') << (int) hash[i];
	}
	return ss.str();
}

struct file_io
{
	char identity[96];
	char username[96];
	char password[96];
};

void
list_encrypt(struct binary_reg *head, std::string filename,
	std::string sha256_key, std::string master_key)
{
	// if file is empty return
	if(head == NULL) return;

	struct binary_reg *reader = head;
	struct file_io entry;

	std::string c_identity;
	std::string c_username;
	std::string c_password;

	// Load the necessary cipher
	EVP_add_cipher(EVP_aes_256_cbc());

	if(master_key.size() < KEY_SIZE)
	{
		std::string tmp = master_key;
		master_key.resize(master_key.size()+KEY_SIZE);
		master_key = tmp;
	}

	// a 256 bit key and a 128 bit IV 01234567890123456789012345678901
	unsigned char *key = (unsigned char *) master_key.c_str();
	unsigned char *iv = (unsigned char *) "0123456789012345";

	// creating new keyvault which begins with the hash of the code
	std::ofstream write_file("new_keylist.dat", std::ios::out | std::ios::binary);
	write_file.write(sha256_key.c_str(), sizeof(char) * 96);

	while(reader != NULL)
	{
		// ENCRYPT
		strcpy(entry.identity, aes_encrypt(key, iv, reader->identity).c_str());
		strcpy(entry.username, aes_encrypt(key, iv, reader->username).c_str());
		strcpy(entry.password, aes_encrypt(key, iv, reader->password).c_str());

		// printf("ENCR| entry.identity: |%s|\n", entry.identity);
		// printf("ENCR| entry.username: |%s|\n", entry.username);
		// printf("ENCR| entry.password: |%s|\n", entry.password);

		// storing data to binary file encrypted
		write_file.write((char *) &entry, sizeof(struct file_io));

		reader = reader->next;
	}

	// OPENSSL_cleanse(key, KEY_SIZE);
	// OPENSSL_cleanse(iv, BLOCK_SIZE);

	key = (unsigned char *)"";
	iv = (unsigned char *)"";

	write_file.close();

	remove(filename.c_str());
	rename("new_keylist.dat", filename.c_str());
}

void
list_decrypt(struct binary_reg **head, struct binary_reg **tail,
	std::string master_key, std::ifstream *read_file)
{
	// Load the necessary cipher
	EVP_add_cipher(EVP_aes_256_cbc());

	struct file_io entry;
	struct binary_reg *reader = new binary_reg(), *reader_prev = NULL;

	bool gone = false;

	*head = reader;
	*tail = reader;

	if(master_key.size() < KEY_SIZE)
	{
		std::string tmp = master_key;
		master_key.resize(master_key.size()+KEY_SIZE);
		master_key = tmp;
	}

	// a 256 bit key and a 128 bit IV
	unsigned char *key = (unsigned char *) master_key.c_str();
	unsigned char *iv = (unsigned char *) "0123456789012345";

	while(read_file->read((char *) &entry, sizeof(struct file_io)))
	{
		// DECRYPT
		reader->identity = aes_decrypt(key, iv, std::string(entry.identity));
		reader->username = aes_decrypt(key, iv, std::string(entry.username));
		reader->password = aes_decrypt(key, iv, std::string(entry.password));

		// std::cout << "DECR| reader->identity: |" << reader->identity << "|" << std::endl;
		// std::cout << "DECR| reader->username: |" << reader->username << "|" << std::endl;
		// std::cout << "DECR| reader->password: |" << reader->password << "|" << std::endl;

		reader->next = new binary_reg();
		reader_prev = reader;
		reader = reader->next;

		gone = true;
	}

	// OPENSSL_cleanse(key, KEY_SIZE);
	// OPENSSL_cleanse(iv, BLOCK_SIZE);

	key = (unsigned char *)"";
	iv = (unsigned char *)"";

	*tail = reader_prev;

	if(!gone) *head = NULL;
	else reader_prev->next = NULL;

	printf("[%s/%d]\n", __FILE__, __LINE__);
}