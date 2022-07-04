#ifndef PROGRAM_DATA_H
#define PROGRAM_DATA_H

#include <threads.h>
#include <stdatomic.h>

#include "Queue.h"
#include "Analyzer.h"

// number of threads for Wachdog to track
#define NR_OF_WATCHDOG_THREADS 3

enum
{
    Reader_f,
    Analyzer_f,
    Printer_f
};

/**
 * Structure for holding program data
 * @param raw_data queue for holding data read from /proc/stat
 * @param queue_max_length Number of stored data before the Reader thread will wait
 * @param mtx_queue mutex for the queue
 * @param cnd_queue_nonempty condition variable for signaling that the queue is not empty
 * @param cnd_queue_nonfull condition variable for signaling that the queue is not full
 * @param cpu_usage array for storing cpu usage
 * @param mtx_cpu_usage mutex fot the cpu_usage array
 * @param finished flag for indicating the end of the program
 * @param core_cnt number of cores in the system
 * @param analyzer_data data for the Analyzer to use
 * @param watchdog_flags flags for watchdog tracking
 */
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

    atomic_bool watchdog_flags[NR_OF_WATCHDOG_THREADS];
};

/**
 *  Constructor for the Program data
 * @param core_cnt number of cores in the system
 * @param queue_max_length specifies the maximum length of the queue
 * @param analyzer_data should be allocated before, data for Analyzer to use
 * @param return newly allocated Program_data
 */
struct Program_data *Program_data_new(unsigned int core_cnt, unsigned int queue_max_length, struct Analyzer *analyzer_data);

/**
 * Should be used after Program_data_new, frees allocated data and itself
 * @param pd object to destroy
 */
int Program_data_free(struct Program_data *pd);

/**
 * Function for waiting inside a thread, it waits for the condition variable or timeout of 1s
 * @param cnd condition variable to wait
 * @param mtx mutex to unlock while waiting
 */
int wait_cnd(cnd_t *cnd, mtx_t *mtx);

#endif