/*
 * history.h
 * Borrow History tracking system
 */

#ifndef HISTORY_H
#define HISTORY_H

#include "utils.h"

/* Borrow History Record */
typedef struct BorrowHistory {
    char studentId[50];
    char studentName[MAX_STRING];
    char bookTitle[MAX_STRING];
    int bookId;
    char borrowDate[20];
    char returnDate[20];
    int returned;
    struct BorrowHistory *next;
} BorrowHistory;

/* Global history list */
extern BorrowHistory *historyList;

/* Function prototypes */
void addBorrowHistory(const char *studentId, const char *studentName,
                      const char *bookTitle, int bookId);
void markAsReturned(const char *studentId, int bookId);
void displayBorrowHistory(const char *studentName);
int getCurrentBorrowedCount(const char *studentId);
void freeHistoryList(void);

#endif /* HISTORY_H */