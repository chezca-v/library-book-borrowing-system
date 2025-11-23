<<<<<<< HEAD
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

void displayWelcomeScreen(void)
{
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

void displayExitScreen(void)
{
    clearScreen();
    printf("============================================================\n");
    printf("                THANK YOU FOR USING C THE LIBRARY\n");
    printf("------------------------------------------------------------\n");
    printf("                \"See what's borrowed, in C.\"\n");
    printf("============================================================\n");
}

void freeMemory(void)
{
    freeBookList(&bookCatalog);
    freeQueue();
    freeStack();
    freeHistoryList();
}

int main(void)
{
    int choice;
    char studentId[50], studentName[MAX_STRING];

    /* Load all data from CSV files */
    loadBooksFromCSV();
    loadQueueFromCSV();
    loadHistoryFromCSV();

    /* Display welcome screen */
    displayWelcomeScreen();

    /* Main menu loop */
    do
    {
        clearScreen();
        printf("============================================================\n");
        printf("                       USER LOGIN\n");
        printf("============================================================\n");
        printf("[1] Login as Admin\n");
        printf("[2] Login as Student\n");
        printf("[3] Exit Program\n");
        printf("------------------------------------------------------------\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n')
                ;
            printf("Invalid input!\n");
            pressEnter();
            continue;
        }
        printf("============================================================\n");

        switch (choice)
        {
        case 1:
            if (adminLogin())
            {
                adminMenu();
            }
            break;
        case 2:
            if (studentLogin(studentId, studentName))
            {
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
    } while (choice != 3);

    /* Save all data before exit */
    saveBooksToCSV();
    saveQueueToCSV();
    saveHistoryToCSV();

    /* Free allocated memory */
    freeMemory();

=======
<<<<<<< HEAD
# include <stdio.h>

int main() {
    printf("Jograt\n");
=======
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
    
    /* Load all data from CSV files */
    loadBooksFromCSV();
    loadQueueFromCSV();
    loadHistoryFromCSV();
    
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
    
    /* Save all data before exit */
    saveBooksToCSV();
    saveQueueToCSV();
    saveHistoryToCSV();
    
    /* Free allocated memory */
    freeMemory();
    
>>>>>>> 70b1755222154172723b14d495a6c1e50c8b63d2
>>>>>>> 321f30469ac5bb8030c281bf5b7235d1d59c5319
    return 0;
}