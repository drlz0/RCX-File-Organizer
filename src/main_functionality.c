#include "../include/main_functionality.h"
#include "../include/utilities.h"
#include "../include/main.h"
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>

// Function to rename files in a folder
void renameFilesInFolder(const char *folderPath, const char *name, int startCounter, int includeTimestamp, int includeLogging, int includeBackup) 
{
    DIR *dir;
    struct dirent *ent;
    int counter = startCounter;
    char logFilePath[256];

    if (includeLogging) {
        snprintf(logFilePath, sizeof(logFilePath), "%s/rename_loggs.txt", folderPath);
        logToFile("Renaming process started.", logFilePath);
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

                    if (includeTimestamp) {
                        time_t t = time(NULL);
                        struct tm currentTime = *localtime(&t);
                        snprintf(newName, sizeof(newName), "%s/%s%d_%d_%02d_%02d_%02d_%02d_%02d%s", folderPath, name, counter,
                                 currentTime.tm_year + 1900, currentTime.tm_mon + 1, currentTime.tm_mday, currentTime.tm_hour, currentTime.tm_min,
                                 currentTime.tm_sec, strrchr(ent->d_name, '.'));
                    } else {
                        snprintf(newName, sizeof(newName), "%s/%s%d%s", folderPath, name, counter, strrchr(ent->d_name, '.'));
                    }

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
        printf("Error opening directory: %s\n", folderPath);
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

// Function to take name from a text file and rename files
void takeNameFromTxt(const char *folderPath, int includeLogging, int includeBackup) 
{
    DIR *dir;
    struct dirent *ent;
    FILE *fptr;
    char numberInFile[32];
    long long int numberInFileInt;
    char logFilePath[256];

    const char *filename = "../random_name.txt";

    if ((fptr = fopen(filename, "r")) == NULL) {
        printf("Failed to open file %s: %s\n", filename, strerror(errno));
        return;
    }

    if (fgets(numberInFile, sizeof(numberInFile), fptr) == NULL) {
        printf("Failed to read number from file %s\n", filename);
        fclose(fptr);
        return;
    }

    fclose(fptr);

    char *endptr;
    numberInFileInt = strtoll(numberInFile, &endptr, 10);

    if (*endptr != '\0' || strspn(numberInFile, "0123456789") != strlen(numberInFile)) {
        printf("Invalid number in file %s\n", filename);
        return;
    }

    if (includeLogging) {
        snprintf(logFilePath, sizeof(logFilePath), "%s/rename_loggs.txt", folderPath);
        logToFile("Renaming process started.", logFilePath);
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
        printf("Error opening directory: %s\n", folderPath);
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
    if ((fptr = fopen(filename, "w")) == NULL) {
        printf("Failed to open file %s for writing: %s\n", filename, strerror(errno));
        if (includeLogging) {
            char logMessage[512];
            snprintf(logMessage, sizeof(logMessage), "Failed to open file: %s for writing", filename);
            logToFile(logMessage, logFilePath);
        }
        return;
    }
    fprintf(fptr, "%s", numberInFile);
    fclose(fptr);
}
