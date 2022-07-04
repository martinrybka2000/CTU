#include "Analyzer.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct Analyzer *Analyzer_new(unsigned long long nr_of_cores, struct queue *raw_data_queue)
{
    struct Analyzer *analyzer = malloc(sizeof(struct Analyzer));

    analyzer->nr_of_cores = nr_of_cores;
    analyzer->que_data = raw_data_queue;
    analyzer->first_read = true;

    analyzer->cpu_usage = (double *)malloc(nr_of_cores * sizeof(double));
    analyzer->prev = (unsigned long long *)malloc((nr_of_cores * NR_OF_COLUMNS) * sizeof(unsigned long long));
    analyzer->now = (unsigned long long *)malloc((nr_of_cores * NR_OF_COLUMNS) * sizeof(unsigned long long));

    for (size_t i = 0; i < nr_of_cores; i++)
    {
        analyzer->cpu_usage[i] = 0;
    }

    for (size_t i = 0; i < nr_of_cores * NR_OF_COLUMNS; i++)
    {
        analyzer->now[i] = 0;
        analyzer->prev[i] = 0;
    }

    return analyzer;
}

void Analyzer_free(struct Analyzer *analyzer)
{
    free(analyzer->cpu_usage);
    free(analyzer->now);
    free(analyzer->prev);

    free(analyzer);
}

void Analyzer_calc_usage(struct Analyzer *analyzer)
{
    if (queue_get_length(analyzer->que_data) > 0)
    {
        char str[7];
        unsigned diff;

        unsigned long long PrevIdle;
        unsigned long long Idle;

        unsigned long long PrevNonIdle;
        unsigned long long NonIdle;

        unsigned long long PrevTotal;
        unsigned long long Total;

        unsigned long long totald;
        unsigned long long idled;

        char *ptr = queue_peek_head(analyzer->que_data);

        if (analyzer->first_read)
        {
            for (size_t i = 0; i < analyzer->nr_of_cores; i++)
            {
                diff = NR_OF_COLUMNS * i;

                sscanf(ptr,
                       "%s %16llu %16llu %16llu %16llu %16llu %16llu %16llu %16llu %16llu %16llu",
                       str,
                       &analyzer->prev[diff + user_c], &analyzer->prev[diff + nice_c], &analyzer->prev[diff + system_c], &analyzer->prev[diff + iowat_c], &analyzer->prev[diff + idle_c],
                       &analyzer->prev[diff + irq_c], &analyzer->prev[diff + softirq_c], &analyzer->prev[diff + steal_c], &analyzer->prev[diff + quest_c], &analyzer->prev[diff + quest_nice_c]);

                ptr = strchr(ptr, '\n') + 1;
            }

            analyzer->first_read = false;
        }
        else
        {
            for (size_t i = 0; i < analyzer->nr_of_cores; i++)
            {
                diff = NR_OF_COLUMNS * i;

                sscanf(ptr,
                       "%s %16llu %16llu %16llu %16llu %16llu %16llu %16llu %16llu %16llu %16llu",
                       str,
                       &analyzer->now[diff + user_c], &analyzer->now[diff + nice_c], &analyzer->now[diff + system_c], &analyzer->now[diff + iowat_c], &analyzer->now[diff + idle_c],
                       &analyzer->now[diff + irq_c], &analyzer->now[diff + softirq_c], &analyzer->now[diff + steal_c], &analyzer->now[diff + quest_c], &analyzer->now[diff + quest_nice_c]);

                PrevIdle = analyzer->prev[diff + idle_c] + analyzer->prev[diff + iowat_c];
                Idle = analyzer->now[diff + idle_c] + analyzer->now[diff + iowat_c];

                PrevNonIdle = analyzer->prev[diff + user_c] + analyzer->prev[diff + nice_c] + analyzer->prev[diff + system_c] + analyzer->prev[diff + irq_c] + analyzer->prev[diff + softirq_c] + analyzer->prev[diff + steal_c];
                NonIdle = analyzer->now[diff + user_c] + analyzer->now[diff + nice_c] + analyzer->now[diff + system_c] + analyzer->now[diff + irq_c] + analyzer->now[diff + softirq_c] + analyzer->now[diff + steal_c];

                PrevTotal = PrevIdle + PrevNonIdle;
                Total = Idle + NonIdle;

                // actual value minus the previous one
                totald = Total - PrevTotal;
                idled = Idle - PrevIdle;

                analyzer->cpu_usage[i] = (double)(totald - idled) / totald;

                ptr = strchr(ptr, '\n') + 1;
            }

            // coping actual to prev reads
            for (size_t i = 0; i < analyzer->nr_of_cores * NR_OF_COLUMNS; i++)
            {
                analyzer->prev[i] = analyzer->now[i];
            }
        }
        queue_pop_free_head(analyzer->que_data);
    }
}