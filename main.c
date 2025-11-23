/*
 * main.c
 * C the Library - Main Program Entry Point
 * "A C console-based system that helps you see what's borrowed"
 * 
 * This is a modular library management system demonstrating:
 * - Linked Lists (Book Catalog)
 * - Queues (Borrow Requests) 
 * - Stacks (Return History)
 * - Dynamic Memory Management
 * - Recursion
 * - CSV File I/O
 * 
 * To compile: make
 * Or manually: gcc -std=c99 -Wall -Wextra *.c -o library
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "utils.h"
#include "book.h"
#include "queue.h"
#include "stack.h"
#include "history.h"
#include "fileio.h"
#include "admin.h"
#include "student.h"
#include "auth.h"

void displayWelcomeScreen(void) {
    clearScreen();
    printf("============================================================\n");
    printf("              WELCOME TO C THE LIBRARY\n");
    printf("============================================================\n");
    printf("      A C console-based system that helps you\n");
    printf("              see what's borrowed\n");
    printf("============================================================\n");
}

/* Comparison function for sorting books by borrowCount (descending) */
static int cmpBorrowCountMain(const void *a, const void *b) {
    Book *const *pa = (Book *const *)a;
    Book *const *pb = (Book *const *)b;
    return (*pb)->borrowCount - (*pa)->borrowCount;
}

void displayDashboardStats(void) {
    printf("============================================================\n");
    printf("              DASHBOARD / STATISTICS\n");
    printf("============================================================\n");
    
    /* Count total books and available */
    int totalBooks = 0, availableBooks = 0;
    Book *temp = bookCatalog;
    while (temp != NULL) {
        totalBooks++;
        availableBooks += temp->quantity;
        temp = temp->next;
    }
    
    /* Count borrowed books */
    int borrowedBooks = 0;
    BorrowHistory *hist = historyList;
    while (hist != NULL) {
        if (!hist->returned) borrowedBooks++;
        hist = hist->next;
    }
    
    /* Count active borrowers */
    int activeBorrowers = 0;
    char counted[100][50];
    int countedIndex = 0;
    hist = historyList;
    
    while (hist != NULL && countedIndex < 100) {
        if (!hist->returned) {
            int alreadyCounted = 0;
            for (int i = 0; i < countedIndex; i++) {
                if (strcmp(counted[i], hist->studentId) == 0) {
                    alreadyCounted = 1;
                    break;
                }
            }
            if (!alreadyCounted) {
                strncpy(counted[countedIndex], hist->studentId, 49);
                counted[countedIndex][49] = '\0';
                countedIndex++;
                activeBorrowers++;
            }
        }
        hist = hist->next;
    }
    
    printf("Total Books Available :  %d\n", availableBooks);
    printf("Total Books Borrowed  :  %d\n", borrowedBooks);
    printf("Active Borrowers      :  %d\n\n", activeBorrowers);
    
    /* Show top 5 most popular books */
    printf("Top 5 Most Popular Books (Popularity Counter):\n");
    printf("------------------------------------------------------------\n");
    
    /* Collect books into array and sort by borrowCount */
    int bookCount = 0;
    Book *iter = bookCatalog;
    while (iter) { bookCount++; iter = iter->next; }
    
    if (bookCount > 0) {
        Book **arr = (Book**)malloc(sizeof(Book*) * bookCount);
        if (arr) {
            int i = 0;
            iter = bookCatalog;
            while (iter) { arr[i++] = iter; iter = iter->next; }
            
            /* Sort descending by borrowCount */
            qsort(arr, bookCount, sizeof(Book*), cmpBorrowCountMain);
            
            int display = bookCount < 5 ? bookCount : 5;
            for (int r = 0; r < display; r++) {
                printf("%d. %-40s (Borrowed %d times)\n", 
                       r+1, arr[r]->title, arr[r]->borrowCount);
            }
            free(arr);
        } else {
            /* Fallback if malloc fails */
            temp = bookCatalog;
            int rank = 1;
            while (temp != NULL && rank <= 5) {
                printf("%d. %-40s (Borrowed %d times)\n", 
                       rank++, temp->title, temp->borrowCount);
                temp = temp->next;
            }
        }
    } else {
        printf("No books available.\n");
    }
    
    printf("============================================================\n");
    fflush(stdout);
}

void displayExitScreen(void) {
    clearScreen();
    printf("============================================================\n");
    printf("           THANK YOU FOR USING C THE LIBRARY\n");
    printf("------------------------------------------------------------\n");
    printf("           \"See what's borrowed, in C.\"\n");
    printf("============================================================\n");
}

void freeMemory(void) {
    freeBookList(&bookCatalog);
    freeQueue();
    freeStack();
    freeHistoryList();
}

int main(void) {
    int choice;
    char studentId[50], studentName[MAX_STRING];

    /* Print build stamp to verify binary was rebuilt */
    printf("BUILD: compiled on %s %s\n", __DATE__, __TIME__);
    fflush(stdout);

    /* Load all data from CSV files */
    loadBooksFromCSV();
    loadQueueFromCSV();
    loadHistoryFromCSV();
    
    /* Display welcome screen */
    displayWelcomeScreen();
    
    /* Main menu loop */
    int firstLoop = 1;
    do {
        if (!firstLoop) {
            clearScreen();
        } else {
            firstLoop = 0;
        }

        /* Show Dashboard Statistics */
        displayDashboardStats();
        
        printf("\n");
        printf("============================================================\n");
        printf("                     MAIN MENU\n");
        printf("============================================================\n");
        printf("[1] Log in as Admin\n");
        printf("[2] Log in as Student\n");
        printf("[3] Exit Program\n");
        printf("------------------------------------------------------------\n");
        printf("Enter choice: ");
        
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n');
            printf("Invalid input!\n");
            pressEnter();
            continue;
        }
        printf("============================================================\n");
        
        switch(choice) {
            case 1:
                if (adminLogin()) {
                    adminMenu();
                }
                break;
            case 2:
                if (studentLogin(studentId, studentName)) {
                    studentMenu(studentId, studentName);
                }
                break;
            case 3:
                displayExitScreen();
                break;
            default:
                printf("Invalid choice! Please try again.\n");
                pressEnter();
        }
    } while(choice != 3);
    
    /* Save all data before exit */
    saveBooksToCSV();
    saveQueueToCSV();
    saveHistoryToCSV();
    
    /* Free allocated memory */
    freeMemory();
    
    return 0;
}