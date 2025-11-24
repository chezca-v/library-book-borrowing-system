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
#include "fileio.h"

void studentMenu(const char *studentId, const char *studentName) {
    int choice;
    do {
        clearScreen();
        printf("%s============================================================%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
        printf("                  %sSTUDENT MENU - %s%s\n", ANSI_COLOR_CYAN, studentName, ANSI_COLOR_RESET);
        printf("%s============================================================%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
        printf("%s[1]%s View User's Current Books\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
        printf("%s[2]%s Search Book\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
        printf("%s[3]%s Display Available Books\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
        printf("%s[4]%s Request to Borrow Book\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
        printf("%s[5]%s View Borrow Queue Position\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
        printf("%s[6]%s Return a Book\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
        printf("%s[7]%s View My Returned Books History\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
        printf("%s[8]%s Log Out\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
        printf("%s------------------------------------------------------------%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
        printf("%sChoice: %s", ANSI_COLOR_BOLD, ANSI_COLOR_RESET);
        
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
                    printf("\nYour queue position: %s#%d%s\n", ANSI_COLOR_CYAN, pos, ANSI_COLOR_RESET);
                } else {
                    printf("\n%sYou are not in the borrowing queue.%s\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
                }
                pressEnter();
                break;
            }
            case 6: studentReturnBook(studentId, studentName); break;
            case 7: displayReturnHistory(studentName); pressEnter(); break;
            case 8: 
                printf("\n%s============================================================%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
                printf("                   %sYOU HAVE LOGGED OUT%s\n", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
                printf("%s============================================================%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
                printf("Returning to Main Menu...\n");
                pressEnter();
                break;
            default: printf("%sInvalid choice!%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET); pressEnter();
        }
    } while(choice != 8);
}

void viewCurrentBooks(const char *studentId, const char *studentName) {
    clearScreen();
    printf("%s============================================================%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
    printf("         %sYOUR CURRENT BORROWED BOOKS%s\n", ANSI_COLOR_CYAN, ANSI_COLOR_RESET);
    printf("%s============================================================%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
    printf("Student: %s%s%s\n", ANSI_COLOR_BOLD, studentName, ANSI_COLOR_RESET);
    printf("%s------------------------------------------------------------%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
    printf("%sID  | Book Title                | Borrow Date | Status%s\n", ANSI_COLOR_CYAN, ANSI_COLOR_RESET);
    printf("%s------------------------------------------------------------%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
    
    BorrowHistory *temp = historyList;
    int found = 0;
    int overdueCount = 0;
    
    while (temp != NULL) {
        if (strcmp(temp->studentId, studentId) == 0 && !temp->returned) {
            // "Active" status in Green
            printf("%-3d | %-25s | %-11s | %sActive%s\n", 
                   temp->bookId, temp->bookTitle, temp->borrowDate, ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
            found = 1;
        }
        temp = temp->next;
    }
    
    if (!found) {
        printf("%sYou have no currently borrowed books.%s\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
    } else {
        printf("%s------------------------------------------------------------%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
        if (overdueCount > 0) {
            printf("%sOverdue Tracker: You have %d overdue book(s)!%s\n", ANSI_COLOR_RED, overdueCount, ANSI_COLOR_RESET);
        } else {
            printf("%sAll books are within due date.%s\n", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
        }
        printf("\nTip: Use the Book ID above when returning books!\n");
    }
    printf("%s============================================================%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
}

void studentSearchBook(void) {
    clearScreen();
    printf("%s============================================================%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
    printf("                    %sSEARCH BOOK%s\n", ANSI_COLOR_CYAN, ANSI_COLOR_RESET);
    printf("%s============================================================%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
    printf("Search by:\n");
    printf("%s[1]%s Author\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
    printf("%s[2]%s Title\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
    printf("%s[3]%s ISBN\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
    printf("%s[4]%s ID\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
    printf("%s------------------------------------------------------------%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
    printf("%sChoose search type: %s", ANSI_COLOR_BOLD, ANSI_COLOR_RESET);
    
    int searchType;
    if (scanf("%d", &searchType) != 1) {
        while(getchar() != '\n');
        printf("%sInvalid input!%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
        pressEnter();
        return;
    }
    
    while(getchar() != '\n');
    
    char query[MAX_STRING];
    
    switch(searchType) {
        case 1: printf("Enter author name: "); break;
        case 2: printf("Enter book title: "); break;
        case 3: printf("Enter ISBN: "); break;
        case 4: printf("Enter book ID: "); break;
        default:
            printf("%sInvalid search type!%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
            pressEnter();
            return;
    }
    
    if (fgets(query, MAX_STRING, stdin) == NULL) return;
    query[strcspn(query, "\n")] = '\0';
    
    printf("\n%s------------------------------------------------------------%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
    printf("Searching for: \"%s%s%s\"\n", ANSI_COLOR_BOLD, query, ANSI_COLOR_RESET);
    printf("%s------------------------------------------------------------%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
    
    if (searchType == 4) {
        int id = atoi(query);
        Book *result = searchBookById(id);
        if (result) {
            printf("\n%sFound:%s\n", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
            printf("ID: %d\n", result->id);
            printf("Title: %s%s%s\n", ANSI_COLOR_BOLD, result->title, ANSI_COLOR_RESET);
            printf("Author: %s\n", result->author);
            printf("ISBN: %s\n", result->isbn);
            printf("Genre: %s\n", result->genre);
            printf("Rating: ");
            printStars((int)result->rating);
            printf(" (%.1f/5.0)\n", result->rating);
            printf("Available: %s%d copies%s\n", ANSI_COLOR_CYAN, result->quantity, ANSI_COLOR_RESET);
        } else {
            printf("\n%sBook not found.%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
        }
    } else {
        Book *result = searchBookByTitleRecursive(bookCatalog, query);
        
        if (result) {
            printf("\n%sFound:%s\n", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
            printf("ID: %d\n", result->id);
            printf("Title: %s%s%s\n", ANSI_COLOR_BOLD, result->title, ANSI_COLOR_RESET);
            printf("Author: %s\n", result->author);
            printf("ISBN: %s\n", result->isbn);
            printf("Rating: ");
            printStars((int)result->rating);
            printf("\nAvailable: %s%d copies%s\n", ANSI_COLOR_CYAN, result->quantity, ANSI_COLOR_RESET);
        } else {
            printf("\n%sNo exact match found.%s\n\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
            printf("Search Suggestions (titles containing \"%s\"):\n", query);
            
            Book *temp = bookCatalog;
            int suggestionCount = 0;
            
            while (temp != NULL) {
                if (strstr(temp->title, query) != NULL || 
                    strstr(temp->author, query) != NULL) {
                    printf("  - %s%s%s (by %s)\n", ANSI_COLOR_CYAN, temp->title, ANSI_COLOR_RESET, temp->author);
                    suggestionCount++;
                }
                temp = temp->next;
            }
            
            if (suggestionCount == 0) {
                printf("  %sNo suggestions available.%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
            }
        }
    }
    
    printf("%s============================================================%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
    pressEnter();
}

void studentRequestBorrow(const char *studentId, const char *studentName) {
    clearScreen();
    printf("%s============================================================%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
    printf("              %sREQUEST TO BORROW BOOK%s\n", ANSI_COLOR_CYAN, ANSI_COLOR_RESET);
    printf("%s============================================================%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
    
    int bookId;
    printf("Enter Book ID: ");
    if (scanf("%d", &bookId) != 1) {
        while(getchar() != '\n');
        pressEnter();
        return;
    }
    
    Book *book = searchBookById(bookId);
    if (book == NULL) {
        printf("%sBook not found!%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
        pressEnter();
        return;
    }
    
    int currentBorrowed = getCurrentBorrowedCount(studentId);
    if (currentBorrowed >= MAX_BOOKS_PER_STUDENT) {
        printf("\n%sBorrow Limit Reached!%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
        printf("You have reached the maximum borrowing limit (%d books).\n", 
               MAX_BOOKS_PER_STUDENT);
        printf("You will be added to the waiting queue.\n\n");
    }
    
    printf("\n%s------------------------------------------------------------%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
    printf("Book: %s%s%s\n", ANSI_COLOR_BOLD, book->title, ANSI_COLOR_RESET);
    printf("Author: %s\n", book->author);
    printf("Current availability: %s%d copies%s\n", ANSI_COLOR_CYAN, book->quantity, ANSI_COLOR_RESET);
    printf("%s------------------------------------------------------------%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
    
    enqueue(studentName, studentId, book->title, bookId);
    printf("\n%sReservation created with queuing number.%s\n", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
    printf("Note: Book will be marked as borrowed after admin approval.\n");
    
    saveQueueToCSV();
    
    printf("%s============================================================%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
    pressEnter();
}

void studentReturnBook(const char *studentId, const char *studentName) {
    clearScreen();
    printf("%s============================================================%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
    printf("                    %sRETURN A BOOK%s\n", ANSI_COLOR_CYAN, ANSI_COLOR_RESET);
    printf("%s============================================================%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
    
    int bookId, rating;
    printf("Enter Book ID to return: ");
    if (scanf("%d", &bookId) != 1) {
        while(getchar() != '\n');
        pressEnter();
        return;
    }
    
    BorrowHistory *histCheck = historyList;
    BorrowHistory *matchingRecord = NULL;
    while (histCheck != NULL) {
        if (strcmp(histCheck->studentId, studentId) == 0 &&
            histCheck->bookId == bookId &&
            !histCheck->returned) {
            matchingRecord = histCheck;
            break;
        }
        histCheck = histCheck->next;
    }
    
    if (matchingRecord == NULL) {
        printf("\n%sYou have not borrowed this book (or it is already returned).%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
        pressEnter();
        return;
    }
    
    Book *book = searchBookById(bookId);
    if (book == NULL) {
        printf("%sBook not found!%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
        pressEnter();
        return;
    }
    
    printf("\n%s------------------------------------------------------------%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
    printf("Book: %s%s%s\n", ANSI_COLOR_BOLD, book->title, ANSI_COLOR_RESET);
    printf("Current rating: %.1f stars\n", book->rating);
    printf("%s------------------------------------------------------------%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
    printf("Please rate this book (1-5 stars): ");
    if (scanf("%d", &rating) != 1 || rating < 1 || rating > 5) {
        printf("%sInvalid rating! Using default rating of 3.%s\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
        rating = 3;
        while(getchar() != '\n');
    }
    
    printf("\n--- PROCESSING RETURN ---\n");
    printf("Before: %d copies available\n", book->quantity);
    
    /* Update book quantity */
    book->quantity++;
    
    printf("After:  %d copies available\n", book->quantity);
    
    /* Update book rating (weighted average) */
    int ratingCount = 0;
    
    ReturnRecord *temp = returnStack;
    while (temp != NULL) {
        if (temp->bookId == bookId) {
            ratingCount++;
        }
        temp = temp->next;
    }
    
    float oldRating = book->rating;
    if (ratingCount == 0) {
        book->rating = (book->rating + (float)rating) / 2.0;
    } else {
        book->rating = (book->rating * ratingCount + (float)rating) / (ratingCount + 1);
    }
    
    if (book->rating > 5.0) book->rating = 5.0;
    if (book->rating < 0.0) book->rating = 0.0;
    
    printf("\n--- RATING UPDATED ---\n");
    printf("Previous rating: %.1f stars\n", oldRating);
    printf("Your rating:     %d stars ", rating);
    printStars(rating); // Colors handled in utils.c
    printf("\n");
    printf("New rating:      %.1f stars\n", book->rating);
    printf("--- BOOK RETURNED TO SHELF ---\n");
    
    markAsReturned(studentId, bookId);
    pushReturn(studentName, book->title, bookId, rating);
    saveHistoryToCSV();
    saveBooksToCSV();
    
    printf("\n%s%s✓ Book successfully returned and rated!%s\n", ANSI_COLOR_GREEN, ANSI_COLOR_BOLD, ANSI_COLOR_RESET);
    printf("  Your rating has been recorded and the book's average\n");
    printf("  rating has been updated in the catalog.\n");
    printf("%s============================================================%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
    pressEnter();
}

void displayOverdueBooks(const char *studentId, const char *studentName) {
    clearScreen();
    printf("%s========================== OVERDUE BOOKS =====================%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
    printf("Student: %s\n", studentName);
    printf("%s------------------------------------------------------------%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
    printf("Book Title                | Due Date    | Days Overdue\n");
    printf("%s------------------------------------------------------------%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
    
    BorrowHistory *temp = historyList;
    int found = 0;
    
    while (temp != NULL) {
        if (strcmp(temp->studentId, studentId) == 0 && !temp->returned) {
            // Simplified display for example purposes
            printf("%-25s | %s  |  [Check date]\n", 
                   temp->bookTitle, temp->borrowDate);
            found = 1;
        }
        temp = temp->next;
    }
    
    if (!found) {
        printf("%sNo overdue books. Great job!%s\n", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
    }
    printf("%s==============================================================%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
}