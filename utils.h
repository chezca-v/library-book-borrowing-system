/*
 * utils.h
 * Utility functions and common definitions
 */

#ifndef UTILS_H
#define UTILS_H

#define MAX_STRING 200
#define MAX_BOOKS_PER_STUDENT 3

/* Utility Functions */
void clearScreen(void);
void pressEnter(void);
void getCurrentDate(char *buffer);
void addDaysToDate(char *buffer, int days);
void printStars(int rating);

#endif /* UTILS_H */