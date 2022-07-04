
#include <stdio.h>

#include "Printer.h"
#include "Program_data.h"

int Printer_thread(void *pdv)
{
    struct Program_data *restrict pd = pdv;

    while (!pd->finished)
    {
        mtx_lock(&pd->mtx_cpu_usage);

        // setting watchdog flag
        pd->watchdog_flags[Printer_f] = true;

        printf("\033[2J\033[1;1H"); // clearing the console
        printf("cpu usage: %.2f %%\n", pd->cpu_usage[0] * 100);
        for (unsigned int i = 1; i < pd->core_cnt + 1; i++)
        {
            printf(" cpu%d : %.2f %%\n", i - 1, pd->cpu_usage[i] * 100);
        }

        mtx_unlock(&pd->mtx_cpu_usage);

        thrd_sleep(&(struct timespec){.tv_sec = 1}, NULL); // sleep 1s
    }

    return 0;
}
