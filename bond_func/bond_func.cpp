#include "bond_func.hpp"

#define bold_on_2	 	"\e[1m"
#define bold_re_2		"\e[0m"

typedef struct binary_reg
{
	std::string identity;
	std::string username;
	std::string password;

	struct binary_reg *next;
	struct binary_reg *prev;
}
binary_reg;

void
change(struct binary_reg *a, struct binary_reg *b)
{
	struct binary_reg temp;

	temp.identity = a->identity;
	temp.username = a->username;
	temp.password = a->password;

	a->identity = b->identity;
	a->username = b->username;
	a->password = b->password;

	b->identity = temp.identity;
	b->username = temp.username;
	b->password = temp.password;
}

void
check(struct binary_reg *start)
{
	int swapped;
	struct binary_reg *current;
	struct binary_reg *next_one = NULL;

	if (current == NULL) return;

	do
	{
		swapped = 0;
		current = start;

		while (current->next != next_one)
		{
			if (current->identity > current->next->identity)
			{
				change(current, current->next);
				swapped = 1;
			}
			current = current->next;
		}
		next_one = current;
	}
	while (swapped);
}

void
sort(struct binary_reg *head)
{
	struct binary_reg *reader = head;
	check(reader);
}

bool
insert(struct binary_reg *head, struct binary_reg *tail)
{
	tail->next = new binary_reg();

	std::cout << "\n" bold_on_2 "enter <new identity> <new username> <new password>: " bold_re_2;
	std::cin >> tail->next->identity >> tail->next->username >> tail->next->password;

	tail->next->next = NULL;
	tail = tail->next;
}

bool
delete_pass(struct binary_reg *head, struct binary_reg *tail)
{
	std::string usr_msg2, usr_msg3;

	if(head == NULL)
	{
		std::cout << "your key list is empty" << std::endl;
	}
	else
	{
		std::cout	<< "\n" bold_on_2 "enter <identity> <username> of the "
				<<"password you want to delete: " bold_re_2 ;
		std::cin		>> usr_msg2 >> usr_msg3;

		struct binary_reg *reader = head;
		struct binary_reg *prev = NULL;

		while(reader != NULL)
		{
			if(reader->identity == usr_msg2 && reader->username == usr_msg3)
			{
				if(prev == NULL)
				{
					head = head->next;
				}
				else if(reader->next != NULL)
				{
					prev->next = reader->next;
				}
				else
				{
					prev->next = NULL;
					tail = prev;
				}

				free(reader);
				break;
			}

			prev = reader;
			reader = reader->next;
		}

		if(reader == NULL) std::cout << "Did not find entry" << std::endl;
	}
}

bool
list_all(struct binary_reg *head)
{
	if(head == NULL)
	{
		std::cout << "your key list is empty" << std::endl;
	}
	else
	{
		struct binary_reg *reader = head;
		while(reader != NULL)
		{
			printf(
				"identity: %-20s username: %-20s password: %-20s\n",
				reader->identity.c_str(), reader->username.c_str(), reader->password.c_str()
			);

			reader = reader->next;
		}
	}
}

bool
list_from(struct binary_reg *head)
{
	if(head == NULL)
	{
		std::cout << "your key list is empty" << std::endl;
	}
	else
	{
		std::string usr_msg2;

		std::cout << "\n" bold_on_2 "enter <identity> you want to see credentials: " bold_re_2 ;
		std::cin >> usr_msg2;

		struct binary_reg *reader = head;
		while(reader != NULL)
		{
			if(reader->identity != usr_msg2)
			{
				printf
				(
					"identity: %-20s username: %-20s password: %-20s\n",
					reader->identity.c_str(), reader->username.c_str(), reader->password.c_str()
				);
			}
			reader = reader->next;
		}
	}
}

bool
edit(struct binary_reg *head)
{
	if(head == NULL)
	{
		std::cout << "your key list is empty" << std::endl;
	}
	else
	{
		std::string usr_msg2, usr_msg3, usr_msg4;

		std::cout << "\n" bold_on_2 "enter <identity> <username> <new password> you want to edit: " bold_re_2 ;
		std::cin >> usr_msg2 >> usr_msg3 >> usr_msg4;

		struct binary_reg *reader = head;
		while(reader != NULL)
		{
			if(reader->identity == usr_msg2 &&
				reader->username == usr_msg3)
			{
				reader->password = usr_msg4;
				break;
			}

			reader = reader->next;
		}
	}
}