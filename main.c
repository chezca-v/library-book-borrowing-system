/*
 * main.c
 * C the Library - Main Program Entry Point
 * "A C console-based system that helps you see what's borrowed"
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
    // Blue borders, Cyan Title, Green subtitle for the "C" emphasis
    printf("%s============================================================%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
    printf("              %s%sWELCOME TO C THE LIBRARY%s\n", ANSI_COLOR_CYAN, ANSI_COLOR_BOLD, ANSI_COLOR_RESET);
    printf("%s============================================================%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
    printf("      A %sC%s console-based system that helps you\n", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
    printf("              see what's borrowed\n");
    printf("%s============================================================%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
}

/* Comparison function for sorting books by borrowCount (descending) */
static int cmpBorrowCountMain(const void *a, const void *b) {
    Book *const *pa = (Book *const *)a;
    Book *const *pb = (Book *const *)b;
    return (*pb)->borrowCount - (*pa)->borrowCount;
}

void displayDashboardStats(void) {
    printf("%s============================================================%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
    printf("              %sDASHBOARD / STATISTICS%s\n", ANSI_COLOR_CYAN, ANSI_COLOR_RESET);
    printf("%s============================================================%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
    
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
    
    // Highlighting the numbers in GREEN to make them pop
    printf("Total Books Available :  %s%d%s\n", ANSI_COLOR_GREEN, availableBooks, ANSI_COLOR_RESET);
    printf("Total Books Borrowed  :  %s%d%s\n", ANSI_COLOR_YELLOW, borrowedBooks, ANSI_COLOR_RESET);
    printf("Active Borrowers      :  %s%d%s\n\n", ANSI_COLOR_CYAN, activeBorrowers, ANSI_COLOR_RESET);
    
    /* Show top 5 most popular books */
    printf("%sTop 5 Most Popular Books (Popularity Counter):%s\n", ANSI_COLOR_MAGENTA, ANSI_COLOR_RESET);
    printf("%s------------------------------------------------------------%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
    
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
                // Highlight the rank number and the borrow count
                printf("%s%d.%s %-40s (Borrowed %s%d%s times)\n", 
                       ANSI_COLOR_YELLOW, r+1, ANSI_COLOR_RESET, 
                       arr[r]->title, 
                       ANSI_COLOR_CYAN, arr[r]->borrowCount, ANSI_COLOR_RESET);
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
    
    printf("%s============================================================%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
    fflush(stdout);
}

void displayExitScreen(void) {
    clearScreen();
    printf("%s============================================================%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
    printf("          %sTHANK YOU FOR USING C THE LIBRARY%s\n", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
    printf("%s------------------------------------------------------------%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
    printf("           \"See what's borrowed, in C.\"\n");
    printf("%s============================================================%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
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

    /* Print build stamp */
    printf("BUILD: compiled on %s %s\n", __DATE__, __TIME__);
    fflush(stdout);

    /* Load all data */
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
        printf("%s============================================================%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
        printf("                      %s%sMAIN MENU%s\n", ANSI_COLOR_CYAN, ANSI_COLOR_BOLD, ANSI_COLOR_RESET);
        printf("%s============================================================%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
        // Using Yellow brackets for options to guide the user
        printf("%s[1]%s Log in as Admin\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
        printf("%s[2]%s Log in as Student\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
        printf("%s[3]%s Exit Program\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
        printf("%s------------------------------------------------------------%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
        printf("%sEnter choice: %s", ANSI_COLOR_BOLD, ANSI_COLOR_RESET);
        
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n');
            printf("%sInvalid input!%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
            pressEnter();
            continue;
        }
        printf("%s============================================================%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
        
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
                printf("%sInvalid choice! Please try again.%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
                pressEnter();
        }
    } while(choice != 3);
    
    /* Save all data */
    saveBooksToCSV();
    saveQueueToCSV();
    saveHistoryToCSV();
    
    /* Free memory */
    freeMemory();
    
    return 0;
}