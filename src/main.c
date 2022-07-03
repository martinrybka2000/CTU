#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include "Reader.h"
#include "Queue.h"

int main(void)
{
    for (size_t i = 0; i < 10; i++)
    {
        char *raw_data = read_stat();

        printf("%s", raw_data);

        free(raw_data);

        sleep(1);
    }

    return 0;
}
