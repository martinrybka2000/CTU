#ifndef PROGRAM_DATA_H
#define PROGRAM_DATA_H

#include <threads.h>
#include <stdatomic.h>

#include "Queue.h"
#include "Analyzer.h"

struct Program_data
{
    struct queue *raw_data;
    atomic_uint queue_max_length;
    mtx_t mtx_queue;
    cnd_t cnd_queue_nonempty;
    cnd_t cnd_queue_nonfull;

    double *cpu_usage;
    mtx_t mtx_cpu_usage;

    atomic_bool finished;
    atomic_uint core_cnt;

    struct Analyzer *analyzer_data;
};

struct Program_data *Program_data_new(unsigned int core_cnt, unsigned int queue_max_length, struct Analyzer *analyzer_data);

int Program_data_free(struct Program_data *pd);

int wait_cnd(cnd_t *cnd, mtx_t *mtx);

#endif