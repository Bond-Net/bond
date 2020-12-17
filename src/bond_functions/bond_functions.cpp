#include "bond_functions.hpp"

bool file_exists(const std::string &name)
{
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

FILE *
file_open(std::string file, std::string type)
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
			{"help", no_argument, 0, 'h'},
			{"verb", no_argument, 0, 'v'},
			{"file", required_argument, 0, 'f'},
			{0, 0, 0, 0}};
	/* getopt_long stores the option index here. */
	int option_index = 0, opt = 0;

	while ((opt = getopt_long(argc, argv, "hvf:c:", long_opt, &option_index)) != -1)
	{
		switch (opt)
		{
		case 'h':
			exit(EXIT_SUCCESS);

		case 'v':
			*verbose = true;
			break;

		case 'f':
			*filename = optarg;
			break;

		default:
			break;
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