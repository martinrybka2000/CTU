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

    struct queue *q = queue_new();

    for (size_t i = 0; i < 10; i++)
    {
        queue_push(q, read_stat());

        printf("%s\n", queue_pop_head(q));

        sleep(1);
    }

    queue_free_all(q);

    return 0;
}
