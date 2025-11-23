/*
 * utils.c
 * Implementation of utility functions
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"

void clearScreen(void) {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pressEnter(void) {
    printf("\nPress ENTER to continue...");
    while(getchar() != '\n');
    getchar();
}

void getCurrentDate(char *buffer) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(buffer, 20, "%Y-%m-%d", tm_info);
}

void addDaysToDate(char *buffer, int days) {
    time_t t = time(NULL);
    t += days * 24 * 60 * 60;
    struct tm *tm_info = localtime(&t);
    strftime(buffer, 20, "%Y-%m-%d", tm_info);
}

void printStars(int rating) {
    int i;
    for (i = 0; i < 5; i++) {
        if (i < rating) printf("*");
        else printf("-");
    }
}