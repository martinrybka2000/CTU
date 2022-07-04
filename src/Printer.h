#ifndef PRINTER_H
#define PRINTER_H

/**
 * Thread function for printing cpu_usage
 * ! memory leak becouse of ncurses usage
 */
int Printer_thread(void *);

/**
 *  Initializes the terminal. Should be called before Print()
 */
void Printer_init(void);

/**
 *  Clears the terminal to the state before lauching
 */
void Printer_close(void);

#endif
