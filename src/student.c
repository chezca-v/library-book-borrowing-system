/*
 * student.c
 * Implementation of student module functions
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student.h"
#include "book.h"
#include "queue.h"
#include "stack.h"
#include "history.h"
#include "utils.h"

void studentSearchBook(void) {
    clearScreen();
    printf("============================================================\n");
    printf("                    SEARCH BOOK\n");
    printf("============================================================\n");
    
    char query[MAX_STRING];
    while(getchar() != '\n');
    printf("Search (title/author): ");
    if (fgets(query, MAX_STRING, stdin) == NULL) return;
    query[strcspn(query, "\n")] = '\0';
    
    Book *result = searchBookByTitleRecursive(bookCatalog, query);
    
    if (result) {
        printf("\nFound:\n");
        printf("ID: %d\n", result->id);
        printf("Title: %s\n", result->title);
        printf("Author: %s\n", result->author);
        printf("Rating: ");
        printStars((int)result->rating);
        printf("\nAvailable: %d copies\n", result->quantity);
    } else {
        printf("\nNo matches found.\n");
    }
    
    printf("============================================================\n");
    pressEnter();
}

void studentRequestBorrow(const char *studentId, const char *studentName) {
    clearScreen();
    printf("============================================================\n");
    printf("                  BORROW REQUEST\n");
    printf("============================================================\n");
    
    int bookId;
    printf("Enter Book ID: ");
    if (scanf("%d", &bookId) != 1) {
        while(getchar() != '\n');
        pressEnter();
        return;
    }
    
    Book *book = searchBookById(bookId);
    if (book == NULL) {
        printf("Book not found!\n");
        pressEnter();
        return;
    }
    
    int currentBorrowed = getCurrentBorrowedCount(studentId);
    if (currentBorrowed >= MAX_BOOKS_PER_STUDENT) {
        printf("You've reached the %d-book limit!\n", MAX_BOOKS_PER_STUDENT);
        pressEnter();
        return;
    }
    
    enqueue(studentName, studentId, book->title, bookId);
    printf("============================================================\n");
    pressEnter();
}

void studentReturnBook(const char *studentId, const char *studentName) {
    clearScreen();
    printf("============================================================\n");
    printf("                        RETURN BOOK\n");
    printf("============================================================\n");
    
    int bookId, rating;
    printf("Book ID: ");
    if (scanf("%d", &bookId) != 1) {
        while(getchar() != '\n');
        pressEnter();
        return;
    }
    
    Book *book = searchBookById(bookId);
    if (book == NULL) {
        printf("Book not found!\n");
        pressEnter();
        return;
    }
    
    printf("Rate (1-5): ");
    if (scanf("%d", &rating) != 1 || rating < 1 || rating > 5) {
        printf("Invalid rating!\n");
        while(getchar() != '\n');
        pressEnter();
        return;
    }
    
    book->quantity++;
    markAsReturned(studentId, bookId);
    pushReturn(studentName, book->title, bookId, rating);
    
    printf("\nBook returned successfully!\n");
    printf("============================================================\n");
    pressEnter();
}

void studentMenu(const char *studentId, const char *studentName) {
    int choice;
    do {
        clearScreen();
        printf("============================================================\n");
        printf("                   STUDENT MENU - %s\n", studentName);
        printf("============================================================\n");
        printf("[1] Browse Books (Paginated)\n");
        printf("[2] Search Book\n");
        printf("[3] Filter by Genre\n");
        printf("[4] View Top Rated Books\n");
        printf("[5] Request to Borrow\n");
        printf("[6] View Queue Position\n");
        printf("[7] Return Book\n");
        printf("[8] View My History\n");
        printf("[9] Logout\n");
        printf("------------------------------------------------------------\n");
        printf("Choice: ");
        
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n');
            continue;
        }
        
        switch(choice) {
            case 1: displayAllBooks(); pressEnter(); break;
            case 2: studentSearchBook(); break;
            case 3: {
                char genre[50];
                while(getchar() != '\n');
                printf("Enter genre: ");
                if (fgets(genre, 50, stdin) != NULL) {
                    genre[strcspn(genre, "\n")] = '\0';
                    displayBooksByGenre(genre);
                }
                pressEnter();
                break;
            }
            case 4: displayTopRatedBooks(); pressEnter(); break;
            case 5: studentRequestBorrow(studentId, studentName); break;
            case 6: {
                int pos = getQueuePosition(studentId);
                printf("\nQueue position: %d\n", pos > 0 ? pos : 0);
                pressEnter();
                break;
            }
            case 7: studentReturnBook(studentId, studentName); break;
            case 8: displayBorrowHistory(studentName); pressEnter(); break;
            case 9: printf("\nLogging out...\n"); break;
            default: printf("Invalid!\n"); pressEnter();
        }
    } while(choice != 9);
}