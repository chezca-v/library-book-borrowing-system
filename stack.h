/*
 * stack.h
 * Stack implementation for Return History (LIFO)
 */

#ifndef STACK_H
#define STACK_H

#include "utils.h"

/* Return Record (Stack Node) */
typedef struct ReturnRecord {
    char studentName[MAX_STRING];
    char bookTitle[MAX_STRING];
    int bookId;
    int rating;
    char dateReturned[20];
    struct ReturnRecord *next;
} ReturnRecord;

/* Global stack pointer */
extern ReturnRecord *returnStack;

/* Function prototypes */
void pushReturn(const char *studentName, const char *bookTitle, 
                int bookId, int rating);
ReturnRecord* popReturn(void);
ReturnRecord* peekReturn(void);
void displayReturnHistory(const char *studentName);
void freeStack(void);

#endif /* STACK_H */