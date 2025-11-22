/*
 * history.c
 * Implementation of Borrow History tracking
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "history.h"

/* Global history list */
BorrowHistory *historyList = NULL;

void addBorrowHistory(const char *studentId, const char *studentName,
                      const char *bookTitle, int bookId) {
    BorrowHistory *newRecord = (BorrowHistory*)malloc(sizeof(BorrowHistory));
    if (!newRecord) {
        printf("Memory allocation failed!\n");
        return;
    }
    
    strncpy(newRecord->studentId, studentId, 49);
    newRecord->studentId[49] = '\0';
    strncpy(newRecord->studentName, studentName, MAX_STRING - 1);
    newRecord->studentName[MAX_STRING - 1] = '\0';
    strncpy(newRecord->bookTitle, bookTitle, MAX_STRING - 1);
    newRecord->bookTitle[MAX_STRING - 1] = '\0';
    newRecord->bookId = bookId;
    getCurrentDate(newRecord->borrowDate);
    strcpy(newRecord->returnDate, "");
    newRecord->returned = 0;
    newRecord->next = historyList;
    historyList = newRecord;
}

void markAsReturned(const char *studentId, int bookId) {
    BorrowHistory *temp = historyList;
    while (temp != NULL) {
        if (strcmp(temp->studentId, studentId) == 0 && 
            temp->bookId == bookId && temp->returned == 0) {
            getCurrentDate(temp->returnDate);
            temp->returned = 1;
            break;
        }
        temp = temp->next;
    }
}

void displayBorrowHistory(const char *studentName) {
    clearScreen();
    printf("====================== BORROWING HISTORY =====================\n");
    printf("Student: %s\n", studentName);
    printf("------------------------------------------------------------\n");
    printf("Book Title                | Borrowed On | Returned On\n");
    printf("------------------------------------------------------------\n");
    
    BorrowHistory *temp = historyList;
    int found = 0;
    
    while (temp != NULL) {
        if (strcmp(temp->studentName, studentName) == 0) {
            printf("%-25s | %-11s | %s\n", 
                   temp->bookTitle, temp->borrowDate,
                   temp->returned ? temp->returnDate : "Not yet");
            found = 1;
        }
        temp = temp->next;
    }
    
    if (!found) {
        printf("No borrowing history found.\n");
    }
    printf("==============================================================\n");
}

int getCurrentBorrowedCount(const char *studentId) {
    BorrowHistory *temp = historyList;
    int count = 0;
    
    while (temp != NULL) {
        if (strcmp(temp->studentId, studentId) == 0 && !temp->returned) {
            count++;
        }
        temp = temp->next;
    }
    return count;
}

void freeHistoryList(void) {
    BorrowHistory *temp;
    while (historyList != NULL) {
        temp = historyList;
        historyList = historyList->next;
        free(temp);
    }
}