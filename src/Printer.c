
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

    static unsigned int test = 0;

    while (!pd->finished)
    {
        clear();

        mtx_lock(&pd->mtx_cpu_usage);

        printw("cpu usage %d: %.2f %%\n", test++, pd->cpu_usage[0] * 100);
        for (int i = 1; i < pd->core_cnt + 1; i++)
        {
            printw(" cpu%d : %.2f %%\n", i - 1, pd->cpu_usage[i] * 100);
        }

        mtx_unlock(&pd->mtx_cpu_usage);

        refresh();

        thrd_sleep(&(struct timespec){.tv_sec = 1}, NULL); // sleep 1s
    }

    return 0;
}