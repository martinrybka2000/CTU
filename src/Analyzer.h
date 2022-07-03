#ifndef ANALYZER_H
#define ANALYZER_H

#include <stdbool.h>
#include "Queue.h"

// for now hard coded
#define NR_OF_CORES 8
#define NR_OF_COLUMNS 10

enum columns
{
    user_c = 0,
    nice_c,
    system_c,
    idle_c,
    iowat_c,
    irq_c,
    softirq_c,
    steal_c,
    quest_c,
    quest_nice_c
};

struct Analyzer
{
    unsigned int nr_of_cores;
    double *cpu_usage;

    struct queue *que_data;

    unsigned long long *prev;
    unsigned long long *now;

    bool first_read;
};

struct Analyzer *Analyzer_new(unsigned long long nr_of_cores, struct queue *raw_data_queue);

void Analyzer_free(struct Analyzer *analyzer);

void Analyzer_calc_usage(struct Analyzer *analyzer);

#endif