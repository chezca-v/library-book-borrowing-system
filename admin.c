/*
 * admin.c - Updated with duplicate detection and batch processing
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
#include "fileio.h"

void adminAddBook(void) {
    clearScreen();
    printf("%s============================================================%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
    printf("                        %sADD NEW BOOK%s\n", ANSI_COLOR_CYAN, ANSI_COLOR_RESET);
    printf("%s============================================================%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
    
    int id, year, quantity;
    char title[MAX_STRING], author[MAX_STRING], genre[MAX_STRING], isbn[50];
    float rating;
    
    printf("Enter Book ID: ");
    if (scanf("%d", &id) != 1) {
        printf("%sInvalid input!%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
        while(getchar() != '\n');
        pressEnter();
        return;
    }
    while(getchar() != '\n');
    
    /* Check if ID already exists */
    Book *existingBook = searchBookById(id);
    if (existingBook != NULL) {
        printf("\n%s[!] ERROR: Book ID %d already exists!%s\n", ANSI_COLOR_RED, id, ANSI_COLOR_RESET);
        printf("    Existing book: \"%s\" by %s\n", existingBook->title, existingBook->author);
        printf("\n    Please use a different ID.\n");
        pressEnter();
        return;
    }
    
    printf("Enter Title: ");
    if (fgets(title, MAX_STRING, stdin) == NULL) return;
    title[strcspn(title, "\n")] = '\0';
    
    /* Check if title already exists (case-insensitive) */
    Book *temp = bookCatalog;
    Book *matchingBook = NULL;
    
    while (temp != NULL) {
        #ifdef _WIN32
        int titleMatch = (_stricmp(temp->title, title) == 0);
        #else
        int titleMatch = (strcasecmp(temp->title, title) == 0);
        #endif
        
        if (titleMatch) {
            matchingBook = temp;
            break;
        }
        temp = temp->next;
    }
    
    if (matchingBook != NULL) {
        printf("\n%s============================================================%s\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
        printf("%s[!] WARNING: A book with this title already exists!%s\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
        printf("%s============================================================%s\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
        printf("    Title:    \"%s\"\n", matchingBook->title);
        printf("    Author:   %s\n", matchingBook->author);
        printf("    ID:       %d\n", matchingBook->id);
        printf("    Quantity: %d copies\n", matchingBook->quantity);
        printf("%s============================================================%s\n\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
        
        char choice;
        printf("What would you like to do?\n");
        printf("  %s[A]%s Add as new book anyway (different edition)\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
        printf("  %s[U]%s Update quantity of existing book\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
        printf("  %s[C]%s Cancel operation\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
        printf("\nYour choice: ");
        scanf(" %c", &choice);
        while(getchar() != '\n');
        
        if (choice == 'U' || choice == 'u') {
            int addQty;
            printf("\nHow many copies to add to existing book? ");
            if (scanf("%d", &addQty) == 1 && addQty > 0) {
                matchingBook->quantity += addQty;
                printf("\n%s============================================================%s\n", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
                printf("%s[✓] SUCCESS: Quantity updated!%s\n", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
                printf("%s============================================================%s\n", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
                printf("    Book:         \"%s\"\n", matchingBook->title);
                printf("    New quantity: %d copies\n", matchingBook->quantity);
                printf("%s============================================================%s\n", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
                saveBooksToCSV();
            } else {
                printf("\n%s[X] Invalid quantity! Operation cancelled.%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
            }
            pressEnter();
            return;
        } else if (choice == 'C' || choice == 'c') {
            printf("\n%s[X] Operation cancelled. No changes made.%s\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
            pressEnter();
            return;
        } else if (choice == 'A' || choice == 'a') {
            printf("\n%s[!] Proceeding to add as separate book...%s\n\n", ANSI_COLOR_CYAN, ANSI_COLOR_RESET);
        } else {
            printf("\n%s[X] Invalid choice. Operation cancelled.%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
            pressEnter();
            return;
        }
    }
    
    printf("Enter Author: ");
    if (fgets(author, MAX_STRING, stdin) == NULL) return;
    author[strcspn(author, "\n")] = '\0';
    
    printf("Enter Genre: ");
    if (fgets(genre, MAX_STRING, stdin) == NULL) return;
    genre[strcspn(genre, "\n")] = '\0';
    
    printf("Enter ISBN: ");
    if (fgets(isbn, 50, stdin) == NULL) return;
    isbn[strcspn(isbn, "\n")] = '\0';
    
    /* Check if ISBN already exists */
    temp = bookCatalog;
    while (temp != NULL) {
        if (strcmp(temp->isbn, isbn) == 0) {
            printf("\n%s[!] ERROR: ISBN %s already exists!%s\n", ANSI_COLOR_RED, isbn, ANSI_COLOR_RESET);
            printf("    Book: \"%s\" by %s (ID: %d)\n", 
                   temp->title, temp->author, temp->id);
            printf("\n    Each book must have a unique ISBN.\n");
            pressEnter();
            return;
        }
        temp = temp->next;
    }
    
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
        printf("\n%s[✓] Book successfully added!%s\n", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
        printf("    Title: %s\n", title);
        printf("    Author: %s\n", author);
        printf("    Quantity: %d copies\n", quantity);
        saveBooksToCSV();
    }
    pressEnter();
}

void adminRemoveBook(void) {
    clearScreen();
    printf("%s============================================================%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
    printf("                        %sREMOVE BOOK%s\n", ANSI_COLOR_CYAN, ANSI_COLOR_RESET);
    printf("%s============================================================%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
    
    int id;
    printf("Enter Book ID to remove: ");
    if (scanf("%d", &id) != 1) {
        while(getchar() != '\n');
        pressEnter();
        return;
    }
    
    Book *book = searchBookById(id);
    if (book == NULL) {
        printf("%sBook not found!%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
        pressEnter();
        return;
    }
    
    printf("%s------------------------------------------------------------%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
    printf("Book: %s%s%s by %s (Qty: %d)\n", ANSI_COLOR_BOLD, book->title, ANSI_COLOR_RESET, book->author, book->quantity);
    
    char confirm;
    printf("Delete? (Y/N): ");
    scanf(" %c", &confirm);
    
    if (confirm == 'Y' || confirm == 'y') {
        deleteBook(id);
        saveBooksToCSV();
        printf("%sBook deleted.%s\n", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
    }
    pressEnter();
}

void adminProcessBorrowRequest(void) {
    clearScreen();
    
    if (queueFront == NULL) {
        printf("%s============================================================%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
        printf("                  %sPROCESS BORROW REQUEST%s\n", ANSI_COLOR_CYAN, ANSI_COLOR_RESET);
        printf("%s============================================================%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
        printf("%sNo pending requests.%s\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
        pressEnter();
        return;
    }
    
    int processedCount = 0;
    char continueProcessing = 'Y';
    
    while (queueFront != NULL && (continueProcessing == 'Y' || continueProcessing == 'y')) {
        clearScreen();
        printf("%s============================================================%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
        printf("                  %sPROCESS BORROW REQUEST%s\n", ANSI_COLOR_CYAN, ANSI_COLOR_RESET);
        printf("%s============================================================%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
        
        BorrowRequest *request = queueFront;
        Book *book = searchBookById(request->bookId);
        
        printf("Next in Queue:\n");
        printf("Queue %s#%03d%s: %s%s%s wants \"%s%s%s\"\n", 
               ANSI_COLOR_CYAN, request->queueNumber, ANSI_COLOR_RESET,
               ANSI_COLOR_BOLD, request->studentName, ANSI_COLOR_RESET,
               ANSI_COLOR_BOLD, request->bookTitle, ANSI_COLOR_RESET);
        
        if (book) {
            printf("Current quantity: %d copies\n", book->quantity);
            if (book->quantity > 0) {
                printf("Status: %sAvailable%s\n", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
            } else {
                printf("Status: %sOUT OF STOCK!%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
            }
        } else {
            printf("%sWarning: Book not found in catalog!%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
        }
        
        char confirm;
        printf("\nApprove this request? (Y/N): ");
        scanf(" %c", &confirm);
        
        if (confirm == 'Y' || confirm == 'y') {
            if (book && book->quantity > 0) {
                printf("\n--- PROCESSING ---\n");
                printf("Before: %d copies\n", book->quantity);
                
                book->quantity--;
                book->borrowCount++;
                
                printf("After:  %d copies\n", book->quantity);
                printf("--- BOOK MARKED AS BORROWED ---\n");
                
                addBorrowHistory(request->studentId, request->studentName,
                               request->bookTitle, request->bookId);
                
                char dueDate[20];
                addDaysToDate(dueDate, 14);
                
                printf("\n%sApproved! Due: %s%s\n", ANSI_COLOR_GREEN, dueDate, ANSI_COLOR_RESET);
                BorrowRequest *toFree = dequeue();
                free(toFree);
                
                processedCount++;
            } else {
                printf("\n%sCannot approve: Book unavailable!%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
                printf("Skipping this request (removing from queue)...\n");
                BorrowRequest *toFree = dequeue();
                free(toFree);
            }
        } else {
            printf("\n%sRequest rejected (removing from queue)...%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
            BorrowRequest *toFree = dequeue();
            free(toFree);
        }
        
        if (queueFront != NULL) {
            printf("\n%s------------------------------------------------------------%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
            printf("Remaining requests in queue: ");
            int remaining = 0;
            BorrowRequest *temp = queueFront;
            while (temp != NULL) {
                remaining++;
                temp = temp->next;
            }
            printf("%d\n", remaining);
            printf("Process next request? (Y/N): ");
            scanf(" %c", &continueProcessing);
        } else {
            printf("\n%s✓ All requests processed!%s\n", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
            break;
        }
    }
    
    if (processedCount > 0) {
        saveBooksToCSV();
        saveQueueToCSV();
        saveHistoryToCSV();
        printf("\n%s%d request(s) processed and saved.%s\n", ANSI_COLOR_GREEN, processedCount, ANSI_COLOR_RESET);
    }
    
    printf("%s============================================================%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
    pressEnter();
}

/* Comparison function for sorting books by borrowCount */
static int cmpBorrowCount(const void *a, const void *b) {
    Book *const *pa = (Book *const *)a;
    Book *const *pb = (Book *const *)b;
    return (*pb)->borrowCount - (*pa)->borrowCount;
}

void adminDashboard(void) {
    clearScreen();
    printf("%s====================== SYSTEM STATISTICS ====================%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
    
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
    
    printf("Total Books in Catalog:  %s%d%s\n", ANSI_COLOR_CYAN, totalBooks, ANSI_COLOR_RESET);
    printf("Books Available       :  %s%d%s\n", ANSI_COLOR_GREEN, availableBooks, ANSI_COLOR_RESET);
    printf("Queue Length          :  %s%d%s\n\n", ANSI_COLOR_YELLOW, queueLength, ANSI_COLOR_RESET);
    
    printf("%sMost Borrowed Books:%s\n", ANSI_COLOR_MAGENTA, ANSI_COLOR_RESET);
    /* Collect books into array and sort by borrowCount desc */
    int bookCount = 0;
    Book *iter = bookCatalog;
    while (iter) { bookCount++; iter = iter->next; }
    if (bookCount > 0) {
        Book **arr = (Book**)malloc(sizeof(Book*) * bookCount);
        if (arr) {
            int i = 0;
            iter = bookCatalog;
            while (iter) { arr[i++] = iter; iter = iter->next; }
            qsort(arr, bookCount, sizeof(Book*), cmpBorrowCount);
            int display = bookCount < 5 ? bookCount : 5;
            for (int r = 0; r < display; r++) {
                printf("%s%d.%s %s (%s%d%s times)\n", 
                       ANSI_COLOR_YELLOW, r+1, ANSI_COLOR_RESET, 
                       arr[r]->title, 
                       ANSI_COLOR_CYAN, arr[r]->borrowCount, ANSI_COLOR_RESET);
            }
            free(arr);
        } else {
            /* fallback */
            int rank = 1;
            temp = bookCatalog;
            while (temp != NULL && rank <= 5) {
                printf("%d. %s (%d times)\n", rank++, temp->title, temp->borrowCount);
                temp = temp->next;
            }
        }
    } else {
        printf("No books available.\n");
    }
    
    printf("%s==============================================================%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
    pressEnter();
}

void adminMenu(void) {
    int choice;
    do {
        clearScreen();
        printf("%s============================================================%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
        printf("                        %sADMIN DASHBOARD%s\n", ANSI_COLOR_CYAN, ANSI_COLOR_RESET);
        printf("%s============================================================%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
        printf("%s[1]%s  Add New Book to Catalog\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
        printf("%s[2]%s  Remove Book from Catalog\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
        printf("%s[3]%s  Display All Books\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
        printf("%s[4]%s  Display Borrowed Books\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
        printf("%s[5]%s  Display Borrowing Queue\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
        printf("%s[6]%s  Process Borrow Requests\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
        printf("%s[7]%s  Display All Users\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
        printf("%s[8]%s  View User's Return History\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
        printf("%s[9]%s  View User Borrowing History\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
        printf("%s[10]%s Log Out\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
        printf("%s------------------------------------------------------------%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
        printf("%sEnter choice: %s", ANSI_COLOR_BOLD, ANSI_COLOR_RESET);
        
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
                printf("\n%s============================================================%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
                printf("                   %sYOU HAVE LOGGED OUT%s\n", ANSI_COLOR_GREEN, ANSI_COLOR_RESET);
                printf("%s============================================================%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
                printf("Returning to Main Menu...\n");
                pressEnter();
                break;
            default: printf("%sInvalid choice!%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET); pressEnter();
        }
    } while(choice != 10);
}

void displayBorrowedBooks(void) {
    clearScreen();
    printf("%s============================================================%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
    printf("            %sCURRENTLY BORROWED BOOKS%s\n", ANSI_COLOR_CYAN, ANSI_COLOR_RESET);
    printf("%s============================================================%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
    printf("%sStudent Name   | Book Title              | Borrow Date | Due Date%s\n", ANSI_COLOR_CYAN, ANSI_COLOR_RESET);
    printf("%s------------------------------------------------------------%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
    
    BorrowHistory *temp = historyList;
    int found = 0;
    
    while (temp != NULL) {
        if (!temp->returned) {
            printf("%-14s | %-23s | %-11s | +14 days\n", 
                   temp->studentName, temp->bookTitle, temp->borrowDate);
            found = 1;
        }
        temp = temp->next;
    }
    
    if (!found) {
        printf("%sNo books currently borrowed.%s\n", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET);
    }
    printf("%s============================================================%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
}

void displayAllUsers(void) {
    clearScreen();
    printf("%s============================================================%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
    printf("                  %sALL USERS%s\n", ANSI_COLOR_CYAN, ANSI_COLOR_RESET);
    printf("%s============================================================%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
    printf("%sID       | Username       | Role%s\n", ANSI_COLOR_CYAN, ANSI_COLOR_RESET);
    printf("%s------------------------------------------------------------%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
    
    FILE *file = fopen("users.csv", "r");
    if (!file) {
        printf("%sError: Cannot open users.csv%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET);
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
        
        (void)pass;
        
        if (!id || !user || !role) continue;
        
        trimWhitespace(id);
        trimWhitespace(user);
        trimWhitespace(role);
        
        printf("%-8s | %-14s | %s\n", id, user, role);
        count++;
    }
    
    fclose(file);
    printf("%s------------------------------------------------------------%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
    printf("Total Users: %s%d%s\n", ANSI_COLOR_GREEN, count, ANSI_COLOR_RESET);
    printf("%s============================================================%s\n", ANSI_COLOR_BLUE, ANSI_COLOR_RESET);
}