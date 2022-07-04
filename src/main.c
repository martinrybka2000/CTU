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

// variable for SIGTERM signal
volatile sig_atomic_t done = 0;

void term(int signum)
{
    done = 1;
}

int main(void)
{
    //************ Initialization **************

    // for SIGTERM signal
    struct sigaction action;
    memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler = term;
    sigaction(SIGTERM, &action, NULL);

    unsigned int core_cnt = 0;
    if (count_cores(&core_cnt) == -1)
    {
        perror("Error while reading core number");
        return -1;
    }

    struct Analyzer *analyzer = Analyzer_new(core_cnt);
    if (analyzer == NULL)
    {
        perror("Error while creating Analyzer");
        return -1;
    }

    struct Program_data *pd = Program_data_new(core_cnt, 20, analyzer);
    if (pd == NULL)
    {
        perror("ERROR while creating Program_data");
        return -1;
    }

    Printer_init();

    thrd_t thrd[3];
    thrd_create(&thrd[0], Printer_thread, pd);
    thrd_create(&thrd[1], Reader_thread, pd);
    thrd_create(&thrd[2], Analyzer_thread, pd);

    // ******************************

    while (!done && !pd->finished)
    {
        thrd_sleep(&(struct timespec){.tv_sec = 1}, NULL); // sleep for 1s
    }

    // cleaning up
    printf("terminated\n");
    pd->finished = true;

    thrd_join(thrd[0], NULL);
    thrd_join(thrd[1], NULL);
    thrd_join(thrd[2], NULL);

    Analyzer_free(analyzer);
    Program_data_free(pd);
    Printer_close();

    return 0;
}
