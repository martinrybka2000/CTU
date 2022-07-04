

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <threads.h>
#include <stdatomic.h>

#include "Reader.h"
#include "Program_data.h"

int Reader_thread(void *pdv)
{
    struct Program_data *restrict pd = pdv;

    char *data;

    while (!pd->finished)
    {
        FILE *file = fopen("/proc/stat", "r");
        if (file == NULL)
        {
            perror("Could not open //proc//stat file");
            return -1;
        }

        // dynamic data for the file data
        data = malloc(16);
        char *iterator = data;
        unsigned int data_size = 0;

        while (!pd->finished)
        {
            // buffer for reading one line of text
            char buffer[MAX_LINE_SIZE];
            char *new_line = fgets(buffer, sizeof(buffer), file);

            if (new_line == NULL)
            {
                perror("Could not read stat file");
                fclose(file);
                free(data);
                return -1;
            }

            // if the cpu usage lines endded then stop reading
            if (strstr(buffer, "cpu") == NULL)
            {
                break;
            }

            // realocating memory for new data
            unsigned int buff_len = strlen(buffer);
            char *tem = realloc(data, buff_len + data_size);
            if (tem == NULL)
            {
                perror("Could not reloocate the memory");
                fclose(file);
                free(data);
                return -1;
            }
            data = tem;

            // moving the iterator to the end of last data
            iterator = data + data_size;
            data_size += buff_len;

            memcpy(iterator, buffer, buff_len);
        }

        fclose(file);

        mtx_lock(&pd->mtx_queue);

        while (!pd->finished && queue_get_length(pd->raw_data) >= pd->queue_max_length)
        {
            wait_cnd(&pd->cnd_queue_nonfull, &pd->mtx_queue);
        }

        queue_push(pd->raw_data, data);

        cnd_signal(&pd->cnd_queue_nonempty);
        mtx_unlock(&pd->mtx_queue);

        // thrd_yield();
        thrd_sleep(&(struct timespec){.tv_nsec = 500000000}, NULL);
    }

    return 0;
}