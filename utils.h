/*
 * utils.h
 * Utility functions and common definitions
 */

#ifndef UTILS_H
#define UTILS_H

#define MAX_STRING 200
#define MAX_BOOKS_PER_STUDENT 3

/* --- COLOR CODES (ANSI) --- */
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_BOLD    "\x1b[1m"

/* Utility Functions */
void clearScreen(void);
void pressEnter(void);
void getCurrentDate(char *buffer);
void addDaysToDate(char *buffer, int days);
void printStars(int rating);

#endif /* UTILS_H */