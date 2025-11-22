/*
 * book.c
 * Implementation of Linked List operations for Book Catalog
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "book.h"

/* Global book catalog */
Book *bookCatalog = NULL;

/*
 * createBook()
 * Parameters: book details (id, title, author, genre, isbn, year, rating, quantity)
 * Returns: pointer to newly created Book node
 * Purpose: Allocates memory and initializes a new book node
 */
Book* createBook(int id, const char *title, const char *author, 
                 const char *genre, const char *isbn, int year, 
                 float rating, int quantity) {
    Book *newBook = (Book*)malloc(sizeof(Book));
    if (!newBook) {
        printf("Memory allocation failed!\n");
        return NULL;
    }
    newBook->id = id;
    strncpy(newBook->title, title, MAX_STRING - 1);
    newBook->title[MAX_STRING - 1] = '\0';
    strncpy(newBook->author, author, MAX_STRING - 1);
    newBook->author[MAX_STRING - 1] = '\0';
    strncpy(newBook->genre, genre, MAX_STRING - 1);
    newBook->genre[MAX_STRING - 1] = '\0';
    strncpy(newBook->isbn, isbn, 49);
    newBook->isbn[49] = '\0';
    newBook->year = year;
    newBook->rating = rating;
    newBook->quantity = quantity;
    newBook->borrowCount = 0;
    newBook->next = NULL;
    return newBook;
}

/*
 * addBook()
 * Parameters: pointer to head pointer, new book node
 * Returns: void (modifies list through pointer)
 * Purpose: Adds a new book to the end of the linked list
 */
void addBook(Book **head, Book *newBook) {
    if (*head == NULL) {
        *head = newBook;
    } else {
        Book *temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newBook;
    }
}

/*
 * displayBooksRecursive()
 * Parameters: pointer to current book node, pointer to count
 * Returns: void
 * Purpose: Recursively displays all books in the catalog
 * Note: This demonstrates the use of recursion in tree/list traversal
 */
void displayBooksRecursive(Book *book, int *count) {
    if (book == NULL) return;
    
    (*count)++;
    printf("%-7d| %-27s| %-16s| %d\n", 
           book->id, book->title, book->author, book->quantity);
    
    displayBooksRecursive(book->next, count);
}

void displayAllBooks(void) {
    clearScreen();
    printf("============================================================\n");
    printf("                         BOOK CATALOG\n");
    printf("============================================================\n");
    
    if (bookCatalog == NULL) {
        printf("No books in the catalog.\n");
    } else {
        printf("ID     | Title                      | Author          | Qty\n");
        printf("------------------------------------------------------------\n");
        int count = 0;
        displayBooksRecursive(bookCatalog, &count);
        printf("------------------------------------------------------------\n");
        printf("Total Books: %d\n", count);
    }
    printf("============================================================\n");
}

Book* searchBookById(int id) {
    Book *temp = bookCatalog;
    while (temp != NULL) {
        if (temp->id == id) return temp;
        temp = temp->next;
    }
    return NULL;
}

/*
 * searchBookByTitleRecursive()
 * Parameters: pointer to current book, search query string
 * Returns: pointer to found book or NULL
 * Purpose: Recursively searches for a book by title or author (partial match)
 * Note: Demonstrates recursive search algorithm
 */
Book* searchBookByTitleRecursive(Book *book, const char *query) {
    if (book == NULL) return NULL;
    
    if (strstr(book->title, query) != NULL || 
        strstr(book->author, query) != NULL) {
        return book;
    }
    
    return searchBookByTitleRecursive(book->next, query);
}

void deleteBook(int id) {
    Book *temp = bookCatalog, *prev = NULL;
    
    if (temp != NULL && temp->id == id) {
        bookCatalog = temp->next;
        free(temp);
        printf("Book successfully removed!\n");
        return;
    }
    
    while (temp != NULL && temp->id != id) {
        prev = temp;
        temp = temp->next;
    }
    
    if (temp == NULL) {
        printf("Book not found!\n");
        return;
    }
    
    prev->next = temp->next;
    free(temp);
    printf("Book successfully removed!\n");
}

void freeBookList(Book **head) {
    Book *temp;
    while (*head != NULL) {
        temp = *head;
        *head = (*head)->next;
        free(temp);
    }
}