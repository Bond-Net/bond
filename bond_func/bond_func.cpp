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

bool
insert(struct binary_reg **head, struct binary_reg **tail)
{
	struct binary_reg *usrinp = new binary_reg();
	struct binary_reg *reader;

	std::cout << "\n" bold_on_2 "enter <new identity> <new username> <new password>: " bold_re_2;
	std::cin >> usrinp->identity >> usrinp->username >> usrinp->password;

	
	if((*head) == NULL) // first entry
	{
		(*head) = new binary_reg();

		(*head)->identity = usrinp->identity;
		(*head)->username = usrinp->username;
		(*head)->password = usrinp->password;

		(*head)->next = NULL;
		(*head)->prev = NULL;

		(*tail) = (*head);

		return true;
	}
	else if((*head)->identity >= usrinp->identity) // head entry
	{
		usrinp->next = (*head); 
		(*head)->prev = usrinp; 
		(*head) = usrinp;

		return true;
	}
	else if((*tail)->identity < usrinp->identity) // tail entry
	{
		usrinp->prev = (*tail); 
		(*tail)->next = usrinp; 
		(*tail) = usrinp;

		return true;
	}
	else // middle entry
	{	
		for(reader = (*head);
			reader->next != NULL && reader->identity < usrinp->identity;
			reader = reader->next);

		printf("reader->identity = %s\n", reader->identity.c_str());

		usrinp->next = reader;
		usrinp->prev = reader->prev;

		usrinp->next->prev = usrinp;
		usrinp->prev->next = usrinp;

		return true;
	}

	return false;
}

bool
delete_pass(struct binary_reg **head, struct binary_reg **tail)
{
	if((*head) == NULL)
	{
		std::cout << "your key list is empty" << std::endl;
	}
	else
	{
		struct binary_reg *usrinp = new binary_reg();
		struct binary_reg *reader = (*head), *prev = NULL;

		std::cout	<< "\n" bold_on_2 "enter <identity> <username> of the "
					<<"password you want to delete: " bold_re_2 ;
		std::cin >> usrinp->identity >> usrinp->username;

		if((*head)->identity == usrinp->identity
			&& (*head)->username == usrinp->username) // head deletion
		{
			if((*head) == (*tail))
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
		else if((*tail)->identity == usrinp->identity
			&& (*tail)->username == usrinp->username) // tail deletion
		{
			printf("tail->identity = %s\n", (*tail)->identity.c_str());	
			(*tail)->prev->next = NULL;
			(*tail) = (*tail)->prev;
			free((*tail)->next);

			return true;
		}
		else // middle deletion
		{	
			for(reader = (*head);
				reader->next != NULL;
				reader = reader->next)
			{
				if(reader->identity == usrinp->identity
					&& reader->username == usrinp->username)
				{
					printf("reader->identity = %s\n", reader->identity.c_str());
					
					reader->prev->next = reader->next;
					reader->next->prev = reader->prev;

					free(reader);
					return true;
				}
			}
		}

		std::cout << "Did not find entry" << std::endl;
		return false;
	}
}

bool
list_all(struct binary_reg *head)
{
	if(head == NULL)
	{
		std::cout << "your key list is empty" << std::endl;
		return false;
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

	return true;
}

bool
list_from(struct binary_reg *head)
{
	if(head == NULL)
	{
		std::cout << "your key list is empty" << std::endl;
		return false;
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

	return true;
}

bool
edit(struct binary_reg *head)
{
	if(head == NULL)
	{
		std::cout << "your key list is empty" << std::endl;
		return true;
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

	return true;
}