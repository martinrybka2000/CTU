
#include <stdlib.h>

#include "Program_data.h"

struct Program_data *Program_data_new(unsigned int core_cnt, unsigned int queue_max_length, struct Analyzer *analyzer_data)
{
    struct Program_data *pd = malloc(sizeof(struct Program_data));
    struct queue *q = queue_new();
    double *cpu_usage = (double *)malloc((core_cnt + 1) * sizeof(double));

    if (pd == NULL || q == NULL || cpu_usage == NULL)
    {
        return NULL;
    }

    pd->raw_data = q;
    pd->cpu_usage = cpu_usage;
    pd->analyzer_data = analyzer_data;

    pd->finished = false;
    pd->core_cnt = core_cnt;
    pd->queue_max_length = queue_max_length;

    for (size_t i = 0; i < NR_OF_WATCHDOG_THREADS; i++)
    {
        pd->watchdog_flags[i] = true;
    }

    if (mtx_init(&(pd->mtx_queue), mtx_plain) == thrd_error)
    {
        return NULL;
    }
    if (mtx_init(&(pd->mtx_cpu_usage), mtx_plain) == thrd_error)
    {
        return NULL;
    }

    int err = cnd_init(&(pd->cnd_queue_nonempty));
    if (err == thrd_error || err == thrd_nomem)
    {
        return NULL;
    }
    err = cnd_init(&(pd->cnd_queue_nonfull));
    if (err == thrd_error || err == thrd_nomem)
    {
        return NULL;
    }

    for (size_t i = 0; i < core_cnt + 1; i++)
    {
        pd->cpu_usage[i] = 0;
    }

    return pd;
}

int Program_data_free(struct Program_data *pd)
{
    if (pd != NULL)
    {
        queue_free_all(pd->raw_data);
        free(pd->cpu_usage);

        mtx_destroy(&(pd->mtx_queue));
        mtx_destroy(&(pd->mtx_cpu_usage));

        cnd_destroy(&(pd->cnd_queue_nonempty));
        cnd_destroy(&(pd->cnd_queue_nonfull));

        free(pd);
        return 0;
    }
    return -1;
}

int wait_cnd(cnd_t *cnd, mtx_t *mtx)
{
    struct timespec now;
    timespec_get(&now, TIME_UTC);
    now.tv_sec += 1;
    return cnd_timedwait(cnd, mtx, &now);
}
