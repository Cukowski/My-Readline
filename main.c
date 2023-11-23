#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

// two global variables (one for your "storage" and one for READLINE_READ_SIZE)
int READLINE_READ_SIZE = 512;
char * storage;

// will init (or reinitialize) the storage
void init_my_readline()
{
	storage = NULL;
}

/*
    I HAVE RECEIVED A BAD PEER REVIEW BECAUSE OF I USED STRCPY() FUNCTION WHICH WASN'T IN THE AUTHORIZED FUNCTIONS LIST
    I WILL NOT MAKE THE SAME MISTAKE AGAIN
    HERE IS A TON OF HAND MADE FRESH FUNCTIONS
*/
// length of the string
int my_strlen(char * param_1)
{
    if (param_1 == NULL) return -1;
    int len = 0;
    while (param_1[len] != '\0') len++;
    return len;
}
// first occurrence of the character
char * my_strchr(char * param_1, char param_2)
{
    for (int i = 0; i < my_strlen(param_1); i++)
        if (param_1[i] == param_2)
            return &param_1[i];
    return 0;
}
// string copy
char * my_strcpy(char* param_1, char* param_2)
{
    for (int i = 0; i < my_strlen(param_2); i++)
        param_1[i] = param_2[i];
    return param_1;
}
char * my_strncpy(char* param_1, char* param_2, int param_3)
{
	for(int i = 0; i < param_3; i++)
    {
    	param_1[i] = param_2[i];
    }
  	return param_1;
}
//
char * my_strndup(char * param_1, size_t n) 
{
    if (param_1 == NULL) return NULL;
    
    size_t len = my_strlen(param_1);
    
    if (n > len) return NULL;
    
    char* dup = (char*)malloc((n + 1) * sizeof(char));
    
    if (dup == NULL) return NULL;
    
    my_strncpy(dup, param_1, n);
    
    dup[n] = '\0';
    
    return dup;
}
char * my_strdup(char* param_1) 
{
    if (param_1 == NULL) return NULL;    
    
    size_t length = my_strlen(param_1);
    
    char* duplicate = (char*)malloc((length + 1) * sizeof(char));
    
    if (duplicate == NULL) return NULL;

    my_strcpy(duplicate, param_1);
    
    return duplicate;
}

char * my_readline(int fd)
{
	if (fd < 0) return NULL;

	char buffer[READLINE_READ_SIZE + 1];

	int bytes = 0;
	char * new_line;

    // if storage is not NULL,
	if (storage)
	{
        // if there is a new line inside of the "storage"
		if ((new_line = my_strchr(storage, '\n')) != NULL)
		{
			// printf("%s\n", "Inside return function");

            // Duplicate the storage until the new line by correct bytes
			char * line = my_strndup(storage, my_strlen(storage) - my_strlen(new_line));
			// printf("%s\n", line);


			storage = my_strdup(new_line + 1);
			// printf("storage = %s\n", "storage");
			return line;
		}
		else 
        {
			free(storage);
			storage = NULL;
		}
	}
	else 
    {
		while ((bytes = read(fd, buffer, READLINE_READ_SIZE)) > 0)
		{
			if (storage == NULL)
			{
				storage = my_strndup(buffer, bytes);
				// printf("storage = %s\n", storage);
			}
			else 
			{
				char * tmp = my_strdup(storage);
				free(storage);
				storage = malloc(sizeof(char) + my_strlen(tmp) + bytes + 1);
				my_strcpy(storage, tmp);
				strncat(storage, buffer, bytes);
				// printf("storage = %s\n", "storage");
			}
		}
		storage[my_strlen(storage) - 1] = '\n';
		return my_readline(fd);
	}

	return NULL;
}

int main(int ac, char ** av)
{
	char * str = NULL;
    int fd;

    // Qwasar: we will change the value of READLINE_READ_SIZE
    // check for if the read size "./my_readline file READLINE_READ_SIZE"
	if (ac == 3)
	{
		READLINE_READ_SIZE = atoi(av[2]);

		fd = open(av[1], O_RDONLY);
		// printf("%d\n", fd);

		if (fd == -1)
		{
            printf("Error: Unable to open %s", av[1]);
			return -1;
		}
    }   
       
    // loop and print
    while ((str = my_readline(fd)) != NULL)
    {
        printf("%s\n", str);
        free(str);
    }
    close(fd);
	
	return 0;
}