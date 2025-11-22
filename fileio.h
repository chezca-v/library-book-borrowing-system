/*
 * fileio.h
 * CSV file I/O operations for data persistence
 */

#ifndef FILEIO_H
#define FILEIO_H

/* Function prototypes */
void loadBooksFromCSV(void);
void saveBooksToCSV(void);
void loadUsersFromCSV(void);
void saveUsersToCSV(void);
void loadQueueFromCSV(void);
void saveQueueToCSV(void);
void loadHistoryFromCSV(void);
void saveHistoryToCSV(void);

#endif /* FILEIO_H */