
#include <stdlib.h>

#include "Program_data.h"

struct Program_data *Program_data_new(unsigned int core_cnt)
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

    pd->finished = false;
    pd->core_cnt = core_cnt;

    if (mtx_init(&(pd->mtx_queue), mtx_plain) == thrd_error)
    {
        return NULL;
    }
    if (mtx_init(&(pd->mtx_queue), mtx_plain) == thrd_error)
    {
        return NULL;
    }

    int err = cnd_init(&(pd->cnd_queue));
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

        cnd_destroy(&(pd->cnd_queue));

        free(pd);
        return 0;
    }
    return -1;
}