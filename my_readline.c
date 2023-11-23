#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
// two global variables (one for your "storage" and one for READLINE_READ_SIZE)
int READLINE_READ_SIZE = 7;
char * storage;

// will init (or reinitialize) the storage
void init_my_readline()
{
	storage = NULL;
}

/*
    I HAVE RECEIVED A BAD PEER REVIEW BECAUSE OF I USED STRCPY() FUNCTION WHICH WASN'T IN THE AUTHORIZED FUNCTIONS LIST
    HERE IS SOME HAND MADE FRESH FUNCTIONS
*/
// length of the string
size_t my_strlen(const char * param_1)
{
    if (param_1 == NULL) return 0;
    size_t  len = 0;
    while (param_1[len] != '\0') 
        len++;
    return len;
}
// first occurrence of the character
char * my_strchr(char * param_1, char param_2)
{
    int len = my_strlen(param_1);
    for (int i = 0; i < len; i++)
        if (param_1[i] == param_2)
            return &param_1[i];
    return 0;
}
// string copy
char * my_strcpy(char* param_1, char* param_2)
{
    int len = my_strlen(param_2);
    for (int i = 0; i < len; i++)
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

char *my_strncat(char *dest, const char *src, size_t n) {
    char *dest_end = dest + strlen(dest); // Find the end of the destination string
    
    while (*src != '\0' && n > 0) {
        *dest_end++ = *src++; // Copy characters from src to dest
        n--;
    }
    
    *dest_end = '\0'; // Null-terminate the concatenated string
    
    return dest;
}


char * my_readline(int fd)
{
	if (fd < 0) return NULL;

    // buffer for the readed data, bytes to count the data and after reading the all file new_line to know how to print the data
	char buffer[READLINE_READ_SIZE + 1];
	int bytes = 0;
	char * new_line;

    // if storage is not NULL (already some data saved), check for the '\n' to find the lines. No '\n' means clean the storage
	if (storage != NULL)
	{
        // if there is a new line inside of the "storage"
		if ((new_line = my_strchr(storage, '\n')) != NULL)
		{
            // Duplicate the storage until the new line by correct bytes
            size_t line_len = (my_strlen(storage) - my_strlen(new_line));
            char * line = (char*)malloc((line_len + 1) * sizeof(char));
            my_strncpy(line, storage, line_len);
            line[line_len] = '\0'; // Null-terminate the string
            // MALLOC THE STORAGE AND COPY THE NEW LINE INSIDE
            storage = (char*)malloc(( my_strlen(new_line + 1) + 1) * sizeof(char));

            my_strcpy(storage, (new_line + 1));

			return line;
		}
		else 
        {
			free(storage);
			init_my_readline();
		}
	}
	else 
    {
        // read() attempts to read up to count bytes from file descriptor fd into the buffer starting at buf.
        
		while ((bytes = read(fd, buffer, READLINE_READ_SIZE)) > 0)
		{
            // if storage is empty, put the buffer on it
			if (storage == NULL)
			{
                storage = (char*)malloc((bytes + 1) * sizeof(char));
                my_strncpy(storage, buffer, bytes);
			}
            // if data in the storage, then put it to a tmp, malloc the storage and append with the buffer
			else 
			{                
                char * tmp = (char*)malloc(( my_strlen(storage) + 1) * sizeof(char));
                my_strcpy(tmp, storage);

				free(storage);

                storage = (char*)malloc(( my_strlen(tmp) + bytes + 1) * sizeof(char));

				my_strcpy(storage, tmp);
				my_strncat(storage, buffer, bytes);
			}
		}
        
        size_t storage_length = my_strlen(storage);
        // printf("storage_length: %zu\n", storage_length);
        if (storage_length > 0 && storage[storage_length - 1] == '\n') {
            // printf("There is a newline at the end of storage.\n");
        } else {
            // printf("There is no newline at the end of storage.\n");
            storage[storage_length] = '\n'; // new line at the end
            storage[storage_length + 1] = '\0'; // null-terminate the string
        }

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
    if (ac < 2)
    {
        while ((str = my_readline(0)) != NULL)
        {
            printf("%s\n", str);
            free(str);
        }
	}
    if (ac == 2)
    {
        fd = open(av[1], O_RDONLY);

        if (fd == -1)
        {
            printf("Error: Unable to open %s", av[1]);
            return -1;
        }
    }
    else if (ac == 3) // Corrected to "else if"
    {
        READLINE_READ_SIZE = atoi(av[2]);
        fd = open(av[1], O_RDONLY);

        if (fd == -1)
        {
            printf("Error: Unable to open %s\n", av[1]);
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


// int main()
// {
//     char *str = NULL;
//     // loop and print
//     while ((str = my_readline(0)) != NULL)
//     {
//         printf("%s\n", str);
//         free(str);
//     }
//     return 0;
// }