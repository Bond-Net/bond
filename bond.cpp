#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <stdio.h>
#include <ctype.h>

#include <fstream>		//File I/O
#include <iomanip>		//Convenient spacing
#include <cstring>		//Manipulate c-strings

#include "pass_gen/pass_gen.hpp"
#include "key_encryption/encrypt.h"
#include "pass_read.hpp"
#include "sha256.hpp"

int main(int argc, char** argv)
{
	std::string curr_key, key, msg, encrypted_msg, decrypted_msg;


	int length = 8; //Length of password to be generated
	int amountGen = 1; //Amount of passwords to generate
	bool smallAlpha = false; //Inclusion of lowercase letters
	bool largeAlpha = false; //Inclusion of uppercase letters
	bool symbols = false; //Whether or not to include symbols
	bool showMeta = false; //Whether or not to include metainfo

	//Create a pass_gen object using user-defined variables
	pass_gen new_pass(rand() % 20 + 12, 1, 1, 1, 1);
	//Output based on amount requested
	cout << new_pass.printPass(showMeta) << std::endl;

	std::cout	<< "This is bond I would like to help you manage your passwords."
				<< std::endl
				<< "(all your data will be encrypted before leaving your computer)"
				<< std::endl << std::endl
				<< "Please give me your master key (remember to keep it somewhere" 
				<< std::endl
				<< "written, so you never forget it)"
				<< std::endl << std::endl;

	while(true)
	{
		std::cout << "master key:\t";
		key = sha256(getpass());

		std::cout	<< "repeat master key:\t";
		if(sha256(getpass()) == key)
			break;
		else
			std::cout	<< std::endl
						<< "Your passwords did not match, please retry"
						<< std::endl << std::endl;
	}

	std::cout	<< std::endl << "Tell me your msg: "; 
	cin >> msg;

	while(true)
	{
		std::cout	<< std::endl << "Please enter the master key in order to encrypt the message: ";
		curr_key = getpass();
		if(sha256(curr_key) == key)
		{
			encrypted_msg = encrypt(msg, curr_key);
			curr_key = "";
			break;
		}
		else
		{
			std::cout	<< std::endl
						<< "Your passwords did not match, please retry"
						<< std::endl << std::endl;
		}
	}	

	std::cout 	<< "Your message is encrypted with your password, that we do not know."
				<< std::endl
				<< "The encrypted message is : " 
				<< encrypted_msg << std::endl;

	while(true)
	{
		std::cout	<< "Please enter the master key in order to decrypt the message : ";
		curr_key = getpass();
		if(sha256(curr_key) == key)
		{
			decrypted_msg = decrypt(encrypted_msg, curr_key);
			curr_key = "";
			break;
		}
		else
		{
			std::cout	<< std::endl
						<< "Your passwords did not match, please retry"
						<< std::endl << std::endl;
		}
	}

	std::cout	<< "The decrypted message is : " << decrypted_msg << std::endl;


	std::cout	<< std::endl
				<< "Creating your binary database with all your encrypted passwords" 
				<< std::endl << std::endl;

	return 0;
}