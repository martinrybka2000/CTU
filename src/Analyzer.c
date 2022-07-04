#include "Analyzer.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct Analyzer *Analyzer_new(unsigned long long nr_of_cores, struct queue *raw_data_queue)
{
    struct Analyzer *analyzer = malloc(sizeof(struct Analyzer));

    analyzer->nr_of_cores = nr_of_cores;
    analyzer->raw_data = raw_data_queue;
    analyzer->first_read = true;

    analyzer->cpu_usage = (double *)malloc(nr_of_cores * sizeof(double));
    analyzer->prev_data = (unsigned long long *)malloc((nr_of_cores * NR_OF_COLUMNS) * sizeof(unsigned long long));
    analyzer->current_data = (unsigned long long *)malloc((nr_of_cores * NR_OF_COLUMNS) * sizeof(unsigned long long));

    for (size_t i = 0; i < nr_of_cores; i++)
    {
        analyzer->cpu_usage[i] = 0;
    }

    for (size_t i = 0; i < nr_of_cores * NR_OF_COLUMNS; i++)
    {
        analyzer->current_data[i] = 0;
        analyzer->prev_data[i] = 0;
    }

    return analyzer;
}

void Analyzer_free(struct Analyzer *analyzer)
{
    free(analyzer->cpu_usage);
    free(analyzer->current_data);
    free(analyzer->prev_data);

    free(analyzer);
}

void Analyzer_calc_usage(struct Analyzer *analyzer)
{
    if (queue_get_length(analyzer->raw_data) > 0)
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

        char *ptr = queue_peek_head(analyzer->raw_data);

        if (analyzer->first_read)
        {
            for (size_t i = 0; i < analyzer->nr_of_cores; i++)
            {
                diff = NR_OF_COLUMNS * i;

                sscanf(ptr,
                       "%s %16llu %16llu %16llu %16llu %16llu %16llu %16llu %16llu %16llu %16llu",
                       str,
                       &analyzer->prev_data[diff + user_c], &analyzer->prev_data[diff + nice_c], &analyzer->prev_data[diff + system_c], &analyzer->prev_data[diff + iowat_c], &analyzer->prev_data[diff + idle_c],
                       &analyzer->prev_data[diff + irq_c], &analyzer->prev_data[diff + softirq_c], &analyzer->prev_data[diff + steal_c], &analyzer->prev_data[diff + quest_c], &analyzer->prev_data[diff + quest_nice_c]);

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
                       &analyzer->current_data[diff + user_c], &analyzer->current_data[diff + nice_c], &analyzer->current_data[diff + system_c], &analyzer->current_data[diff + iowat_c], &analyzer->current_data[diff + idle_c],
                       &analyzer->current_data[diff + irq_c], &analyzer->current_data[diff + softirq_c], &analyzer->current_data[diff + steal_c], &analyzer->current_data[diff + quest_c], &analyzer->current_data[diff + quest_nice_c]);

                PrevIdle = analyzer->prev_data[diff + idle_c] + analyzer->prev_data[diff + iowat_c];
                Idle = analyzer->current_data[diff + idle_c] + analyzer->current_data[diff + iowat_c];

                PrevNonIdle = analyzer->prev_data[diff + user_c] + analyzer->prev_data[diff + nice_c] + analyzer->prev_data[diff + system_c] + analyzer->prev_data[diff + irq_c] + analyzer->prev_data[diff + softirq_c] + analyzer->prev_data[diff + steal_c];
                NonIdle = analyzer->current_data[diff + user_c] + analyzer->current_data[diff + nice_c] + analyzer->current_data[diff + system_c] + analyzer->current_data[diff + irq_c] + analyzer->current_data[diff + softirq_c] + analyzer->current_data[diff + steal_c];

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
                analyzer->prev_data[i] = analyzer->current_data[i];
            }
        }
        queue_pop_free_head(analyzer->raw_data);
    }
}