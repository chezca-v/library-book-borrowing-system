/*
 * fileio.c
 * Implementation of CSV file I/O operations
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileio.h"
#include "book.h"
#include "queue.h"
#include "history.h"
#include "utils.h"

/*
 * loadBooksFromCSV()
 * Purpose: Loads book data from books.csv file
 * Format: id,title,author,genre,isbn,year,rating,quantity
 */
void loadBooksFromCSV(void) {
    FILE *fp = fopen("books.csv", "r");
    
    if (fp == NULL) {
        printf("books.csv not found. Loading default data...\n");
        /* Load default sample data if file doesn't exist */
        addBook(&bookCatalog, createBook(101, "The Silent Patient", "Alex Michaelides", 
                "Thriller", "978-1250301697", 2019, 4.5, 4));
        addBook(&bookCatalog, createBook(102, "Atomic Habits", "James Clear", 
                "Self-help", "978-0735211292", 2018, 4.8, 2));
        addBook(&bookCatalog, createBook(103, "The Hobbit", "J.R.R. Tolkien", 
                "Fantasy", "978-0547928227", 1937, 4.7, 5));
        addBook(&bookCatalog, createBook(104, "Becoming", "Michelle Obama", 
                "Biography", "978-1524763138", 2018, 4.6, 1));
        addBook(&bookCatalog, createBook(105, "Educated", "Tara Westover", 
                "Memoir", "978-0399590504", 2018, 4.7, 3));
        addBook(&bookCatalog, createBook(106, "Where the Crawdads Sing", "Delia Owens", 
                "Fiction", "978-0735219090", 2018, 4.5, 2));
        addBook(&bookCatalog, createBook(107, "The Midnight Library", "Matt Haig", 
                "Fiction", "978-0525559474", 2020, 4.4, 3));
        return;
    }
    
    char line[500];
    /* Skip header line */
    if (fgets(line, sizeof(line), fp) == NULL) {
        fclose(fp);
        return;
    }
    
    /* Read each line and parse CSV */
    while (fgets(line, sizeof(line), fp) != NULL) {
        int id, year, quantity;
        char title[200], author[200], genre[200], isbn[50];
        float rating;
        
        /* Parse CSV line: id,title,author,genre,isbn,year,rating,quantity */
        char *token;
        int field = 0;
        
        token = strtok(line, ",");
        while (token != NULL && field < 8) {
            switch(field) {
                case 0: id = atoi(token); break;
                case 1: strncpy(title, token, 199); title[199] = '\0'; break;
                case 2: strncpy(author, token, 199); author[199] = '\0'; break;
                case 3: strncpy(genre, token, 199); genre[199] = '\0'; break;
                case 4: strncpy(isbn, token, 49); isbn[49] = '\0'; break;
                case 5: year = atoi(token); break;
                case 6: rating = atof(token); break;
                case 7: quantity = atoi(token); break;
            }
            token = strtok(NULL, ",");
            field++;
        }
        
        /* Create and add book */
        if (field == 8) {
            Book *newBook = createBook(id, title, author, genre, isbn, year, rating, quantity);
            if (newBook) {
                addBook(&bookCatalog, newBook);
            }
        }
    }
    
    fclose(fp);
    printf("Books loaded successfully from CSV.\n");
}

/*
 * saveBooksToCSV()
 * Purpose: Saves current book catalog to books.csv
 * Format: id,title,author,genre,isbn,year,rating,quantity
 */
void saveBooksToCSV(void) {
    FILE *fp = fopen("books.csv", "w");
    if (fp == NULL) {
        printf("Error: Unable to save books to CSV.\n");
        return;
    }
    
    /* Write header */
    fprintf(fp, "id,title,author,genre,isbn,year,rating,quantity\n");
    
    /* Write each book */
    Book *temp = bookCatalog;
    while (temp != NULL) {
        fprintf(fp, "%d,%s,%s,%s,%s,%d,%.1f,%d\n",
                temp->id, temp->title, temp->author, temp->genre,
                temp->isbn, temp->year, temp->rating, temp->quantity);
        temp = temp->next;
    }
    
    fclose(fp);
    printf("Books saved to CSV successfully.\n");
}

void loadUsersFromCSV(void) {
    /* TODO: Load user data from users.csv */
}

void saveUsersToCSV(void) {
    /* TODO: Save user data to users.csv */
}

/*
 * loadQueueFromCSV()
 * Purpose: Loads borrow queue from queue_data.csv
 * Format: queue_number,student_id,student_name,book_id,book_title,status
 */
