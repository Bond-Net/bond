#include "bond_help.hpp"

bool
file_exists(const std::string& name)
{
	struct stat buffer;
	return (stat (name.c_str(), &buffer) == 0);
}

FILE *
file_open(std::string file, std::string type)
{
	FILE *file_db = fopen(file.c_str(), type.c_str());
	if(file_db == NULL)
	{
		printf("Error opening keylist.dat\n");
		exit(1);
	}
	return file_db;
}

bool
arg_int(int argc, char** argv, std::string *filename, bool *verbose)
{
	struct option long_opt[] =
	{
		/* These options set a flag. */
		{"help",	no_argument,     	0,	'h'},
		{"verb",	no_argument,		0,	'v'},
		{"file",	required_argument,	0,	'f'},
		{0,			0, 					0,	0}
	};
	/* getopt_long stores the option index here. */
	int option_index = 0, opt = 0;

	while((opt = getopt_long (argc, argv, "hvs:c:", long_opt, &option_index)) != -1)
	{
		switch (opt)
		{
			case 'h':
				prnt_inf("HELP");
				exit(EXIT_SUCCESS);

			case 'v':
				prnt_inf("VERBOSE");
				*verbose = true;
				break;

			case 'f':
				prnt_inf("file is read");
				*filename = optarg;
				break;

			default:
				break;	
		}
	}

	return true;
}