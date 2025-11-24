/*
 * book.c
 * Implementation of Linked List operations for Book Catalog
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "book.h"

/* Global book catalog */
Book *bookCatalog = NULL;

/*
 * stristr() - Case-insensitive substring search
 * Similar to strstr() but ignores case
 */
static char* stristr(const char *haystack, const char *needle) {
    if (!*needle) return (char*)haystack;
    
    for (; *haystack; haystack++) {
        const char *h = haystack;
        const char *n = needle;
        
        while (*h && *n && tolower((unsigned char)*h) == tolower((unsigned char)*n)) {
            h++;
            n++;
        }
        
        if (!*n) return (char*)haystack;
    }
    
    return NULL;
}

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
        printf("============================================================\n");
        return;
    }
    
    /* Count total books first */
    int totalBooks = 0;
    Book *counter = bookCatalog;
    while (counter != NULL) {
        totalBooks++;
        counter = counter->next;
    }
    
    const int BOOKS_PER_PAGE = 15;
    int totalPages = (totalBooks + BOOKS_PER_PAGE - 1) / BOOKS_PER_PAGE;
    int currentPage = 1;
    char choice;
    
    do {
        clearScreen();
        printf("============================================================\n");
        printf("                  BOOK CATALOG (Page %d/%d)\n", currentPage, totalPages);
        printf("============================================================\n");
        printf("ID     | Title                      | Author          | Qty\n");
        printf("------------------------------------------------------------\n");
        
        /* Calculate start and end positions */
        int startIdx = (currentPage - 1) * BOOKS_PER_PAGE;
        int endIdx = startIdx + BOOKS_PER_PAGE;
        
        /* Display books for current page */
        Book *temp = bookCatalog;
        int idx = 0;
        int displayed = 0;
        
        while (temp != NULL) {
            if (idx >= startIdx && idx < endIdx) {
                printf("%-7d| %-27s| %-16s| %d\n", 
                       temp->id, temp->title, temp->author, temp->quantity);
                displayed++;
            }
            idx++;
            temp = temp->next;
        }
        
        printf("------------------------------------------------------------\n");
        printf("Showing %d-%d of %d books\n", 
               startIdx + 1, startIdx + displayed, totalBooks);
        printf("============================================================\n");
        
        /* Navigation menu */
        if (totalPages > 1) {
            printf("\n[N] Next Page | [P] Previous Page | [Q] Quit: ");
            scanf(" %c", &choice);
            
            if (choice == 'N' || choice == 'n') {
                if (currentPage < totalPages) currentPage++;
            } else if (choice == 'P' || choice == 'p') {
                if (currentPage > 1) currentPage--;
            }
        } else {
            choice = 'Q'; /* Auto-quit if only one page */
        }
        
    } while (choice != 'Q' && choice != 'q');
    
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
 * Purpose: Recursively searches for a book by title or author (case-insensitive partial match)
 * Note: Demonstrates recursive search algorithm
 */
Book* searchBookByTitleRecursive(Book *book, const char *query) {
    if (book == NULL) return NULL;
    
    /* Use case-insensitive substring search */
    if (stristr(book->title, query) != NULL || 
        stristr(book->author, query) != NULL) {
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

void displayBooksByGenre(const char *genre) {
    clearScreen();
    printf("============================================================\n");
    printf("                  BOOKS IN GENRE: %s\n", genre);
    printf("============================================================\n");
    printf("ID     | Title                      | Author          | Qty\n");
    printf("------------------------------------------------------------\n");
    
    Book *temp = bookCatalog;
    int count = 0;
    
    while (temp != NULL) {
        /* Case-insensitive genre search */
        if (stristr(temp->genre, genre) != NULL) {
            printf("%-7d| %-27s| %-16s| %d\n", 
                   temp->id, temp->title, temp->author, temp->quantity);
            count++;
        }
        temp = temp->next;
    }
    
    if (count == 0) {
        printf("No books found in this genre.\n");
    }
    
    printf("------------------------------------------------------------\n");
    printf("Total: %d books\n", count);
    printf("============================================================\n");
}

void displayTopRatedBooks(void) {
    clearScreen();
    printf("============================================================\n");
    printf("                  TOP RATED BOOKS (4.5+ Stars)\n");
    printf("============================================================\n");
    printf("ID     | Title                      | Rating | Qty\n");
    printf("------------------------------------------------------------\n");
    
    Book *temp = bookCatalog;
    int count = 0;
    
    while (temp != NULL) {
        if (temp->rating >= 4.5) {
            printf("%-7d| %-27s| %.1f    | %d\n", 
                   temp->id, temp->title, temp->rating, temp->quantity);
            count++;
        }
        temp = temp->next;
    }
    
    if (count == 0) {
        printf("No highly rated books found.\n");
    }
    
    printf("------------------------------------------------------------\n");
    printf("Total: %d books\n", count);
    printf("============================================================\n");
}