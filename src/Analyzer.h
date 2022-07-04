#ifndef ANALYZER_H
#define ANALYZER_H

#include <stdbool.h>

// hard coded
#define NR_OF_COLUMNS 10

/**
 * enum for storing the names of columns in /proc/stat file
 */
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
    unsigned long long *prev_data;
    unsigned long long *current_data;

    bool first_read;
};

struct Analyzer *Analyzer_new(unsigned long long nr_of_cores);

void Analyzer_free(struct Analyzer *analyzer);

int Analyzer_thread(void *);

#endif