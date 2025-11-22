/*
 * queue.h
 * Queue implementation for Borrow Requests (FIFO)
 */

#ifndef QUEUE_H
#define QUEUE_H

#include "utils.h"

/* Borrow Request (Queue Node) */
typedef struct BorrowRequest {
    int queueNumber;
    char studentName[MAX_STRING];
    char studentId[50];
    char bookTitle[MAX_STRING];
    int bookId;
    char status[20];
    struct BorrowRequest *next;
} BorrowRequest;

/* Global queue pointers */
extern BorrowRequest *queueFront;
extern BorrowRequest *queueRear;
extern int queueCounter;

/* Function prototypes */
void enqueue(const char *studentName, const char *studentId, 
             const char *bookTitle, int bookId);
BorrowRequest* dequeue(void);
void displayQueue(void);
int getQueuePosition(const char *studentId);
void freeQueue(void);

#endif /* QUEUE_H */