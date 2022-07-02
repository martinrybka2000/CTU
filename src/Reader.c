
#include "Reader.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char *read_stat()
{
    // opening the file
    FILE *file = fopen("/proc/stat", "r");
    if (file == NULL)
    {
        perror("Could not open //proc//stat file");
        return NULL;
    }

    // dynamic data for the file data
    char *data = malloc(16);
    char *iterator = data;
    unsigned int data_size = 0;

    while (1)
    {
        // buffer for reading one line of text
        char buffer[MAX_LINE_SIZE];
        char *new_line = fgets(buffer, sizeof(buffer), file);

        if (new_line == NULL)
        {
            perror("Could not read stat file");
            fclose(file);
            free(data);
            return NULL;
        }

        // if the cpu usage lines endded then stop reading
        if (strstr(buffer, "cpu") == NULL)
        {
            break;
        }

        // realocating memory for new data
        unsigned int buff_len = strlen(buffer);
        char *foo = realloc(data, buff_len + data_size);
        if (foo == NULL)
        {
            perror("Could not reloocate the memory");
            fclose(file);
            free(data);
            return NULL;
        }
        data = foo;

        // moving the iterator to the end of last data
        iterator = data + data_size;
        data_size += buff_len;

        memcpy(iterator, buffer, buff_len);
    }

    fclose(file);

    return data;
}