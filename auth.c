/*
 * auth.c
 * Implementation of authentication functions
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "auth.h"
#include "utils.h"

#define USERS_FILE "users.csv"

/* Trim leading/trailing whitespace */
void trimWhitespace(char *str) {
    char *start = str;
    while(isspace((unsigned char)*start)) start++;
    memmove(str, start, strlen(start)+1);

    char *end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) *end-- = '\0';
}

int adminLogin(void) {
    clearScreen();
    printf("============================================================\n");
    printf("                       ADMIN LOGIN\n");
    printf("============================================================\n");
    
    char username[50], password[50];
    printf("Username: ");
    if (scanf("%s", username) != 1) {
        while(getchar() != '\n');
        printf("\nInput error!\n");
        pressEnter();
        return 0;
    }
    printf("Password: ");
    if (scanf("%s", password) != 1) {
        while(getchar() != '\n');
        printf("\nInput error!\n");
        pressEnter();
        return 0;
    }

    FILE *file = fopen(USERS_FILE, "r");
    if (!file) {
        printf("\nError: Cannot open %s\n", USERS_FILE);
        pressEnter();
        return 0;
    }

    char line[512];
    int isFirstLine = 1;
    
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = '\0';
        
        /* Skip header line */
        if (isFirstLine) {
            isFirstLine = 0;
            continue;
        }
        
        /* Skip empty lines */
        if (line[0] == '\0') continue;

        /* Parse CSV: id,username,password,role */
        char temp[512];
        strncpy(temp, line, sizeof(temp)-1);
        temp[sizeof(temp)-1] = '\0';

        char *id = strtok(temp, ",");
        char *user = strtok(NULL, ",");
        char *pass = strtok(NULL, ",");
        char *role = strtok(NULL, ",");

        if (!id || !user || !pass || !role) continue;

        /* Trim whitespace */
        trimWhitespace(id);
        trimWhitespace(user);
        trimWhitespace(pass);
        trimWhitespace(role);

        /* Check if admin and credentials match */
        if (strcmp(role, "admin") == 0 &&
            strcmp(user, username) == 0 &&
            strcmp(pass, password) == 0) {
            printf("\nLogin successful! Welcome, %s.\n", user);
            fclose(file);
            pressEnter();
            return 1;
        }
    }

    fclose(file);
    printf("\nInvalid credentials!\n");
    pressEnter();
    return 0;
}

int studentLogin(char *studentId, char *studentName) {
    clearScreen();
    printf("============================================================\n");
    printf("                      STUDENT LOGIN\n");
    printf("============================================================\n");
    
    char input[50], password[50];
    printf("Student ID or Username: ");
    if (scanf("%s", input) != 1) {
        while(getchar() != '\n');
        printf("\nInput error!\n");
        pressEnter();
        return 0;
    }
    printf("Password: ");
    if (scanf("%s", password) != 1) {
        while(getchar() != '\n');
        printf("\nInput error!\n");
        pressEnter();
        return 0;
    }

    FILE *file = fopen(USERS_FILE, "r");
    if (!file) {
        printf("\nError: Cannot open %s\n", USERS_FILE);
        pressEnter();
        return 0;
    }

    char line[512];
    int isFirstLine = 1;
    
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = '\0';
        
        /* Skip header line */
        if (isFirstLine) {
            isFirstLine = 0;
            continue;
        }
        
        /* Skip empty lines */
        if (line[0] == '\0') continue;

        /* Parse CSV: id,username,password,role */
        char temp[512];
        strncpy(temp, line, sizeof(temp)-1);
        temp[sizeof(temp)-1] = '\0';

        char *id = strtok(temp, ",");
        char *user = strtok(NULL, ",");
        char *pass = strtok(NULL, ",");
        char *role = strtok(NULL, ",");

        if (!id || !user || !pass || !role) continue;

        /* Trim whitespace */
        trimWhitespace(id);
        trimWhitespace(user);
        trimWhitespace(pass);
        trimWhitespace(role);

        /* Check if student and credentials match (accept either ID or username) */
        if (strcmp(role, "student") == 0 &&
            strcmp(pass, password) == 0 &&
            (strcmp(user, input) == 0 || strcmp(id, input) == 0)) {
            
            strcpy(studentName, user);
            strcpy(studentId, id);
            printf("\nLogin successful! Welcome, %s.\n", user);
            fclose(file);
            pressEnter();
            return 1;
        }
    }

    fclose(file);
    printf("\nInvalid credentials!\n");
    pressEnter();
    return 0;
}