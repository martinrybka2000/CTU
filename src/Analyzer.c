#include "Analyzer.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <threads.h>
#include <stdatomic.h>

#include "Program_data.h"

struct Analyzer *Analyzer_new(unsigned long long nr_of_cores)
{
    struct Analyzer *analyzer = malloc(sizeof(struct Analyzer));

    analyzer->first_read = true;

    analyzer->prev_data = (unsigned long long *)malloc(((nr_of_cores + 1) * NR_OF_COLUMNS) * sizeof(unsigned long long));
    analyzer->current_data = (unsigned long long *)malloc(((nr_of_cores + 1) * NR_OF_COLUMNS) * sizeof(unsigned long long));

    for (size_t i = 0; i < (nr_of_cores + 1) * NR_OF_COLUMNS; i++)
    {
        analyzer->current_data[i] = 0;
        analyzer->prev_data[i] = 0;
    }

    return analyzer;
}

void Analyzer_free(struct Analyzer *analyzer)
{
    free(analyzer->current_data);
    free(analyzer->prev_data);

    free(analyzer);
}

int Analyzer_thread(void *pdv)
{
    struct Program_data *restrict pd = pdv;

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

    while (!pd->finished)
    {
        mtx_lock(&pd->mtx_queue);

        while (!pd->finished && queue_get_length(pd->raw_data) == 0)
        {
            wait_cnd(&pd->cnd_queue_nonempty, &pd->mtx_queue);
        }
        if (queue_get_length(pd->raw_data) == 0)
            return -1;

        char *ptr = queue_peek_head(pd->raw_data);

        if (pd->analyzer_data->first_read)
        {
            for (size_t i = 0; i < pd->core_cnt + 1; i++)
            {
                diff = NR_OF_COLUMNS * i;

                sscanf(ptr,
                       "%s %16llu %16llu %16llu %16llu %16llu %16llu %16llu %16llu %16llu %16llu",
                       str,
                       &pd->analyzer_data->prev_data[diff + user_c], &pd->analyzer_data->prev_data[diff + nice_c], &pd->analyzer_data->prev_data[diff + system_c], &pd->analyzer_data->prev_data[diff + iowat_c], &pd->analyzer_data->prev_data[diff + idle_c],
                       &pd->analyzer_data->prev_data[diff + irq_c], &pd->analyzer_data->prev_data[diff + softirq_c], &pd->analyzer_data->prev_data[diff + steal_c], &pd->analyzer_data->prev_data[diff + quest_c], &pd->analyzer_data->prev_data[diff + quest_nice_c]);

                ptr = strchr(ptr, '\n') + 1;
            }

            pd->analyzer_data->first_read = false;
        }
        else
        {
            for (size_t i = 0; i < pd->core_cnt + 1; i++)
            {
                diff = NR_OF_COLUMNS * i;

                sscanf(ptr,
                       "%s %16llu %16llu %16llu %16llu %16llu %16llu %16llu %16llu %16llu %16llu",
                       str,
                       &pd->analyzer_data->current_data[diff + user_c], &pd->analyzer_data->current_data[diff + nice_c], &pd->analyzer_data->current_data[diff + system_c], &pd->analyzer_data->current_data[diff + iowat_c], &pd->analyzer_data->current_data[diff + idle_c],
                       &pd->analyzer_data->current_data[diff + irq_c], &pd->analyzer_data->current_data[diff + softirq_c], &pd->analyzer_data->current_data[diff + steal_c], &pd->analyzer_data->current_data[diff + quest_c], &pd->analyzer_data->current_data[diff + quest_nice_c]);

                PrevIdle = pd->analyzer_data->prev_data[diff + idle_c] + pd->analyzer_data->prev_data[diff + iowat_c];
                Idle = pd->analyzer_data->current_data[diff + idle_c] + pd->analyzer_data->current_data[diff + iowat_c];

                PrevNonIdle = pd->analyzer_data->prev_data[diff + user_c] + pd->analyzer_data->prev_data[diff + nice_c] + pd->analyzer_data->prev_data[diff + system_c] + pd->analyzer_data->prev_data[diff + irq_c] + pd->analyzer_data->prev_data[diff + softirq_c] + pd->analyzer_data->prev_data[diff + steal_c];
                NonIdle = pd->analyzer_data->current_data[diff + user_c] + pd->analyzer_data->current_data[diff + nice_c] + pd->analyzer_data->current_data[diff + system_c] + pd->analyzer_data->current_data[diff + irq_c] + pd->analyzer_data->current_data[diff + softirq_c] + pd->analyzer_data->current_data[diff + steal_c];

                PrevTotal = PrevIdle + PrevNonIdle;
                Total = Idle + NonIdle;

                // actual value minus the previous one
                totald = Total - PrevTotal;
                idled = Idle - PrevIdle;

                mtx_lock(&pd->mtx_cpu_usage);
                pd->cpu_usage[i] = (double)(totald - idled) / totald;
                mtx_unlock(&pd->mtx_cpu_usage);

                ptr = strchr(ptr, '\n') + 1;
            }

            // coping actual to prev reads
            for (size_t i = 0; i < (pd->core_cnt + 1) * NR_OF_COLUMNS; i++)
            {
                pd->analyzer_data->prev_data[i] = pd->analyzer_data->current_data[i];
            }
        }

        queue_pop_free_head(pd->raw_data);

        cnd_signal(&pd->cnd_queue_nonfull);
        mtx_unlock(&pd->mtx_queue);
    }

    return 0;
}