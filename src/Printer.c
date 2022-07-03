
#include <ncurses.h>

#include "Printer.h"

void Printer_init()
{
    initscr();

    refresh();
}

void Printed_close()
{
    endwin();
}

void Print(struct Analyzer *analyzer)
{
    if (analyzer != NULL)
    {
        clear();

        printw("cpu usage: %.2f %%\n", analyzer->cpu_usage[0] * 100);

        for (size_t i = 1; i < analyzer->nr_of_cores; i++)
        {
            printw(" cpu%d : %.2f %%\n", i - 1, analyzer->cpu_usage[i] * 100);
        }
        refresh();
    }
}