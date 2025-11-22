/*
 * fileio.c
 * Implementation of CSV file I/O operations
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileio.h"
#include "book.h"

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