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


void
kappaprint(char *str)
{
	for(int i=0; i < 16; i++)
	{ std::cout << str[i] << ","; }
printf("\n");
}

void
list_encrypt(struct binary_reg *head, std::string filename,
	char *sha256_key, std::string master_key)
{
	// if file is empty return
	if(head == NULL) return;

	struct binary_reg *reader = head;
	struct binary_reg *cypher = new binary_reg();
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
	write_file.write(sha256_key, sizeof(char) * 128);

	while(reader != NULL)
	{
		// ENCRYPT
		cypher->identity_len = aes_encrypt((unsigned char *)reader->identity, strlen(reader->identity),
			key, iv, (unsigned char *)cypher->identity);
		cypher->username_len = aes_encrypt((unsigned char *)reader->username, strlen(reader->username),
			key, iv, (unsigned char *)cypher->username);
		cypher->password_len = aes_encrypt((unsigned char *)reader->password, strlen(reader->password),
			key, iv, (unsigned char *)cypher->password);

		// storing data to binary file encrypted
		printf("writing plain identity: |%-30s| username: |%-30s| password: |%-30s|\n",
			reader->identity, reader->username, reader->password);
		printf("writing crypt identity: |%-30s| username: |%-30s| password: |%-30s|\n",
			cypher->identity, cypher->username, cypher->password);

		write_file.write((char *) cypher, sizeof(struct binary_reg));

		reader = reader->next;
	}
	printf("%s/%d\n", __FILE__, __LINE__);

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

	struct binary_reg *entry = new binary_reg();
	struct binary_reg *pltext = new binary_reg();
	struct binary_reg *reader = new binary_reg();
	struct binary_reg *reader_prev = NULL;

	bool gone = false;

	*head = reader;
	*tail = reader;

	if(master_key.size() < KEY_SIZE)
	{
		std::string tmp = master_key;
		master_key.resize(master_key.size()+KEY_SIZE);
		master_key = tmp;
	}

	// a 256 bit key and a 128 bit IV 01234567890123456789012345678901
	unsigned char *key = (unsigned char *) master_key.c_str();
	unsigned char *iv = (unsigned char *) "0123456789012345";

	while(read_file->read((char *) entry, sizeof(struct binary_reg)))
	{
		// DECRYPT
		pltext->identity_len = aes_decrypt((unsigned char *)entry->identity, entry->identity_len,
			key, iv, (unsigned char *)pltext->identity);
		pltext->username_len = aes_decrypt((unsigned char *)entry->username, entry->username_len,
			key, iv, (unsigned char *)pltext->username);
		pltext->password_len = aes_decrypt((unsigned char *)entry->password, entry->password_len,
			key, iv, (unsigned char *)pltext->password);

		/* Add a NULL terminator. We are expecting printable text */
  		pltext->identity[pltext->identity_len] = '\0';
  		pltext->username[pltext->username_len] = '\0';
  		pltext->password[pltext->password_len] = '\0';

		strcpy(reader->identity, pltext->identity);
		strcpy(reader->username, pltext->username);
		strcpy(reader->password, pltext->password);

		reader->next = new binary_reg();
		reader->next->prev = reader;
		
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