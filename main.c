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
    printf("                 LIBRARY BORROWING SYSTEM\n");
    printf("============================================================\n");
    printf("                   Welcome to the Library!\n");
    printf("                A Simple Console-Based System\n");
    printf("------------------------------------------------------------\n");
    printf("                    Press ENTER to continue\n");
    printf("============================================================\n");
    getchar();
}

void displayExitScreen(void) {
    clearScreen();
    printf("============================================================\n");
    printf("                THANK YOU FOR USING C THE LIBRARY\n");
    printf("------------------------------------------------------------\n");
    printf("                \"See what's borrowed, in C.\"\n");
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
    
    /* Load initial data */
    loadBooksFromCSV();
    
    /* Display welcome screen */
    displayWelcomeScreen();
    
    /* Main menu loop */
    do {
        clearScreen();
        printf("============================================================\n");
        printf("                       USER LOGIN\n");
        printf("============================================================\n");
        printf("[1] Login as Admin\n");
        printf("[2] Login as Student\n");
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
    
    /* Save data before exit */
    saveBooksToCSV();
    
    /* Free allocated memory */
    freeMemory();
    
    return 0;
}