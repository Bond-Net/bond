#ifndef DFS_SSL_CRYPTER_H
#define DFS_SSL_CRYPTER_H

#include <iostream>
#include <string>
#include <memory>
#include <limits>
#include <stdexcept>

#include <openssl/evp.h>
#include <openssl/rand.h>

static const unsigned int KEY_SIZE = 32;
static const unsigned int BLOCK_SIZE = 16;

typedef unsigned char byte;
using EVP_CIPHER_CTX_free_ptr = std::unique_ptr<EVP_CIPHER_CTX, decltype(&::EVP_CIPHER_CTX_free)>;

std::string
aes_encrypt(const byte key[KEY_SIZE], const byte iv[BLOCK_SIZE],
	const std::string& ptext);

std::string
aes_decrypt(const byte key[KEY_SIZE], const byte iv[BLOCK_SIZE], 
	const std::string& ctext);

#endif // DFS_SSL_CRYPTER_H
