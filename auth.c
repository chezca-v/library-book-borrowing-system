/*
 * auth.c
 * Implementation of authentication functions
 */

#include <stdio.h>
#include <string.h>
#include "auth.h"
#include "utils.h"

int adminLogin(void) {
    clearScreen();
    printf("============================================================\n");
    printf("                       ADMIN LOGIN\n");
    printf("============================================================\n");
    
    char username[50], password[50];
    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);
    
    if (strcmp(username, "admin") == 0 && strcmp(password, "admin123") == 0) {
        printf("\nLogin successful! Welcome, Admin.\n");
        pressEnter();
        return 1;
    }
    
    printf("\nInvalid credentials!\n");
    pressEnter();
    return 0;
}

int studentLogin(char *studentId, char *studentName) {
    clearScreen();
    printf("============================================================\n");
    printf("                      STUDENT LOGIN\n");
    printf("============================================================\n");
    
    char password[50];
    printf("Student ID: ");
    scanf("%s", studentId);
    printf("Password  : ");
    scanf("%s", password);
    
    /* Demo credentials */
    if (strcmp(studentId, "student1") == 0 && strcmp(password, "pass1") == 0) {
        strcpy(studentName, "John Doe");
        printf("\nLogin successful! Welcome, %s.\n", studentName);
        pressEnter();
        return 1;
    } else if (strcmp(studentId, "student2") == 0 && strcmp(password, "pass2") == 0) {
        strcpy(studentName, "Jane Smith");
        printf("\nLogin successful! Welcome, %s.\n", studentName);
        pressEnter();
        return 1;
    }
    
    printf("\nInvalid credentials!\n");
    pressEnter();
    return 0;
}