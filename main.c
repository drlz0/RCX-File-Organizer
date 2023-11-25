#define true 1
#define false 0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ctype.h>
#include <time.h>

// Function to check if a file has an image extension
int isImageFile(const char *fileName) 
{
    const char *imageExtensions[] = {".jpg", ".jpeg", ".png", ".bmp", ".mp4", ".webm", ".gif"}; // Add more image extensions if needed
    int numExtensions = sizeof(imageExtensions) / sizeof(imageExtensions[0]);

    char lowercaseFileName[256];
    strcpy(lowercaseFileName, fileName);
    for (int i = 0; i < strlen(lowercaseFileName); i++) {
        lowercaseFileName[i] = tolower(lowercaseFileName[i]);
    }

    for (int i = 0; i < numExtensions; i++) {
        if (strstr(lowercaseFileName, imageExtensions[i]) != NULL) {
            return 1; // File has an image extension
        }
    }

    return 0; // File does not have an image extension
}

// logging
void logToFile(const char *logMessage, const char *logFilePath) {
    FILE *logFile = fopen(logFilePath, "a");
    if (logFile != NULL) {
        time_t rawtime;
        struct tm *timeinfo;
        time(&rawtime);
        timeinfo = localtime(&rawtime);

        fprintf(logFile, "[%04d-%02d-%02d %02d:%02d:%02d] %s\n",
                timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday,
                timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, logMessage);

        fclose(logFile);
    } else {
        printf("Failed to open log file.\n");
    }
}

// Function to rename files in a folder
void renameFilesInFolder(const char *folderPath, const char *name, int startCounter, int includeTime, int includeLogging) 
{
    DIR *dir;
    struct dirent *ent;
    int counter = startCounter; // Initialize the counter with the startCounter value
    char logFilePath[256]; // Buffer for the log path

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

                    if (includeTime) {

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

void takeNameFromTxt(const char *folderPath, int includeLogging) 
{
    DIR *dir;
    struct dirent *ent;
    FILE *fptr;
    char numberInFile[32];
    long long int numberInFileInt; // Needed for incrementation of the number in file
    char logFilePath[256]; // Buffer for the log path

    if((fptr = fopen("randomname.txt", "r")) == NULL) {
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
    if((fptr = fopen("randomname.txt", "w")) == NULL) {
        printf("Failed to open file\n");
        if (includeLogging) {
            char logMessage[512];
            snprintf(logMessage, sizeof(logMessage), "Failed to open file: randomname.txt for writing");
            logToFile(logMessage, "rename_log.txt");
        }
        fclose(fptr);
        return;
    }
    fprintf(fptr, numberInFile);
    fclose(fptr);
}

int main(int argc, char *argv[]) 
{
    if (argc < 3) {
        printf("Usage: %s <folder_path> -r <new_filename> [-s <start_counter>] [-d] [-l]\n", argv[0]);
        printf("or\n");
        printf("Usage: %s <folder_path> -t [-l]\n", argv[0]);
        return 1;
    }

    char folderPath[256];
    char name[256];
    int startCounter = 0; // Default startCounter value
    int includeTime = false;
    int includeLogging = false;
    int inputFlagChecker = false;
    int inputRename = false;

    // Parse command-line arguments
    strncpy(folderPath, argv[1], sizeof(folderPath) - 1);

    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "-r") == 0 && i + 1 < argc) {
            strncpy(name, argv[i + 1], sizeof(name) - 1);
            inputRename = true;
        } else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            startCounter = atoi(argv[i + 1]);
        } else if (strcmp(argv[i], "-d") == 0) {
            includeTime = true;
        } else if (strcmp(argv[i], "-l") == 0) {
            includeLogging = true;
        } else if (strcmp(argv[i], "-t") == 0) {
            inputFlagChecker = true;
        }
    }
    
    /* Protection for incorrect use of flags */
    if (
        (inputFlagChecker && (includeTime || startCounter > 0)) 
        || 
        (!inputFlagChecker && (name[0] == '\0' || (includeTime && includeLogging))) 
        ||
        (inputFlagChecker && inputRename)   
        ) 
    {
        printf("Invalid combination of flags!\n");
        return 1;
    }

    if (inputFlagChecker) {
        takeNameFromTxt(folderPath, includeLogging);
    } else {
        renameFilesInFolder(folderPath, name, startCounter, includeTime, includeLogging);
    }

    return 0;
}
