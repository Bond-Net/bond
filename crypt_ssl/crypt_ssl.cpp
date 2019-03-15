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
pri(std::string stringer)
{
	std::cout << "HASH: ";
	for(int i = 0; i < stringer.length(); i++)
	{
		std::cout << stringer[i] << "|";
	}
	std::cout << std::endl;
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
		// ENCRYPT DATA
		c_identity = aes_encrypt(key, iv, reader->identity);
		c_username = aes_encrypt(key, iv, reader->username);
		c_password = aes_encrypt(key, iv, reader->password);

		// COPY FROM STRING TO CHAR *
		strcpy(entry.identity, c_identity.c_str());
		strcpy(entry.username, c_username.c_str());
		strcpy(entry.password, c_password.c_str());

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

	//

	struct binary_reg *headk = NULL, *tailk = NULL;
	std::ifstream file_db(filename, std::ios::in | std::ios::binary);
	if (!file_db.is_open())
	{
		std::cout << "failed to open " << filename << '\n';
	}
	else
	{
		char sha256_key_c[96];
		file_db.read((char*) &sha256_key_c, sizeof(char) * 96);
		list_decrypt(&headk, &tailk, master_key, &file_db);
	}

	file_db.close();

	//
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
		// COPY FROM CHAR * TO STRING
		reader->identity = entry.identity;
		reader->username = entry.username;
		reader->password = entry.password;

		// DECRYPT DATA
		reader->identity = aes_decrypt(key, iv, reader->identity);
		reader->username = aes_decrypt(key, iv, reader->username);
		reader->password = aes_decrypt(key, iv, reader->password);

		std::cout << "DECR| reader->identity: |" << reader->identity << "|" << std::endl;
		std::cout << "DECR| reader->username: |" << reader->username << "|" << std::endl;
		std::cout << "DECR| reader->password: |" << reader->password << "|" << std::endl;

		reader->next = new binary_reg();
		reader_prev = reader;
		reader = reader->next;

		gone = true;
	}
	printf("[%s/%d]\n", __FILE__, __LINE__);

	// OPENSSL_cleanse(key, KEY_SIZE);
	// OPENSSL_cleanse(iv, BLOCK_SIZE);
	key = (unsigned char *)"";
	iv = (unsigned char *)"";

	*tail = reader_prev;

	if(!gone) *head = NULL;
	else reader_prev->next = NULL;

	printf("[%s/%d]\n", __FILE__, __LINE__);
}


// bool
// encrypt_list(std::string old_file, std::string new_file, std::string master_key)
// {
// 	int bytes_read, bytes_written;
// 	unsigned char indata[AES_BLOCK_SIZE];
// 	unsigned char outdata[AES_BLOCK_SIZE];

// 	/* ckey and ivec are the two 128-bits keys necesary to
// 	en- and recrypt your data.  Note that ckey can be
// 	192 or 256 bits as well */
// 	unsigned char *ckey = (unsigned char *) malloc (sizeof(master_key.c_str()));
// 	memcpy(ckey, master_key.c_str(), sizeof(master_key.c_str()));
// 	unsigned char ivec[] = "dontusethisinput";

// 	/* data structure that contains the key itself */
// 	AES_KEY key;

// 	 set the encryption key 
// 	AES_set_encrypt_key(ckey, 128, &key);

// 	/* set where on the 128 bit encrypted block to begin encryption*/
// 	int num = 0;

// 	FILE *old_f = file_open(old_file.c_str(), "r");
// 	FILE *new_f = file_open("new_keylist.dat", "w");

// 	while (1)
// 	{
// 		bytes_read = fread(indata, 1, AES_BLOCK_SIZE, old_f);

// 		AES_cfb128_encrypt(indata, outdata, bytes_read, &key, ivec, &num,
// 		AES_ENCRYPT);

// 		bytes_written = fwrite(outdata, 1, bytes_read, new_f);
// 		if (bytes_read < AES_BLOCK_SIZE)
// 		break;
// 	}

// 	fclose(old_f);
// 	fclose(new_f);

// 	remove(new_file.c_str());
// 	rename("new_keylist.dat", new_file.c_str());
// }