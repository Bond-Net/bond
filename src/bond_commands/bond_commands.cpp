#include "bond_commands.hpp"

// typedef struct binary_reg
// {
// 	char identity[128];
// 	char username[128];
// 	char password[128];

// 	int identity_len;
// 	int username_len;
// 	int password_len;

// 	struct binary_reg *next;
// 	struct binary_reg *prev;
// } binary_reg;

bool insert(struct binary_reg **head, struct binary_reg **tail)
{
	struct binary_reg *usrinp = (struct binary_reg *)malloc(sizeof(binary_reg));
	struct binary_reg *reader;

	std::cout << "enter <new identity> <new username> <new password>: ";
	std::cin >> usrinp->identity >> usrinp->username >> usrinp->password;

	if ((*head) == NULL) // first entry
	{
		(*head) = usrinp;

		(*head)->next = NULL;
		(*head)->prev = NULL;

		(*tail) = (*head);

		return true;
	}
	else if (strcmp((*head)->identity, usrinp->identity) >= 0) // head entry
	{
		usrinp->next = (*head);
		(*head)->prev = usrinp;
		(*head) = usrinp;

		return true;
	}
	else if (strcmp((*tail)->identity, usrinp->identity) < 0) // tail entry
	{
		usrinp->prev = (*tail);
		(*tail)->next = usrinp;
		(*tail) = usrinp;

		return true;
	}
	else // middle entry
	{
		for (reader = (*head);
			 reader->next != NULL &&
			 strcmp(reader->identity, usrinp->identity) < 0;
			 reader = reader->next)
			;

		usrinp->next = reader;
		usrinp->prev = reader->prev;

		usrinp->next->prev = usrinp;
		usrinp->prev->next = usrinp;

		return true;
	}

	return false;
}

bool delete_pass(struct binary_reg **head, struct binary_reg **tail)
{
	if ((*head) == NULL)
	{
		std::cout << "your key list is empty" << std::endl;
	}
	else
	{
		int user_input = -1;
		struct binary_reg *reader = (*head);

		std::cout << "enter <index> of the password you want to delete: ";
		std::cin >> user_input;

		for (int i = 1; reader != NULL; i++, reader = reader->next)
		{
			std::cout << i << " == " << user_input << std::endl;
			if (i == user_input)
			{
				std::string overwrite;
				std::cout << "\nbond> are you sure you want to delete entry: "
						  << reader->identity << ", " << reader->username << ", "
						  << reader->password << "? [y/n]" << std::endl;
				std::cin >> overwrite;
				if (overwrite == "y")
				{
					if ((*head) == (reader)) // head deletion
					{
						if ((*head) == (*tail))
						{
							free((*head));
							(*head) = (*tail) = NULL;
						}
						else
						{
							(*head) = (*head)->next;
							free((*head)->prev);
							(*head)->prev = NULL;

							return true;
						}
					}
					else if ((*tail) == (reader)) // tail deletion
					{
						(*tail)->prev->next = NULL;
						(*tail) = (*tail)->prev;
						free((*tail)->next);

						return true;
					}
					else // middle deletion
					{
						reader->prev->next = reader->next;
						reader->next->prev = reader->prev;
						free(reader);

						return true;
					}
				}
				break;
			}
		}

		std::cout << "no entry with given index" << std::endl;
	}

	return false;
}

bool list_all(struct binary_reg *head)
{
	if (head == NULL)
	{
		std::cout << "your key list is empty" << std::endl;
		return false;
	}
	else
	{
		printf("%-3s %-25s %-25s %-25s\n", "idx", "identity", "username", "password");
		int i = 0;
		for (struct binary_reg *reader = head; reader != NULL; i++, reader = reader->next)
		{
			printf("%-3d %-25s %-25s %-25s\n",
				   i, reader->identity, reader->username, reader->password);
		}
	}

	return true;
}

bool list_from(struct binary_reg *head)
{
	if (head == NULL)
	{
		std::cout << "your key list is empty" << std::endl;
		return false;
	}
	else
	{
		char msg[128];

		std::cout << "\nenter <identity> you want "
					 "to see credentials: ";
		std::cin >> msg;

		for (struct binary_reg *reader = head; reader != NULL; reader = reader->next)
		{
			if (strcmp(reader->identity, msg) == 0)
			{
				printf("identity: %-25s username: %-25s password: %-25s\n",
					   reader->identity, reader->username, reader->password);

				break;
			}
		}
	}

	return true;
}

bool edit(struct binary_reg *head)
{
	if (head == NULL)
	{
		std::cout << "your key list is empty" << std::endl;
		return true;
	}
	else
	{
		char usr_msg2[128], usr_msg3[128], usr_msg4[128];

		std::cout << "\nenter <identity> <username> "
					 "<new password> you want to edit: ";
		std::cin >> usr_msg2 >> usr_msg3 >> usr_msg4;

		struct binary_reg *reader = head;
		while (reader != NULL)
		{
			if (strcmp(reader->identity, usr_msg2) == 0 &&
				strcmp(reader->username, usr_msg3) == 0)
			{
				strcpy(reader->password, usr_msg4);
				break;
			}

			reader = reader->next;
		}
	}

	return true;
}

bool change_master(std::string &sha256_key, std::string &sha256_iv)
{
	return get_master(sha256_key, sha256_iv);
}