#include "bond_func.hpp"

#define bold_on_2	 	"\e[1m"
#define bold_re_2		"\e[0m"

typedef struct binary_reg
{
	char identity[32];
	char username[32];
	char password[32];

	struct binary_reg *next;
	struct binary_reg *prev;
}
binary_reg;

void
change(struct binary_reg *a, struct binary_reg *b)
{
	struct binary_reg temp;

	strcpy (temp.identity, a->identity);
	strcpy (temp.username, a->username);
	strcpy (temp.password, a->password);

	strcpy (a->identity, b->identity);
	strcpy (a->username, b->username);
	strcpy (a->password, b->password);

	strcpy (b->identity, temp.identity);
	strcpy (b->username, temp.username);
	strcpy (b->password, temp.password);
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
			if (strcmp(current->identity, current->next->identity) > 0)
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
	std::string usr_msg2, usr_msg3, usr_msg4;

	std::cout << "\n" bold_on_2 "enter <new identity> <new username> <new password>: " bold_re_2 ;
			std::cin >> usr_msg2 >> usr_msg3 >> usr_msg4;

	if(head == NULL)
	{
		head = (binary_reg *) malloc (sizeof(struct binary_reg));

		strcpy(head->identity, usr_msg2.c_str());
		strcpy(head->username, usr_msg3.c_str());
		strcpy(head->password, usr_msg4.c_str());
		
		tail = head;
	}
	else
	{
		tail->next = (binary_reg *) malloc (sizeof(struct binary_reg));
		tail->next->next = NULL;

		strcpy(tail->next->identity, usr_msg2.c_str());
		strcpy(tail->next->username, usr_msg3.c_str());
		strcpy(tail->next->password, usr_msg4.c_str());
		
		tail = tail->next;
	}
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
			if((strcmp(reader->identity, usr_msg2.c_str()) +
				strcmp(reader->username, usr_msg3.c_str())) == 0)
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
				reader->identity ,reader->username ,reader->password
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
			if(!strcmp(reader->identity, usr_msg2.c_str()))
			{
				printf
				(
					"identity: %-20s username: %-20s password: %-20s\n",
					reader->identity ,reader->username ,reader->password
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
			if((strcmp(reader->identity, usr_msg2.c_str()) +
				strcmp(reader->username, usr_msg3.c_str())) == 0)
			{
				strcpy(reader->password, usr_msg4.c_str());
				break;
			}

			reader = reader->next;
		}
	}
}