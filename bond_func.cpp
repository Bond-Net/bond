#include "bond_func.hpp"

inline bool file_exists(const std::string& name)
{
	struct stat buffer;
	return (stat (name.c_str(), &buffer) == 0);
}

void change(struct binary_reg *a, struct binary_reg *b)
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

void check(struct binary_reg *start)
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

FILE *
file_open(std::string name)
{
	FILE *file_db = fopen(name.c_str(), "w");
	if(file_db == NULL)
	{
		printf("Error opening keylist.dat\n");
		exit(1);
	}
	return file_db;
}

void
encrypt(struct binary_reg *head, char *name, char *sha256_key_c, std::string master_key)
{
	std::string
		msg;
	struct binary_reg
		*reader = head;
	FILE
		*new_file_db = file_open("new_keylist.dat");

	fwrite(sha256_key_c, sizeof(char) * 70, 1, new_file_db);

	while(reader != NULL)
	{
		//decrypt
		strcpy(reader->identity, encrypt(msg=reader->identity, master_key).c_str());
		strcpy(reader->username, encrypt(msg=reader->username, master_key).c_str());
		strcpy(reader->password, encrypt(msg=reader->password, master_key).c_str());

		fwrite(reader, sizeof(struct binary_reg), 1, new_file_db);
		reader = reader->next;
	}

	fclose(new_file_db);

	remove(name);
	rename("new_keylist.dat", name);
}

struct binary_reg *
decrypt(std::string master_key, FILE *file_db)
{
	std::string
		msg;
	struct binary_reg
		*head = NULL,
		*row_from_db_prev = NULL,
		*row_from_db = NULL;
	int
		not_gone_through = 1;

	row_from_db = (binary_reg *) malloc (sizeof(struct binary_reg));
	head = row_from_db;

	while (fread(row_from_db, sizeof(struct binary_reg), 1, file_db) != 0)
	{
		//decrypt
		strcpy(row_from_db->identity, decrypt(msg=row_from_db->identity, master_key).c_str());
		strcpy(row_from_db->username, decrypt(msg=row_from_db->username, master_key).c_str());
		strcpy(row_from_db->password, decrypt(msg=row_from_db->password, master_key).c_str());

		row_from_db->next = (binary_reg *) malloc (sizeof(struct binary_reg));
		row_from_db_prev = row_from_db;
		row_from_db = row_from_db->next;

		not_gone_through = 0;
	}

	if(not_gone_through) head = NULL;
	else row_from_db_prev->next = NULL;

	return head;
}