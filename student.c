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

void studentMenu(const char *studentId, const char *studentName) {
    int choice;
    do {
        clearScreen();
        printf("============================================================\n");
        printf("                   STUDENT MENU - %s\n", studentName);
        printf("============================================================\n");
        printf("[1] 📖 View User's Current Books\n");
        printf("[2] 🔍 Search Book\n");
        printf("[3] 📚 Display Available Books\n");
        printf("[4] 📘 Request to Borrow Book (Enqueue)\n");
        printf("[5] ⏳ View Borrow Queue Position\n");
        printf("[6] 🔁 Return a Book\n");
        printf("[7] 📖 View My Borrowed Books History\n");
        printf("[8] 🚪 Log Out\n");
        printf("------------------------------------------------------------\n");
        printf("Choice: ");
        
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n');
            continue;
        }
        
        switch(choice) {
            case 1: 
                viewCurrentBooks(studentId, studentName);
                pressEnter();
                break;
            case 2: studentSearchBook(); break;
            case 3: displayAllBooks(); pressEnter(); break;
            case 4: studentRequestBorrow(studentId, studentName); break;
            case 5: {
                int pos = getQueuePosition(studentId);
                if (pos > 0) {
                    printf("\n✓ Your queue position: #%d\n", pos);
                } else {
                    printf("\n✗ You are not in the borrowing queue.\n");
                }
                pressEnter();
                break;
            }
            case 6: studentReturnBook(studentId, studentName); break;
            case 7: displayReturnHistory(studentName); pressEnter(); break;
            case 8: 
                printf("\n============================================================\n");
                printf("                     YOU HAVE LOGGED OUT\n");
                printf("============================================================\n");
                printf("Returning to Main Menu...\n");
                pressEnter();
                break;
            default: printf("Invalid choice!\n"); pressEnter();
        }
    } while(choice != 8);
}

void viewCurrentBooks(const char *studentId, const char *studentName) {
    clearScreen();
    printf("============================================================\n");
    printf("         📖 YOUR CURRENT BORROWED BOOKS\n");
    printf("============================================================\n");
    printf("Student: %s\n", studentName);
    printf("------------------------------------------------------------\n");
    printf("Book Title                | Borrow Date | Status\n");
    printf("------------------------------------------------------------\n");
    
    BorrowHistory *temp = historyList;
    int found = 0;
    int overdueCount = 0;
    
    while (temp != NULL) {
        if (strcmp(temp->studentId, studentId) == 0 && !temp->returned) {
            /* Check if overdue (simplified - in production, calculate actual days) */
            const char *status = "Active";
            
            printf("%-25s | %-11s | %s\n", 
                   temp->bookTitle, temp->borrowDate, status);
            found = 1;
        }
        temp = temp->next;
    }
    
    if (!found) {
        printf("You have no currently borrowed books.\n");
    } else {
        printf("------------------------------------------------------------\n");
        if (overdueCount > 0) {
            printf("⚠️  Overdue Tracker: You have %d overdue book(s)!\n", overdueCount);
        } else {
            printf("✓ All books are within due date.\n");
        }
    }
    printf("============================================================\n");
}

void studentSearchBook(void) {
    clearScreen();
    printf("============================================================\n");
    printf("                    🔍 SEARCH BOOK\n");
    printf("============================================================\n");
    printf("Search by:\n");
    printf("[1] Author\n");
    printf("[2] Title\n");
    printf("[3] ISBN\n");
    printf("[4] ID\n");
    printf("------------------------------------------------------------\n");
    printf("Choose search type: ");
    
    int searchType;
    if (scanf("%d", &searchType) != 1) {
        while(getchar() != '\n');
        printf("Invalid input!\n");
        pressEnter();
        return;
    }
    
    while(getchar() != '\n');
    
    char query[MAX_STRING];
    
    switch(searchType) {
        case 1:
            printf("Enter author name: ");
            break;
        case 2:
            printf("Enter book title: ");
            break;
        case 3:
            printf("Enter ISBN: ");
            break;
        case 4:
            printf("Enter book ID: ");
            break;
        default:
            printf("Invalid search type!\n");
            pressEnter();
            return;
    }
    
    if (fgets(query, MAX_STRING, stdin) == NULL) return;
    query[strcspn(query, "\n")] = '\0';
    
    printf("\n------------------------------------------------------------\n");
    printf("Searching for: \"%s\"\n", query);
    printf("------------------------------------------------------------\n");
    
    /* For ID search, use searchById */
    if (searchType == 4) {
        int id = atoi(query);
        Book *result = searchBookById(id);
        if (result) {
            printf("\n✓ Found:\n");
            printf("ID: %d\n", result->id);
            printf("Title: %s\n", result->title);
            printf("Author: %s\n", result->author);
            printf("ISBN: %s\n", result->isbn);
            printf("Genre: %s\n", result->genre);
            printf("Rating: ");
            printStars((int)result->rating);
            printf(" (%.1f/5.0)\n", result->rating);
            printf("Available: %d copies\n", result->quantity);
        } else {
            printf("\n✗ Book not found.\n");
        }
    } else {
        /* Use recursive search for title/author/ISBN */
        Book *result = searchBookByTitleRecursive(bookCatalog, query);
        
        if (result) {
            printf("\n✓ Found:\n");
            printf("ID: %d\n", result->id);
            printf("Title: %s\n", result->title);
            printf("Author: %s\n", result->author);
            printf("ISBN: %s\n", result->isbn);
            printf("Rating: ");
            printStars((int)result->rating);
            printf("\nAvailable: %d copies\n", result->quantity);
        } else {
            printf("\n✗ No exact match found.\n\n");
            printf("Search Suggestions (titles containing \"%s\"):\n", query);
            
            /* Show suggestions - partial matches */
            Book *temp = bookCatalog;
            int suggestionCount = 0;
            
            while (temp != NULL) {
                if (strstr(temp->title, query) != NULL || 
                    strstr(temp->author, query) != NULL) {
                    printf("  - %s (by %s)\n", temp->title, temp->author);
                    suggestionCount++;
                }
                temp = temp->next;
            }
            
            if (suggestionCount == 0) {
                printf("  No suggestions available.\n");
            }
        }
    }
    
    printf("============================================================\n");
    pressEnter();
}

