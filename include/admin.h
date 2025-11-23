/*
 * admin.h
 * Admin module functions
 */

#ifndef ADMIN_H
#define ADMIN_H

/* Function prototypes */
void adminAddBook(void);
void adminRemoveBook(void);
void adminProcessBorrowRequest(void);
void adminDashboard(void);
void displayBorrowedBooks(void);
void displayAllUsers(void);
void trimWhitespace(char *str);
void adminMenu(void);

#endif /* ADMIN_H */