#include "malcolm.h"

bool	is_hexa(char* str, int size)
{
	for (int i = 0; i < size; i++)
	{
		if (!((str[i] >= '0' && str[i] <= '9') || (str[i] >= 'a' && str[i] <= 'f') || (str[i] >= 'A' && str[i] <= 'F')))
			return false;
	}
	return true;
}

bool	is_character(char* str, int size, char c)
{
	for (int i = 0; i < size; i++)
	{
		if (str[i] != c)
			return false;
	}
	return true;
}