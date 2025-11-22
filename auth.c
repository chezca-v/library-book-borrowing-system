#include <stdio.h>
#include <string.h>
#include "auth.h"
#include "utils.h"

#define USERS_FILE "users.csv"

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

    FILE *file = fopen(USERS_FILE, "r");
    if (!file) {
        printf("\nError: Cannot open users.csv\n");
        pressEnter();
        return 0;
    }

    char line[256];
    fgets(line, sizeof(line), file); // skip header
    while (fgets(line, sizeof(line), file)) {
        char role[20], id[50], name[50], pass[50];
        sscanf(line, "%[^,],%[^,],%[^,],%s", role, id, name, pass);

        if (strcmp(role, "admin") == 0 && strcmp(id, username) == 0 && strcmp(pass, password) == 0) {
            printf("\nLogin successful! Welcome, Admin.\n");
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
    
    char password[50];
    printf("Student ID: ");
    scanf("%s", studentId);
    printf("Password  : ");
    scanf("%s", password);

    FILE *file = fopen(USERS_FILE, "r");
    if (!file) {
        printf("\nError: Cannot open users.csv\n");
        pressEnter();
        return 0;
    }

    char line[256];
    fgets(line, sizeof(line), file); // skip header
    while (fgets(line, sizeof(line), file)) {
        char role[20], id[50], name[50], pass[50];
        sscanf(line, "%[^,],%[^,],%[^,],%s", role, id, name, pass);

        if (strcmp(role, "student") == 0 && strcmp(id, studentId) == 0 && strcmp(pass, password) == 0) {
            strcpy(studentName, name);
            printf("\nLogin successful! Welcome, %s.\n", studentName);
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
