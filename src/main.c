#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <threads.h>

#include "Program_data.h"
#include "Reader.h"
#include "Queue.h"
#include "Analyzer.h"
#include "Printer.h"
#include "core_counter.h"

volatile sig_atomic_t done = 0;

void term(int signum)
{
    done = 1;
}

int main(void)
{
    // Initialization **************

    struct sigaction action;
    memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler = term;
    sigaction(SIGTERM, &action, NULL);

    unsigned int core_cnt = 0;
    count_cores(&core_cnt);

    struct Program_data *pd = Program_data_new(core_cnt);
    Printer_init();

    thrd_t thrd[3];

    thrd_create(&thrd[0], Printer_thread, pd);

    // ******************************

    while (!done)
    {
        thrd_sleep(&(struct timespec){.tv_sec = 1}, NULL);
    }

    // cleanin up
    printf("terminated\n");
    pd->finished = true;

    thrd_join(thrd[0], NULL);

    Program_data_free(pd);
    Printer_close();

    return 0;
}
