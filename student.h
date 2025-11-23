/*
 * student.h
 * Student module functions
 */

#ifndef STUDENT_H
#define STUDENT_H

/* Function prototypes */
void viewCurrentBooks(const char *studentId, const char *studentName);
void studentSearchBook(void);
void studentRequestBorrow(const char *studentId, const char *studentName);
void studentReturnBook(const char *studentId, const char *studentName);
void displayOverdueBooks(const char *studentId, const char *studentName);
void studentMenu(const char *studentId, const char *studentName);

#endif /* STUDENT_H */