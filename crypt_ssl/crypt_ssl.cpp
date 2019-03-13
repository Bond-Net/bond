#include "crypt_ssl.hpp"
#include <openssl/sha.h>

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
list_encrypt(struct binary_reg *head, std::string name,
	std::string sha256_key, std::string master_key)
{
	if(head == NULL) return;

	std::string msg;
	struct binary_reg *reader = head;
	FILE *new_file_db = file_open("new_keylist.dat", "w");

	// every keyvault begins with the hash of the code
	fwrite(sha256_key.c_str(), sizeof(char) * 70, 1, new_file_db);

	// Load the necessary cipher
	EVP_add_cipher(EVP_aes_256_cbc());

	// plaintext, ciphertext, recovered text
	secure_string ptext = "Now is the time for all good men to come to the aide of their country";
	secure_string ctext, rtext;

	byte key[KEY_SIZE], iv[BLOCK_SIZE];
	gen_params(key, iv, "master_key");

	while(reader != NULL)
	{
		//decrypt
		// strcpy(reader->identity, encrypt(msg=reader->identity, master_key).c_str());
		// strcpy(reader->username, encrypt(msg=reader->username, master_key).c_str());
		// strcpy(reader->password, encrypt(msg=reader->password, master_key).c_str());

		fwrite(reader, sizeof(struct binary_reg), 1, new_file_db);
		reader = reader->next;
	}

	fclose(new_file_db);

	remove(name.c_str());
	rename("new_keylist.dat", name.c_str());
}

void
list_decrypt(struct binary_reg **head, struct binary_reg **tail,
	std::string master_key, FILE *file_db)
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
		// strcpy(row_from_db->identity, decrypt(msg=row_from_db->identity, master_key).c_str());
		// strcpy(row_from_db->username, decrypt(msg=row_from_db->username, master_key).c_str());
		// strcpy(row_from_db->password, decrypt(msg=row_from_db->password, master_key).c_str());
		
		row_from_db->next = (binary_reg *) malloc (sizeof(struct binary_reg));
		row_from_db_prev = row_from_db;
		row_from_db = row_from_db->next;

		not_gone_through = 0;
	}
	*tail = row_from_db_prev;
	
	if(not_gone_through) *head = NULL;
	else row_from_db_prev->next = NULL;
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