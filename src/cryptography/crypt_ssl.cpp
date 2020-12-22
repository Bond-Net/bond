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
	for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
	{
		ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
	}
	return ss.str();
}

char *
sha256(char *str)
{
	unsigned char hash[SHA256_DIGEST_LENGTH];
	char *str_hash = (char *)malloc(sizeof(char) * 65);

	SHA256_CTX sha256;
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, str, strlen(str));
	SHA256_Final(hash, &sha256);

	for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
		sprintf(str_hash + (i * 2), "%02x", hash[i]);

	str_hash[64] = 0;

	return str_hash;
}

void list_encrypt(struct binary_reg *head, std::string filename, char *sha256_key,
				  char *sha256_iv, std::string master_key, std::string master_iv)
{
	if (file_exists(filename) != true)
	{
		while (filename.size() == 0 || file_exists(filename) == true)
		{
			std::cout << "bond> enter key-list name (without .dat)" << std::endl;
			std::cin >> filename;
			filename += ".dat";

			if (file_exists(filename))
			{
				std::string overwrite;
				std::cout << "bond> file already exists, do you want to overwrite it? [y/n]" << std::endl;
				std::cin >> overwrite;
				if (overwrite == "y")
					break;
			}
		}
	}

	// creating new keyvault which begins with the hash of the code
	std::ofstream write_file(filename, std::ios::out | std::ios::binary);

	write_file.write(sha256_key, sizeof(char) * 128);
	write_file.write(sha256_iv, sizeof(char) * 128);

	if (head != NULL)
	{
		struct binary_reg *reader = head;
		struct binary_reg *cypher = new binary_reg();

		// Load the necessary cipher
		EVP_add_cipher(EVP_aes_256_cbc());

		if (master_key.size() < KEY_SIZE)
		{
			std::string tmp = master_key;
			master_key.resize(master_key.size() + KEY_SIZE);
			master_key = tmp;
		}

		if (master_iv.size() < BLOCK_SIZE)
		{
			std::string tmp = master_iv;
			master_iv.resize(master_iv.size() + BLOCK_SIZE);
			master_iv = tmp;
		}

		// a 256 bit key and a 128 bit IV 01234567890123456789012345678901-0123456789012345
		unsigned char *key = (unsigned char *)master_key.c_str();
		unsigned char *iv = (unsigned char *)master_iv.c_str();

		while (reader != NULL)
		{
			// ENCRYPT
			cypher->identity_len = aes_encrypt((unsigned char *)reader->identity,
											   strlen(reader->identity), key, iv, (unsigned char *)cypher->identity);
			cypher->username_len = aes_encrypt((unsigned char *)reader->username,
											   strlen(reader->username), key, iv, (unsigned char *)cypher->username);
			cypher->password_len = aes_encrypt((unsigned char *)reader->password,
											   strlen(reader->password), key, iv, (unsigned char *)cypher->password);

			write_file.write((char *)cypher, sizeof(struct binary_reg));

			reader = reader->next;
		}

		key = (unsigned char *)"";
		iv = (unsigned char *)"";
	}
	write_file.close();
}

void list_decrypt(struct binary_reg **head, struct binary_reg **tail, std::string master_key,
				  std::string master_iv, std::ifstream *read_file)
{
	// Load the necessary cipher
	EVP_add_cipher(EVP_aes_256_cbc());

	struct binary_reg *entry = new binary_reg();
	struct binary_reg *reader = new binary_reg();
	struct binary_reg *reader_prev = NULL;

	bool gone = false;

	*head = reader;
	*tail = reader;

	if (master_key.size() < KEY_SIZE)
	{
		std::string tmp = master_key;
		master_key.resize(master_key.size() + KEY_SIZE);
		master_key = tmp;
	}

	if (master_iv.size() < KEY_SIZE)
	{
		std::string tmp = master_iv;
		master_iv.resize(master_iv.size() + KEY_SIZE);
		master_iv = tmp;
	}

	// a 256 bit key and a 128 bit IV 01234567890123456789012345678901-0123456789012345
	unsigned char *key = (unsigned char *)master_key.c_str();
	unsigned char *iv = (unsigned char *)master_iv.c_str();

	while (read_file->read((char *)entry, sizeof(struct binary_reg)))
	{
		// DECRYPT
		reader->identity_len = aes_decrypt((unsigned char *)entry->identity,
										   entry->identity_len, key, iv, (unsigned char *)reader->identity);
		reader->username_len = aes_decrypt((unsigned char *)entry->username,
										   entry->username_len, key, iv, (unsigned char *)reader->username);
		reader->password_len = aes_decrypt((unsigned char *)entry->password,
										   entry->password_len, key, iv, (unsigned char *)reader->password);

		/* Add a NULL terminator. We are expecting printable text */
		reader->identity[reader->identity_len] = '\0';
		reader->username[reader->username_len] = '\0';
		reader->password[reader->password_len] = '\0';

		reader->next = new binary_reg();
		reader->next->prev = reader;

		reader_prev = reader;
		reader = reader->next;

		gone = true;
	}

	key = (unsigned char *)"";
	iv = (unsigned char *)"";

	*tail = reader_prev;

	if (!gone)
		*head = NULL;
	else
		reader_prev->next = NULL;
}