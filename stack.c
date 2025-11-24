/*
 * stack.c
 * Implementation of Stack operations for Return History
 * Follows LIFO (Last In First Out) principle
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include "history.h"  // Need this to access historyList

/* Global stack pointer */
ReturnRecord *returnStack = NULL;

/*
 * pushReturn()
 * Parameters: student name, book title, book ID, rating
 * Returns: void
 * Purpose: Pushes a new return record onto the stack
 * Complexity: O(1)
 */
void pushReturn(const char *studentName, const char *bookTitle, 
                int bookId, int rating) {
    ReturnRecord *newRecord = (ReturnRecord*)malloc(sizeof(ReturnRecord));
    if (!newRecord) {
        printf("Memory allocation failed!\n");
        return;
    }
    
    strncpy(newRecord->studentName, studentName, MAX_STRING - 1);
    newRecord->studentName[MAX_STRING - 1] = '\0';
    strncpy(newRecord->bookTitle, bookTitle, MAX_STRING - 1);
    newRecord->bookTitle[MAX_STRING - 1] = '\0';
    newRecord->bookId = bookId;
    newRecord->rating = rating;
    getCurrentDate(newRecord->dateReturned);
    
    newRecord->next = returnStack;
    returnStack = newRecord;
}

/*
 * popReturn()
 * Parameters: none
 * Returns: pointer to popped record (caller must free)
 * Purpose: Removes and returns the top record from the stack
 * Complexity: O(1)
 */
ReturnRecord* popReturn(void) {
    if (returnStack == NULL) {
        return NULL;
    }
    
    ReturnRecord *temp = returnStack;
    returnStack = returnStack->next;
    return temp;
}

/*
 * peekReturn()
 * Parameters: none
 * Returns: pointer to top record (without removing it)
 * Purpose: Views the top record without modifying the stack
 * Complexity: O(1)
 */
ReturnRecord* peekReturn(void) {
    return returnStack;
}

void displayReturnHistory(const char *studentName) {
    clearScreen();
    printf("======================== RETURN HISTORY ======================\n");
    printf("Student: %s\n", studentName);
    printf("------------------------------------------------------------\n");
    printf("Book Title                | Borrow Date | Return Date\n");
    printf("------------------------------------------------------------\n");
    
    /* Read from historyList (persistent data) instead of returnStack */
    BorrowHistory *temp = historyList;
    int found = 0;
    
    while (temp != NULL) {
        /* Check if matches student name AND book was returned */
        if (strcmp(temp->studentName, studentName) == 0 && temp->returned) {
            printf("%-25s | %-11s | %s\n", 
                   temp->bookTitle, temp->borrowDate, temp->returnDate);
            found = 1;
        }
        temp = temp->next;
    }
    
    if (!found) {
        printf("No return history found.\n");
    }
    printf("==============================================================\n");
}

void freeStack(void) {
    ReturnRecord *temp;
    while (returnStack != NULL) {
        temp = returnStack;
        returnStack = returnStack->next;
        free(temp);
    }
}