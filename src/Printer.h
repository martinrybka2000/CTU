#ifndef PRINTER_H
#define PRINTER_H

#include "Analyzer.h"

/**
 * Prints the cpu usage from analyzer. Should be called after Print_init()
 * @param analyzer pointer to the analyzer that stores data for cpu usage
 */
void Print(struct Analyzer *analyzer);

/**
 *  Initializes the terminal. Should be called before Print()
 */
void Printer_init();

/**
 *  Clears the terminal to the state before lauching
 */
void Printer_close();

#endif