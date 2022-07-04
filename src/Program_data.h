#ifndef PROGRAM_DATA_H
#define PROGRAM_DATA_H

#include <threads.h>
#include <stdatomic.h>

#include "Queue.h"
#include "Analyzer.h"

struct Program_data
{
    struct queue *raw_data;
    mtx_t mtx_queue;
    cnd_t cnd_queue;

    double *cpu_usage;
    mtx_t mtx_cpu_usage;

    atomic_bool finished;
    atomic_uint core_cnt;
};

struct Program_data *Program_data_new(unsigned int core_cnt);

int Program_data_free(struct Program_data *pd);

#endif