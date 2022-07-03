#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <threads.h>

#include "Reader.h"
#include "Queue.h"
#include "Analyzer.h"

int main(void)
{
    struct queue *q = queue_new();

    struct Analyzer *analyzer = Analyzer_new(NR_OF_CORES + 1, q);

    for (size_t i = 0; i < 1000; i++)
    {
        queue_push(q, read_stat());

        Analyzer_calc_usage(analyzer);

        printf("cpu all usage: %f %%\n", analyzer->cpu_usage[0] * 100);
        for (int j = 1; j < NR_OF_CORES + 1; j++)
        {
            printf("\tcpu%d usage: %f %%\n", j - 1, analyzer->cpu_usage[j] * 100);
        }
        printf("\n");

        thrd_sleep(&(struct timespec){.tv_nsec = 100000000}, NULL); // sleep 100ms
    }

    queue_free_all(q);
    Analyzer_free(analyzer);

    return 0;
}
