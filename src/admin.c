/*
 * admin.c
 * Implementation of admin module functions
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "admin.h"
#include "book.h"
#include "queue.h"
#include "stack.h"
#include "history.h"
#include "utils.h"

void adminAddBook(void) {
    clearScreen();
    printf("============================================================\n");
    printf("                        ADD NEW BOOK\n");
    printf("============================================================\n");
    
    int id, year, quantity;
    char title[MAX_STRING], author[MAX_STRING], genre[MAX_STRING], isbn[50];
    float rating;
    
    printf("Enter Book ID: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid input!\n");
        while(getchar() != '\n');
        pressEnter();
        return;
    }
    while(getchar() != '\n');
    
    printf("Enter Title: ");
    if (fgets(title, MAX_STRING, stdin) == NULL) return;
    title[strcspn(title, "\n")] = '\0';
    
    printf("Enter Author: ");
    if (fgets(author, MAX_STRING, stdin) == NULL) return;
    author[strcspn(author, "\n")] = '\0';
    
    printf("Enter Genre: ");
    if (fgets(genre, MAX_STRING, stdin) == NULL) return;
    genre[strcspn(genre, "\n")] = '\0';
    
    printf("Enter ISBN: ");
    if (fgets(isbn, 50, stdin) == NULL) return;
    isbn[strcspn(isbn, "\n")] = '\0';
    
    printf("Enter Year: ");
    if (scanf("%d", &year) != 1) {
        while(getchar() != '\n');
        pressEnter();
        return;
    }
    
    printf("Enter Rating (0-5): ");
    if (scanf("%f", &rating) != 1) {
        while(getchar() != '\n');
        pressEnter();
        return;
    }
    
    printf("Enter Quantity: ");
    if (scanf("%d", &quantity) != 1) {
        while(getchar() != '\n');
        pressEnter();
        return;
    }
    
    Book *newBook = createBook(id, title, author, genre, isbn, year, rating, quantity);
    if (newBook) {
        addBook(&bookCatalog, newBook);
        printf("\nBook successfully added!\n");
    }
    pressEnter();
}

void adminRemoveBook(void) {
    clearScreen();
    printf("============================================================\n");
    printf("                        REMOVE BOOK\n");
    printf("============================================================\n");
    
    int id;
    printf("Enter Book ID to remove: ");
    if (scanf("%d", &id) != 1) {
        while(getchar() != '\n');
        pressEnter();
        return;
    }
    
    Book *book = searchBookById(id);
    if (book == NULL) {
        printf("Book not found!\n");
        pressEnter();
        return;
    }
    
    printf("------------------------------------------------------------\n");
    printf("Book: %s by %s (Qty: %d)\n", book->title, book->author, book->quantity);
    
    char confirm;
    printf("Delete? (Y/N): ");
    scanf(" %c", &confirm);
    
    if (confirm == 'Y' || confirm == 'y') {
        deleteBook(id);
    }
    pressEnter();
}

void adminProcessBorrowRequest(void) {
    clearScreen();
    printf("============================================================\n");
    printf("                  PROCESS BORROW REQUEST\n");
    printf("============================================================\n");
    
    if (queueFront == NULL) {
        printf("No pending requests.\n");
        pressEnter();
        return;
    }
    
    BorrowRequest *request = queueFront;
    Book *book = searchBookById(request->bookId);
    
    printf("Next in Queue:\n");
    printf("Queue #%03d: %s wants \"%s\"\n", 
           request->queueNumber, request->studentName, request->bookTitle);
    
    if (book) {
        printf("Current quantity: %d copies\n", book->quantity);
        if (book->quantity > 0) {
            printf("Status: Available\n");
        } else {
            printf("Status: OUT OF STOCK!\n");
        }
    } else {
        printf("Warning: Book not found in catalog!\n");
    }
    
    char confirm;
    printf("\nApprove? (Y/N): ");
    scanf(" %c", &confirm);
    
    if (confirm == 'Y' || confirm == 'y') {
        if (book && book->quantity > 0) {
            printf("\n--- PROCESSING ---\n");
            printf("Before: %d copies\n", book->quantity);
            
            book->quantity--;           /* DECREASE QUANTITY */
            book->borrowCount++;
            
            printf("After:  %d copies\n", book->quantity);
            printf("--- BOOK MARKED AS BORROWED ---\n");
            
            addBorrowHistory(request->studentId, request->studentName,
                           request->bookTitle, request->bookId);
            
            char dueDate[20];
            addDaysToDate(dueDate, 14);
            
            printf("\nApproved! Due: %s\n", dueDate);
            BorrowRequest *toFree = dequeue();
            free(toFree);
        } else {
            printf("\nCannot approve: Book unavailable!\n");
        }
    } else {
        printf("\nRequest rejected.\n");
    }
    pressEnter();
}

