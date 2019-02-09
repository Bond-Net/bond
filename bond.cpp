#include <iostream>

#include "pass_read.hpp"

int main(int argc, char** argv)
{
	std::string master_key, master_key_r;

	std::cout	<< "This is bond I would like to help you manage your passwords.\n"
				<< "(all your data will be encrypted before leaving your computer)\n\n"
				<< "Please give me your master-key (remember to keep it somewhere \n"
				<< "written, so you never forget it)\n\n";

	while(true)
	{
		std::cout	<< "master-key: ";
		master_key = getpass();

		std::cout	<< "repeat master-key: ";
		master_key_r = getpass();

		if(master_key == master_key_r)
			break;
		else
			std::cout	<< "\nYour passwords did not match, please retry\n\n";
	}

	std::cout	<< "\nCreating your binary database with all your encrypted passwords\n\n";

	return 0;
}