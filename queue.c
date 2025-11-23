/*
 * queue.c
 * Implementation of Queue operations for Borrow Requests
 * Follows FIFO (First In First Out) principle
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

/* Global queue variables */
BorrowRequest *queueFront = NULL;
BorrowRequest *queueRear = NULL;
int queueCounter = 1;

/*
 * enqueue()
 * Parameters: student name, student ID, book title, book ID
 * Returns: void
 * Purpose: Adds a new borrow request to the end of the queue
 * Complexity: O(1)
 */
void enqueue(const char *studentName, const char *studentId, 
             const char *bookTitle, int bookId) {
    BorrowRequest *newRequest = (BorrowRequest*)malloc(sizeof(BorrowRequest));
    if (!newRequest) {
        printf("Memory allocation failed!\n");
        return;
    }
    
    newRequest->queueNumber = queueCounter++;
    strncpy(newRequest->studentName, studentName, MAX_STRING - 1);
    newRequest->studentName[MAX_STRING - 1] = '\0';
    strncpy(newRequest->studentId, studentId, 49);
    newRequest->studentId[49] = '\0';
    strncpy(newRequest->bookTitle, bookTitle, MAX_STRING - 1);
    newRequest->bookTitle[MAX_STRING - 1] = '\0';
    newRequest->bookId = bookId;
    strcpy(newRequest->status, "Pending");
    newRequest->next = NULL;
    
    if (queueRear == NULL) {
        queueFront = queueRear = newRequest;
    } else {
        queueRear->next = newRequest;
        queueRear = newRequest;
    }
    
    printf("\nYou have been added to the borrowing queue.\n");
    printf("Your Queue Number: %03d\n", newRequest->queueNumber);
}

/*
 * dequeue()
 * Parameters: none
 * Returns: pointer to dequeued request (caller must free)
 * Purpose: Removes and returns the front request from the queue
 * Complexity: O(1)
 */
BorrowRequest* dequeue(void) {
    if (queueFront == NULL) {
        return NULL;
    }
    
    BorrowRequest *temp = queueFront;
    queueFront = queueFront->next;
    
    if (queueFront == NULL) {
        queueRear = NULL;
    }
    
    return temp;
}

void displayQueue(void) {
    clearScreen();
    printf("========================= BORROW QUEUE ======================\n");
    printf("Queue# | Student Name     | Book Title              | Status\n");
    printf("------------------------------------------------------------\n");
    
    if (queueFront == NULL) {
        printf("Queue is empty.\n");
    } else {
        BorrowRequest *temp = queueFront;
        while (temp != NULL) {
            printf("%03d    | %-16s | %-23s | %s\n", 
                   temp->queueNumber, temp->studentName, 
                   temp->bookTitle, temp->status);
            temp = temp->next;
        }
    }
    printf("==============================================================\n");
}

int getQueuePosition(const char *studentId) {
    BorrowRequest *temp = queueFront;
    int position = 1;
    
    while (temp != NULL) {
        if (strcmp(temp->studentId, studentId) == 0) {
            return position;
        }
        position++;
        temp = temp->next;
    }
    return -1;
}

void freeQueue(void) {
    BorrowRequest *temp;
    while (queueFront != NULL) {
        temp = queueFront;
        queueFront = queueFront->next;
        free(temp);
    }
    queueRear = NULL;
}