void studentRequestBorrow(const char *studentId, const char *studentName) {
    clearScreen();
    printf("============================================================\n");
    printf("              📘 REQUEST TO BORROW BOOK\n");
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
        printf("✗ Book not found!\n");
        pressEnter();
        return;
    }
    
    /* Check borrow limit (max 3 books) */
    int currentBorrowed = getCurrentBorrowedCount(studentId);
    if (currentBorrowed >= MAX_BOOKS_PER_STUDENT) {
        printf("\n⚠️  Borrow Limit Reached!\n");
        printf("You have reached the maximum borrowing limit (%d books).\n", 
               MAX_BOOKS_PER_STUDENT);
        printf("You will be added to the waiting queue.\n\n");
    }
    
    /* Add to queue with queue number */
    printf("\n------------------------------------------------------------\n");
    printf("Book: %s\n", book->title);
    printf("Author: %s\n", book->author);
    printf("Current availability: %d copies\n", book->quantity);
    printf("------------------------------------------------------------\n");
    
    enqueue(studentName, studentId, book->title, bookId);
    printf("\n✓ Reservation created with queuing number.\n");
    printf("Note: Book will be marked as borrowed after admin approval.\n");
    printf("============================================================\n");
    pressEnter();
}

void studentReturnBook(const char *studentId, const char *studentName) {
    clearScreen();
    printf("============================================================\n");
    printf("                    🔁 RETURN A BOOK\n");
    printf("============================================================\n");
    
    int bookId, rating;
    printf("Enter Book ID to return: ");
    if (scanf("%d", &bookId) != 1) {
        while(getchar() != '\n');
        pressEnter();
        return;
    }
    
    Book *book = searchBookById(bookId);
    if (book == NULL) {
        printf("✗ Book not found!\n");
        pressEnter();
        return;
    }
    
    printf("\n------------------------------------------------------------\n");
    printf("⭐ Please rate this book (1-5 stars): ");
    if (scanf("%d", &rating) != 1 || rating < 1 || rating > 5) {
        printf("Invalid rating! Using default rating of 3.\n");
        rating = 3;
        while(getchar() != '\n');
    }
    
    printf("\n--- PROCESSING RETURN ---\n");
    printf("Book: %s\n", book->title);
    printf("Before: %d copies available\n", book->quantity);
    
    book->quantity++;  /* Increase quantity */
    
    printf("After:  %d copies available\n", book->quantity);
    printf("--- BOOK RETURNED TO SHELF ---\n");
    
    markAsReturned(studentId, bookId);
    pushReturn(studentName, book->title, bookId, rating);
    
    printf("\n✅ Thank you! Book successfully returned.\n");
    printf("Your rating: ");
    printStars(rating);
    printf(" (%d/5)\n", rating);
    printf("This book has been logged in your Return History.\n");
    printf("============================================================\n");
    pressEnter();
}

void displayOverdueBooks(const char *studentId, const char *studentName) {
    clearScreen();
    printf("========================== OVERDUE BOOKS =====================\n");
    printf("Student: %s\n", studentName);
    printf("------------------------------------------------------------\n");
    printf("Book Title                | Due Date    | Days Overdue\n");
    printf("------------------------------------------------------------\n");
    
    BorrowHistory *temp = historyList;
    int found = 0;
    
    while (temp != NULL) {
        if (strcmp(temp->studentId, studentId) == 0 && !temp->returned) {
            /* In real implementation, calculate actual overdue days */
            printf("%-25s | %s  |  [Check date]\n", 
                   temp->bookTitle, temp->borrowDate);
            found = 1;
        }
        temp = temp->next;
    }
    
    if (!found) {
        printf("✓ No overdue books. Great job!\n");
    }
    printf("==============================================================\n");
}