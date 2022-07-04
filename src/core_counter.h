#ifndef CORE_COUNTER_H
#define CORE_COUNTER_H

/**
 * Function for counting the number of cores in the system.
 * It reads the /proc/cpuinfo file for counting the cores.
 * @param core_cnt variable for storing the result of the function
 * @param return 0 if succes, -1 if failed
 */
int count_cores(unsigned int *core_cnt);

#endif