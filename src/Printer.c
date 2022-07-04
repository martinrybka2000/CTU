
#include <ncurses.h>

#include "Printer.h"
#include "Program_data.h"

void Printer_init()
{
    initscr();

    refresh();
}

void Printer_close()
{
    endwin();
}

int Printer_thread(void *pdv)
{
    struct Program_data *restrict pd = pdv;

    while (!pd->finished)
    {
        clear();

        mtx_lock(&pd->mtx_cpu_usage);

        // setting watchdog flag
        pd->watchdog_flags[Printer_f] = true;

        printw("cpu usage: %.2f %%\n", pd->cpu_usage[0] * 100);
        for (unsigned int i = 1; i < pd->core_cnt + 1; i++)
        {
            printw(" cpu%d : %.2f %%\n", i - 1, pd->cpu_usage[i] * 100);
        }

        mtx_unlock(&pd->mtx_cpu_usage);

        refresh();

        thrd_sleep(&(struct timespec){.tv_sec = 1}, NULL); // sleep 1s
    }

    return 0;
}
