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
    printf("Queue #%03d: %s wants \"%s\"\n", 
           request->queueNumber, request->studentName, request->bookTitle);
    
    char confirm;
    printf("\nApprove? (Y/N): ");
    scanf(" %c", &confirm);
    
    if (confirm == 'Y' || confirm == 'y') {
        Book *book = searchBookById(request->bookId);
        if (book && book->quantity > 0) {
            book->quantity--;
            book->borrowCount++;
            
            addBorrowHistory(request->studentId, request->studentName,
                           request->bookTitle, request->bookId);
            
            char dueDate[20];
            addDaysToDate(dueDate, 14);
            
            printf("\nApproved! Due: %s\n", dueDate);
            BorrowRequest *toFree = dequeue();
            free(toFree);
        } else {
            printf("Book unavailable!\n");
        }
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
        printf("[1] Add New Book\n");
        printf("[2] Remove Book\n");
        printf("[3] Display All Books\n");
        printf("[4] Display Borrowing Queue\n");
        printf("[5] Process Borrow Request\n");
        printf("[6] View User Return History\n");
        printf("[7] View User Borrow History\n");
        printf("[8] Dashboard Statistics\n");
        printf("[9] Logout\n");
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
            case 4: displayQueue(); pressEnter(); break;
            case 5: adminProcessBorrowRequest(); break;
            case 6: {
                char name[MAX_STRING];
                while(getchar() != '\n');
                printf("Student name: ");
                if (fgets(name, MAX_STRING, stdin) != NULL) {
                    name[strcspn(name, "\n")] = '\0';
                    displayReturnHistory(name);
                }
                pressEnter();
                break;
            }
            case 7: {
                char name[MAX_STRING];
                while(getchar() != '\n');
                printf("Student name: ");
                if (fgets(name, MAX_STRING, stdin) != NULL) {
                    name[strcspn(name, "\n")] = '\0';
                    displayBorrowHistory(name);
                }
                pressEnter();
                break;
            }
            case 8: adminDashboard(); break;
            case 9: printf("\nLogging out...\n"); break;
            default: printf("Invalid!\n"); pressEnter();
        }
    } while(choice != 9);
}