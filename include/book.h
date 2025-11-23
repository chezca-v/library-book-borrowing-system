/*
 * book.h
 * Linked List implementation for Book Catalog
 */

#ifndef BOOK_H
#define BOOK_H

#include "utils.h"

/* Book structure (Linked List Node) */
typedef struct Book {
    int id;
    char title[MAX_STRING];
    char author[MAX_STRING];
    char genre[MAX_STRING];
    char isbn[50];
    int year;
    float rating;
    int quantity;
    int borrowCount;
    struct Book *next;
} Book;

/* Global book catalog */
extern Book *bookCatalog;

/* Function prototypes */
Book* createBook(int id, const char *title, const char *author, 
                 const char *genre, const char *isbn, int year, 
                 float rating, int quantity);
void addBook(Book **head, Book *newBook);
void displayBooksRecursive(Book *book, int *count);
void displayAllBooks(void);
void displayBooksByGenre(const char *genre);
void displayTopRatedBooks(void);
Book* searchBookById(int id);
Book* searchBookByTitleRecursive(Book *book, const char *query);
void deleteBook(int id);
void freeBookList(Book **head);

#endif /* BOOK_H */