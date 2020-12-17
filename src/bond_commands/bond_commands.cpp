#include "bond_commands.hpp"

#define BOLD_ON	 	"\e[1m"
#define BOLD_OFF	"\e[0m"

typedef struct binary_reg
{
	char identity[128];
	char username[128];
	char password[128];

	int identity_len;
	int username_len;
	int password_len;

	struct binary_reg *next;
	struct binary_reg *prev;
}
binary_reg;

bool
insert(struct binary_reg **head, struct binary_reg **tail)
{
	struct binary_reg *usrinp = (struct binary_reg *) malloc (sizeof(binary_reg));
	struct binary_reg *reader;

	std::cout << "\n" BOLD_ON "enter <new identity> "
		"<new username> <new password>: " BOLD_OFF;
	std::cin >> usrinp->identity >> usrinp->username >> usrinp->password;

	if((*head) == NULL) // first entry
	{
		(*head) = usrinp;

		(*head)->next = NULL;
		(*head)->prev = NULL;

		(*tail) = (*head);

		return true;
	}
	else if(strcmp((*head)->identity, usrinp->identity) >= 0) // head entry
	{
		usrinp->next = (*head); 
		(*head)->prev = usrinp; 
		(*head) = usrinp;

		return true;
	}
	else if(strcmp((*tail)->identity, usrinp->identity) < 0)  // tail entry
	{
		usrinp->prev = (*tail); 
		(*tail)->next = usrinp; 
		(*tail) = usrinp;

		return true;
	}
	else // middle entry
	{
		for(reader = (*head);
			reader->next != NULL &&
				strcmp(reader->identity, usrinp->identity) < 0;
			reader = reader->next);

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
		struct binary_reg *usrinp = (struct binary_reg *) malloc (sizeof(binary_reg));
		struct binary_reg *reader = (*head), *prev = NULL;

		std::cout	<< "\n" BOLD_ON "enter <identity> <username> of the "
					<<"password you want to delete: " BOLD_OFF ;
		std::cin >> usrinp->identity >> usrinp->username;

		if(strcmp((*head)->identity, usrinp->identity) == 0 &&
			strcmp((*head)->username, usrinp->username) == 0) // head deletion
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
		else if(strcmp((*tail)->identity, usrinp->identity) == 0
			&& strcmp((*tail)->username, usrinp->username) == 0) // tail deletion
		{
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
				if(strcmp(reader->identity, usrinp->identity) == 0 &&
					strcmp(reader->username, usrinp->username) == 0)
				{					
					reader->prev->next = reader->next;
					reader->next->prev = reader->prev;

					free(reader);
					return true;
				}
			}
		}

		std::cout << "Did not find entry" << std::endl;
	}

	return false;
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
			printf("identity: " BOLD_ON "%-25s" BOLD_OFF " username: "
				BOLD_ON "%-25s" BOLD_OFF " password: "
				BOLD_ON "%-25s" BOLD_OFF "\n",
				reader->identity, reader->username, reader->password);

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
		char msg[128];

		std::cout << "\n" BOLD_ON "enter <identity> you want "
			"to see credentials: " BOLD_OFF ;
		std::cin >> msg;

		struct binary_reg *reader = head;
		while(reader != NULL)
		{
			if(strcmp(reader->identity, msg) != 0)
			{
				printf("identity: " BOLD_ON "%-25s" BOLD_OFF " username: "
					BOLD_ON "%-25s" BOLD_OFF " password: "
					BOLD_ON "%-25s" BOLD_OFF "\n",
					reader->identity, reader->username, reader->password);
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
		char usr_msg2[128], usr_msg3[128], usr_msg4[128];

		std::cout << "\n" BOLD_ON "enter <identity> <username> "
			"<new password> you want to edit: " BOLD_OFF ;
		std::cin >> usr_msg2 >> usr_msg3 >> usr_msg4;

		struct binary_reg *reader = head;
		while(reader != NULL)
		{
			if(strcmp(reader->identity, usr_msg2) == 0 &&
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