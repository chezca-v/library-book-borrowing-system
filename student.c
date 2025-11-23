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

void studentReturnBook(const char *studentId, const char *studentName) {
    clearScreen();
    printf("============================================================\n");
    printf("                    RETURN A BOOK\n");
    printf("============================================================\n");
    
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
        printf("\nYou have not borrowed this book (or it is already returned).\n");
        pressEnter();
        return;
    }
    
    Book *book = searchBookById(bookId);
    if (book == NULL) {
        printf("Book not found!\n");
        pressEnter();
        return;
    }
    
    printf("\n------------------------------------------------------------\n");
    printf("Book: %s\n", book->title);
    printf("Current rating: %.1f stars\n", book->rating);
    printf("------------------------------------------------------------\n");
    printf("Please rate this book (1-5 stars): ");
    if (scanf("%d", &rating) != 1 || rating < 1 || rating > 5) {
        printf("Invalid rating! Using default rating of 3.\n");
        rating = 3;
        while(getchar() != '\n');
    }
    
    printf("\n--- PROCESSING RETURN ---\n");
    printf("Before: %d copies available\n", book->quantity);
    
    /* Update book quantity */
    book->quantity++;
    
    printf("After:  %d copies available\n", book->quantity);
    
    /* Update book rating (weighted average) */
    /* Formula: newRating = (oldRating * borrowCount + userRating) / (borrowCount + 1) */
    /* But since borrowCount tracks total borrows, we use a simpler running average */
    
    /* Count how many ratings this book has received */
    int ratingCount = 0;
    float totalRating = book->rating; /* Start with current rating as baseline */
    
    /* Count ratings from return stack */
    ReturnRecord *temp = returnStack;
    while (temp != NULL) {
        if (temp->bookId == bookId) {
            ratingCount++;
            if (ratingCount == 1) {
                totalRating = temp->rating; /* First rating found */
            }
        }
        temp = temp->next;
    }
    
    /* Calculate new average rating */
    float oldRating = book->rating;
    if (ratingCount == 0) {
        /* No previous ratings in stack, use simple average with current rating */
        book->rating = (book->rating + (float)rating) / 2.0;
    } else {
        /* Calculate running average */
        book->rating = (book->rating * ratingCount + (float)rating) / (ratingCount + 1);
    }
    
    /* Ensure rating stays within 0-5 range */
    if (book->rating > 5.0) book->rating = 5.0;
    if (book->rating < 0.0) book->rating = 0.0;
    
    printf("\n--- RATING UPDATED ---\n");
    printf("Previous rating: %.1f stars\n", oldRating);
    printf("Your rating:     %d stars ", rating);
    printStars(rating);
    printf("\n");
    printf("New rating:      %.1f stars\n", book->rating);
    printf("--- BOOK RETURNED TO SHELF ---\n");
    
    /* Mark as returned in history */
    markAsReturned(studentId, bookId);
    
    /* Push to return stack */
    pushReturn(studentName, book->title, bookId, rating);
    
    /* Save all changes */
    saveHistoryToCSV();
    saveBooksToCSV();
    
    printf("\n✓ Book successfully returned and rated!\n");
    printf("  Your rating has been recorded and the book's average\n");
    printf("  rating has been updated in the catalog.\n");
    printf("============================================================\n");
    pressEnter();
}