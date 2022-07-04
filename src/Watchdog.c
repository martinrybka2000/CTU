#include <threads.h>
#include <stdatomic.h>
#include <stdio.h>

#include "Watchdog.h"
#include "Program_data.h"

int Watchdog_thread(void *pdv)
{
    struct Program_data *restrict pd = pdv;

    while (!pd->finished)
    {
        for (size_t i = 0; i < NR_OF_WATCHDOG_THREADS; i++)
        {
            pd->watchdog_flags[i] = false;
        }

        thrd_sleep(&(struct timespec){.tv_sec = 2}, NULL);

        if (pd->watchdog_flags[Reader_f] == false)
        {
            perror("Thread \"Reader\" not responding \n");
            pd->finished = true;
            return -1;
        }
        if (pd->watchdog_flags[Analyzer_f] == false)
        {
            perror("Thread \"Analyzer\" not responding \n");
            pd->finished = true;
            return -1;
        }
        if (pd->watchdog_flags[Printer_f] == false)
        {
            perror("Thread \"Printer\" not responding \n");
            pd->finished = true;
            return -1;
        }
    }

    return 0;
}