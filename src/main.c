#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <threads.h>

#include "Reader.h"
#include "Queue.h"
#include "Analyzer.h"
#include "Printer.h"
#include "core_counter.h"

//
int main(void)
{
    unsigned int core_cnt = 0;

    count_cores(&core_cnt);

    Printer_init();
    struct queue *q = queue_new();

    struct Analyzer *analyzer = Analyzer_new(core_cnt + 1, q);

    for (size_t i = 0; i < 10; i++)
    {
        queue_push(q, Read_stat_file());

        Analyzer_calc_usage(analyzer);

        Print(analyzer);

        thrd_sleep(&(struct timespec){.tv_sec = 1}, NULL); // sleep 1s
    }

    queue_free_all(q);
    Analyzer_free(analyzer);
    Printer_close();

    return 0;
}
