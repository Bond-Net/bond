#include "bond_functions.hpp"

bool file_exists(const std::string &name)
{
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

FILE *file_open(std::string file, std::string type)
{
	FILE *file_db = fopen(file.c_str(), type.c_str());
	if (file_db == NULL)
	{
		printf("Error opening keylist.dat\n");
		exit(1);
	}
	return file_db;
}

bool arg_int(int argc, char **argv, std::string *filename, bool *verbose)
{
	struct option long_opt[] =
		{
			/* These options set a flag. */
			{"file", required_argument, 0, 'f'},
			{"pass", required_argument, 0, 'p'},
			{"verbose", no_argument, 0, 'V'},
			{"help", no_argument, 0, 'h'},
			{"version", no_argument, 0, 'v'},
			{0, 0, 0, 0}};

	/* getopt_long stores the option index here. */
	int option_index = 0, opt = 0;

	while ((opt = getopt_long(argc, argv, "fp:hvV", long_opt, &option_index)) != -1)
	{
		switch (opt)
		{
		case 'h':
			std::cout << "Flags:" << std::endl
					  << "======" << std::endl
					  << std::endl
					  << "flag name  "
					  << " | flag acronym "
					  << " | Argument     "
					  << " | Description                                                              " << std::endl
					  << "-file      "
					  << " | -f           "
					  << " | file         "
					  << " | key-list file you wish to open                                           " << std::endl
					  << "-pass      "
					  << " | -p           "
					  << " | number       "
					  << " | returns a strong password with given length                              " << std::endl
					  << "-verbose   "
					  << " | -V           "
					  << " | none         "
					  << " | executes as normal but with more information                             " << std::endl
					  << "-help      "
					  << " | -h           "
					  << " | none         "
					  << " | organizes *only* photos                                                  " << std::endl
					  << "-version   "
					  << " | -v           "
					  << " | none         "
					  << " | returns current builds version                                           " << std::endl
					  << std::endl;
			exit(EXIT_SUCCESS);

		case 'v':
			std::cout << "BOND Version: 0.8.0" << std::endl
					  << "A project by Keybraker (https://github.com/keybraker)" << std::endl
					  << "License - Released under the GNU LICENSE (https://www.gnu.org/philosophy/free-sw.html)" << std::endl
					  << "Copyrights © Keybraker 2020, All rights reserved" << std::endl;
			*verbose = true;
			exit(EXIT_SUCCESS);

		case 'V':
			*verbose = true;
			break;

		case 'f':
			*filename = optarg;
			break;

		case 0:
			break;

		case '?':
			std::cout << "there was an error with given flags" << std::endl;
			return EXIT_FAILURE;

		default:
			std::cout << "there was an error with given flags" << std::endl;
			return EXIT_FAILURE;
		}
	}

	return true;
}

int getch()
{
	int ch;
	struct termios t_old, t_new;

	tcgetattr(STDIN_FILENO, &t_old);
	t_new = t_old;
	t_new.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &t_new);

	ch = getchar();

	tcsetattr(STDIN_FILENO, TCSANOW, &t_old);
	return ch;
}

std::string get_pass(bool show_asterisk)
{
	const char BACKSPACE = 127;
	const char RETURN = 10;

	std::string password;
	unsigned char ch = 0;

	while ((ch = getch()) != RETURN)
	{
		if (ch == BACKSPACE)
		{
			if (password.length() != 0)
			{
				if (show_asterisk)
					std::cout << "\b \b";
				password.resize(password.length() - 1);
			}
		}
		else
		{
			password += ch;
			if (show_asterisk)
				std::cout << '*';
		}
	}
	std::cout << std::endl;

	return password;
}

bool get_master(std::string &sha256_key, std::string &sha256_iv)
{
	std::string master_key, master_iv;
	while (true)
	{
		std::cout << "enter master key:\t";
		sha256_key = sha256(get_pass(true));

		std::cout << "re-enter master key:\t";
		if (sha256((master_key = get_pass(true))) != sha256_key)
		{
			std::cout << "(your passwords did not match, please retry)"
					  << std::endl
					  << std::endl;

			continue;
		}

		break;
	}

	while (true)
	{
		std::cout << "enter master iv:\t";
		sha256_iv = sha256(master_iv = get_pass(true));
		if (sha256_key == sha256_iv)
		{
			std::cout << "(your iv is the same with key, please retry)"
					  << std::endl
					  << std::endl;

			continue;
		}

		std::cout << "re-enter master iv:\t";
		if (sha256((master_iv = get_pass(true))) != sha256_iv)
		{
			std::cout << "(your ivs did not match, please retry)"
					  << std::endl
					  << std::endl;

			continue;
		}

		break;
	}

	return true;
}