void adminDashboard(void) {
    clearScreen();
    printf("====================== SYSTEM STATISTICS ====================\n");
    
    int totalBooks = 0, availableBooks = 0;
    Book *temp = bookCatalog;
    while (temp != NULL) {
        totalBooks++;
        availableBooks += temp->quantity;
        temp = temp->next;
    }
    
    int queueLength = 0;
    BorrowRequest *qTemp = queueFront;
    while (qTemp != NULL) {
        queueLength++;
        qTemp = qTemp->next;
    }
    
    printf("Total Books in Catalog:  %d\n", totalBooks);
    printf("Books Available       :  %d\n", availableBooks);
    printf("Queue Length          :  %d\n\n", queueLength);
    
    printf("Most Borrowed Books:\n");
    temp = bookCatalog;
    int rank = 1;
    while (temp != NULL && rank <= 5) {
        printf("%d. %s (%d times)\n", rank++, temp->title, temp->borrowCount);
        temp = temp->next;
    }
    
    printf("==============================================================\n");
    pressEnter();
}

void adminMenu(void) {
    int choice;
    do {
        clearScreen();
        printf("============================================================\n");
        printf("                        ADMIN DASHBOARD\n");
        printf("============================================================\n");
        printf("[1]  Add New Book to Catalog\n");
        printf("[2]  Remove Book from Catalog\n");
        printf("[3]  Display All Books\n");
        printf("[4]  Display Borrowed Books\n");
        printf("[5]  Display Borrowing Queue\n");
        printf("[6]  Process Borrow Requests\n");
        printf("[7]  Display All Users\n");
        printf("[8]  View User's Return History\n");
        printf("[9]  View User Borrowing History\n");
        printf("[10] Log Out\n");
        printf("------------------------------------------------------------\n");
        printf("Enter choice: ");
        
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n');
            continue;
        }
        
        switch(choice) {
            case 1: adminAddBook(); break;
            case 2: adminRemoveBook(); break;
            case 3: displayAllBooks(); pressEnter(); break;
            case 4: displayBorrowedBooks(); pressEnter(); break;
            case 5: displayQueue(); pressEnter(); break;
            case 6: adminProcessBorrowRequest(); break;
            case 7: displayAllUsers(); pressEnter(); break;
            case 8: {
                char name[MAX_STRING];
                while(getchar() != '\n');
                printf("\nEnter student name: ");
                if (fgets(name, MAX_STRING, stdin) != NULL) {
                    name[strcspn(name, "\n")] = '\0';
                    displayReturnHistory(name);
                }
                pressEnter();
                break;
            }
            case 9: {
                char name[MAX_STRING];
                while(getchar() != '\n');
                printf("\nEnter student name: ");
                if (fgets(name, MAX_STRING, stdin) != NULL) {
                    name[strcspn(name, "\n")] = '\0';
                    displayBorrowHistory(name);
                }
                pressEnter();
                break;
            }
            case 10: 
                printf("\n============================================================\n");
                printf("                     YOU HAVE LOGGED OUT\n");
                printf("============================================================\n");
                printf("Returning to Main Menu...\n");
                pressEnter();
                break;
            default: printf("Invalid choice!\n"); pressEnter();
        }
    } while(choice != 10);
}

void displayBorrowedBooks(void) {
    clearScreen();
    printf("============================================================\n");
    printf("            CURRENTLY BORROWED BOOKS\n");
    printf("============================================================\n");
    printf("Student Name   | Book Title              | Borrow Date | Due Date\n");
    printf("------------------------------------------------------------\n");
    
    BorrowHistory *temp = historyList;
    int found = 0;
    
    while (temp != NULL) {
        if (!temp->returned) {
            /* Calculate due date (14 days from borrow) */
            printf("%-14s | %-23s | %-11s | +14 days\n", 
                   temp->studentName, temp->bookTitle, temp->borrowDate);
            found = 1;
        }
        temp = temp->next;
    }
    
    if (!found) {
        printf("No books currently borrowed.\n");
    }
    printf("============================================================\n");
}

void displayAllUsers(void) {
    clearScreen();
    printf("============================================================\n");
    printf("                   ALL USERS\n");
    printf("============================================================\n");
    printf("ID       | Username       | Role\n");
    printf("------------------------------------------------------------\n");
    
    FILE *file = fopen("users.csv", "r");
    if (!file) {
        printf("Error: Cannot open users.csv\n");
        return;
    }
    
    char line[512];
    int isFirstLine = 1;
    int count = 0;
    
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = '\0';
        
        if (isFirstLine) {
            isFirstLine = 0;
            continue;
        }
        
        if (line[0] == '\0') continue;
        
        char temp[512];
        strncpy(temp, line, sizeof(temp)-1);
        temp[sizeof(temp)-1] = '\0';
        
        char *id = strtok(temp, ",");
        char *user = strtok(NULL, ",");
        char *pass = strtok(NULL, ",");
        char *role = strtok(NULL, ",");
        
        (void)pass; /* Unused - don't display password */
        
        if (!id || !user || !role) continue;
        
        trimWhitespace(id);
        trimWhitespace(user);
        trimWhitespace(role);
        
        printf("%-8s | %-14s | %s\n", id, user, role);
        count++;
    }
    
    fclose(file);
    printf("------------------------------------------------------------\n");
    printf("Total Users: %d\n", count);
    printf("============================================================\n");
}