void loadQueueFromCSV(void) {
    FILE *fp = fopen("queue_data.csv", "r");
    if (fp == NULL) {
        /* No queue file exists yet */
        return;
    }
    
    char line[500];
    int isFirstLine = 1;
    int maxQueueNum = 0;
    
    while (fgets(line, sizeof(line), fp) != NULL) {
        line[strcspn(line, "\r\n")] = '\0';
        
        /* Skip header */
        if (isFirstLine) {
            isFirstLine = 0;
            continue;
        }
        
        if (line[0] == '\0') continue;
        
        /* Parse: queue_number,student_id,student_name,book_id,book_title,status */
        char temp[500];
        strncpy(temp, line, sizeof(temp)-1);
        temp[sizeof(temp)-1] = '\0';
        
        char *qNum = strtok(temp, ",");
        char *studentId = strtok(NULL, ",");
        char *studentName = strtok(NULL, ",");
        char *bookId = strtok(NULL, ",");
        char *bookTitle = strtok(NULL, ",");
        char *status = strtok(NULL, ",");
        
        if (!qNum || !studentId || !studentName || !bookId || !bookTitle || !status) continue;
        
        int queueNumber = atoi(qNum);
        int bid = atoi(bookId);
        
        /* Manually add to queue (don't use enqueue to avoid printing messages) */
        BorrowRequest *newRequest = (BorrowRequest*)malloc(sizeof(BorrowRequest));
        if (newRequest) {
            newRequest->queueNumber = queueNumber;
            strncpy(newRequest->studentId, studentId, 49);
            newRequest->studentId[49] = '\0';
            strncpy(newRequest->studentName, studentName, MAX_STRING-1);
            newRequest->studentName[MAX_STRING-1] = '\0';
            strncpy(newRequest->bookTitle, bookTitle, MAX_STRING-1);
            newRequest->bookTitle[MAX_STRING-1] = '\0';
            newRequest->bookId = bid;
            strncpy(newRequest->status, status, 19);
            newRequest->status[19] = '\0';
            newRequest->next = NULL;
            
            if (queueRear == NULL) {
                queueFront = queueRear = newRequest;
            } else {
                queueRear->next = newRequest;
                queueRear = newRequest;
            }
            
            if (queueNumber > maxQueueNum) {
                maxQueueNum = queueNumber;
            }
        }
    }
    
    /* Update queue counter */
    queueCounter = maxQueueNum + 1;
    
    fclose(fp);
    printf("Queue loaded from CSV.\n");
}

/*
 * saveQueueToCSV()
 * Purpose: Saves current borrow queue to queue_data.csv
 */
void saveQueueToCSV(void) {
    FILE *fp = fopen("queue_data.csv", "w");
    if (fp == NULL) {
        printf("Error: Unable to save queue to CSV.\n");
        return;
    }
    
    /* Write header */
    fprintf(fp, "queue_number,student_id,student_name,book_id,book_title,status\n");
    
    /* Write each queue entry */
    BorrowRequest *temp = queueFront;
    while (temp != NULL) {
        fprintf(fp, "%d,%s,%s,%d,%s,%s\n",
                temp->queueNumber, temp->studentId, temp->studentName,
                temp->bookId, temp->bookTitle, temp->status);
        temp = temp->next;
    }
    
    fclose(fp);
}

/*
 * loadHistoryFromCSV()
 * Purpose: Loads borrow history from history_data.csv
 * Format: student_id,student_name,book_id,book_title,borrow_date,return_date,returned
 */
void loadHistoryFromCSV(void) {
    FILE *fp = fopen("history_data.csv", "r");
    if (fp == NULL) {
        /* No history file exists yet */
        return;
    }
    
    char line[500];
    int isFirstLine = 1;
    
    while (fgets(line, sizeof(line), fp) != NULL) {
        line[strcspn(line, "\r\n")] = '\0';
        
        /* Skip header */
        if (isFirstLine) {
            isFirstLine = 0;
            continue;
        }
        
        if (line[0] == '\0') continue;
        
        /* Parse CSV */
        char temp[500];
        strncpy(temp, line, sizeof(temp)-1);
        temp[sizeof(temp)-1] = '\0';
        
        char *studentId = strtok(temp, ",");
        char *studentName = strtok(NULL, ",");
        char *bookId = strtok(NULL, ",");
        char *bookTitle = strtok(NULL, ",");
        char *borrowDate = strtok(NULL, ",");
        char *returnDate = strtok(NULL, ",");
        char *returned = strtok(NULL, ",");
        
        if (!studentId || !studentName || !bookId || !bookTitle || !borrowDate || !returnDate || !returned) continue;
        
        /* Manually add to history list */
        BorrowHistory *newRecord = (BorrowHistory*)malloc(sizeof(BorrowHistory));
        if (newRecord) {
            strncpy(newRecord->studentId, studentId, 49);
            newRecord->studentId[49] = '\0';
            strncpy(newRecord->studentName, studentName, MAX_STRING-1);
            newRecord->studentName[MAX_STRING-1] = '\0';
            strncpy(newRecord->bookTitle, bookTitle, MAX_STRING-1);
            newRecord->bookTitle[MAX_STRING-1] = '\0';
            newRecord->bookId = atoi(bookId);
            strncpy(newRecord->borrowDate, borrowDate, 19);
            newRecord->borrowDate[19] = '\0';
            strncpy(newRecord->returnDate, returnDate, 19);
            newRecord->returnDate[19] = '\0';
            newRecord->returned = atoi(returned);
            newRecord->next = historyList;
            historyList = newRecord;
        }
    }
    
    fclose(fp);
    printf("History loaded from CSV.\n");
}

/*
 * saveHistoryToCSV()
 * Purpose: Saves borrow history to history_data.csv
 */
void saveHistoryToCSV(void) {
    FILE *fp = fopen("history_data.csv", "w");
    if (fp == NULL) {
        printf("Error: Unable to save history to CSV.\n");
        return;
    }
    
    /* Write header */
    fprintf(fp, "student_id,student_name,book_id,book_title,borrow_date,return_date,returned\n");
    
    /* Write each history entry */
    BorrowHistory *temp = historyList;
    while (temp != NULL) {
        fprintf(fp, "%s,%s,%d,%s,%s,%s,%d\n",
                temp->studentId, temp->studentName, temp->bookId,
                temp->bookTitle, temp->borrowDate, temp->returnDate,
                temp->returned);
        temp = temp->next;
    }
    
    fclose(fp);
}