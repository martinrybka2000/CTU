#ifndef ANALYZER_H
#define ANALYZER_H

#include <stdbool.h>

// hard coded number of vields in /proc/stat
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

/**
 * struct for storing Analyzer data
 * @param prev_data data from previous read
 * @param current_data data from current read
 * @param first_read flag for first read from the queue the cpu usage can not be yet calculated
 */
struct Analyzer
{
    unsigned long long *prev_data;
    unsigned long long *current_data;

    bool first_read;
};

/**
 * Constructor for Analyzer data
 * @param nr_of_cores number of cores in the system
 * @param return newly constructed Analyzer_data
 */
struct Analyzer *Analyzer_new(unsigned long long nr_of_cores);

/**
 * Destructor for Analyzer_data should be used after Analyzer_new(),
 * Frees previusly allocated data and itself
 * @param analyzer Analyzer data to destroy
 */
void Analyzer_free(struct Analyzer *analyzer);

/**
 * Thread function for calculating cpu usage
 */
int Analyzer_thread(void *);

#endif