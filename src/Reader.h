#ifndef READER_H
#define READER_H

#define MAX_LINE_SIZE 128

/**
 * Function for reading cpu usage data from /proc/stat file
 */
char *Read_stat_file();

#endif