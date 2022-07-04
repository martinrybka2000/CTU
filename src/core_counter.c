#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "core_counter.h"

int count_cores(unsigned int *core_cnt)
{
    *core_cnt = 0;

    FILE *file = fopen("/proc/cpuinfo", "r");
    if (file == NULL)
    {
        perror("Could not open //proc//cpuinfo file");
        return -1;
    }

    char line[64];

    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (strstr(line, "processor") != NULL)
            *core_cnt += 1;
    }

    fclose(file);

    return 0;
}
