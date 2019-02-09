#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <string>

int 
getch()
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

std::string getpass(bool show_asterisk=true)
{
	const char BACKSPACE=127;
	const char RETURN=10;

	std::string password;
	unsigned char ch=0;

	while((ch=getch())!=RETURN)
	{
		if(ch==BACKSPACE)
		{
			if(password.length()!=0)
			{
				if(show_asterisk)
				std::cout <<"\b \b";
				password.resize(password.length()-1);
			}
		}
		else
		{
			password+=ch;
			if(show_asterisk)
				std::cout <<'*';
		}
	}

	std::cout << std::endl;
	return password;
}



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
		master_key = getpass(true);

		std::cout	<< "repeat master-key: ";
		master_key_r = getpass(true);

		if(master_key == master_key_r)
			break;
		else
			std::cout	<< "\nYour passwords did not match, please retry\n\n";
	}

	std::cout	<< "\nCreating your binary database with all your encrypted passwords\n\n";

	return 0;
}