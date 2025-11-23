/*
 * auth.h
 * Authentication and authorization functions
 */

#ifndef AUTH_H
#define AUTH_H

/* Function prototypes */
int adminLogin(void);
int studentLogin(char *studentId, char *studentName);

#endif /* AUTH_H */