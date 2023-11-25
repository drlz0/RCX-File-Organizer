#include "main_functionality.h"
#include "utilities.h"
#include "main.h"

// Function to rename files in a folder
void renameFilesInFolder(const char *folderPath, const char *name, int startCounter, int includeTimestamp, int includeLogging, int includeBackup) 
{
    DIR *dir;
    struct dirent *ent;
    int counter = startCounter; // Initialize the counter with the startCounter value
    char logFilePath[256]; // Buffer for the log path

    /* Check if a backup is requested */ 
    if (includeBackup) {
        backupFiles(folderPath);
    }

    /* Start the logging process */
    if (includeLogging) {
        char logMessage[512];

        snprintf(logFilePath, sizeof(logFilePath), "%s/rename_loggs.txt", folderPath);
        snprintf(logMessage, sizeof(logMessage), "Renaming process started.");
        logToFile(logMessage, logFilePath);
    }

    /* Core of function - renaming files using dirent */
    if ((dir = opendir(folderPath)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            char filePath[256];
            snprintf(filePath, sizeof(filePath), "%s/%s", folderPath, ent->d_name);

            struct stat fileStat;
            if (stat(filePath, &fileStat) == 0 && S_ISREG(fileStat.st_mode)) {
                if (isImageFile(ent->d_name)) {
                    char oldName[256];
                    char newName[256];

                    snprintf(oldName, sizeof(oldName), "%s/%s", folderPath, ent->d_name);

                    if (includeTimestamp) {

                        time_t t = time(NULL);
                        struct tm currentTime = *localtime(&t);
                        
                        snprintf(newName, sizeof(newName), "%s/%s%d_%d_%02d_%02d_%02d_%02d_%02d%s", folderPath, name, counter,
                        currentTime.tm_year + 1900, currentTime.tm_mon + 1, currentTime.tm_mday, currentTime.tm_hour, currentTime.tm_min,
                        currentTime.tm_sec, strrchr(ent->d_name, '.'));
                    } else {
                        snprintf(newName, sizeof(newName), "%s/%s%d%s", folderPath, name, counter, strrchr(ent->d_name, '.'));
                    }
                    
                    // Rename the file
                    if (rename(oldName, newName) == 0) {
                        printf("Renamed %s to %s\n", oldName, newName);
                        if (includeLogging) {
                            char logMessage[512];
                            snprintf(logMessage, sizeof(logMessage), "Renamed %s to %s", oldName, newName);
                            logToFile(logMessage, logFilePath);
                        }
                    } else {
                        printf("Failed to rename %s\n", oldName);
                        if (includeLogging) {
                            char logMessage[512];
                            snprintf(logMessage, sizeof(logMessage), "Failed to rename %s", oldName);
                            logToFile(logMessage, logFilePath);
                        }
                    }

                    counter++;
                }
            }
        }
        closedir(dir);
    } else {
        printf("Error opening directory.\n");
        if (includeLogging) {
        char logMessage[512];
        snprintf(logMessage, sizeof(logMessage), "Error opening directory.");
        logToFile(logMessage, logFilePath);
        }
    }
     if (includeLogging) {
        char logMessage[512];
        snprintf(logMessage, sizeof(logMessage), "Renaming process finished.");
        logToFile(logMessage, logFilePath);
    }
}

void takeNameFromTxt(const char *folderPath, int includeLogging, int includeBackup) 
{
    DIR *dir;
    struct dirent *ent;
    FILE *fptr;
    char numberInFile[32];
    long long int numberInFileInt; // Needed for incrementation of the number in file
    char logFilePath[256]; // Buffer for the log path

    if((fptr = fopen("random_name.txt", "r")) == NULL) {
        printf("Failed to open file\n");
        fclose(fptr);
        return;
    }

    if (fgets(numberInFile, sizeof(numberInFile), fptr) == NULL) {
        printf("Failed to read number from file\n");
        fclose(fptr);
        return;
    }

    fclose(fptr);

    // Check if the read content is a valid number
    char *endptr;
    numberInFileInt = strtoll(numberInFile, &endptr, 10);

    // Check for a valid number
    if (*endptr != '\0' || strspn(numberInFile, "0123456789") != strlen(numberInFile)) {
        printf("Invalid number in file\n");
        return;
    }

    /* Check if a backup is requested */ 
    if (includeBackup) {
        backupFiles(folderPath);
    }

    /* Start logging process */
    if (includeLogging) {
        char logMessage[512];

        snprintf(logFilePath, sizeof(logFilePath), "%s/rename_loggs.txt", folderPath);
        snprintf(logMessage, sizeof(logMessage), "Renaming process started.");
        logToFile(logMessage, logFilePath);
    }

    if ((dir = opendir(folderPath)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            char filePath[256];
            snprintf(filePath, sizeof(filePath), "%s/%s", folderPath, ent->d_name);

            struct stat fileStat;
            if (stat(filePath, &fileStat) == 0 && S_ISREG(fileStat.st_mode)) {
                if (isImageFile(ent->d_name)) {
                    char oldName[256];
                    char newName[256];

                    snprintf(oldName, sizeof(oldName), "%s/%s", folderPath, ent->d_name);
                    snprintf(newName, sizeof(newName), "%s/%s%s", folderPath, numberInFile, strrchr(ent->d_name, '.'));
                    
                    // Rename the file
                    if (rename(oldName, newName) == 0) {
                        printf("Renamed %s to %s\n", oldName, newName);
                        if (includeLogging) {
                            char logMessage[512];
                            snprintf(logMessage, sizeof(logMessage), "Renamed %s to %s", oldName, newName);
                            logToFile(logMessage, logFilePath);
                        }
                    } else {
                        printf("Failed to rename %s\n", oldName);
                        if (includeLogging) {
                            char logMessage[512];
                            snprintf(logMessage, sizeof(logMessage), "Failed to rename %s", oldName);
                            logToFile(logMessage, logFilePath);
                        }
                    }
                    numberInFileInt = atoll(numberInFile);
                    numberInFileInt++;
                    snprintf(numberInFile, sizeof(numberInFile), "%lld", numberInFileInt);
                }
            }
        }
        closedir(dir);
    } else {
        printf("Error opening directory.\n");
        if (includeLogging) {
        char logMessage[512];
        snprintf(logMessage, sizeof(logMessage), "Error opening directory.");
        logToFile(logMessage, logFilePath);
        }
    } 
    if (includeLogging) {
        char logMessage[512];
        snprintf(logMessage, sizeof(logMessage), "Renaming process finished.");
        logToFile(logMessage, logFilePath);
    }
    if((fptr = fopen("random_name.txt", "w")) == NULL) {
        printf("Failed to open file\n");
        if (includeLogging) {
            char logMessage[512];
            snprintf(logMessage, sizeof(logMessage), "Failed to open file: random_name.txt for writing");
            logToFile(logMessage, "rename_log.txt");
        }
        fclose(fptr);
        return;
    }
    fprintf(fptr, numberInFile);
    fclose(fptr);
}