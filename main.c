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

/* Loggin implementation */
void logToFile(const char *logMessage, const char *logFilePath) 
{
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

/* File copying function */
int copyFile(const char *sourcePath, const char *destinationPath) {
    // Open the source file for reading
    FILE *sourceFile = fopen(sourcePath, "rb");
    if (sourceFile == NULL) {
        printf("Failed to open source file '%s' for reading.\n", sourcePath);
        return 0; 
    }

    // Open the destination file for writing
    FILE *destinationFile = fopen(destinationPath, "wb");
    if (destinationFile == NULL) {
        fclose(sourceFile);
        printf("Failed to open destination file '%s' for writing.\n", destinationPath);
        return 0;
    }

    // Read and write the file content
    char buffer[1024];
    size_t bytesRead;

    while ((bytesRead = fread(buffer, 1, sizeof(buffer), sourceFile)) > 0) {
        if (fwrite(buffer, 1, bytesRead, destinationFile) != bytesRead) {
            printf("Error writing to destination file '%s'.\n", destinationPath);
            fclose(sourceFile);
            fclose(destinationFile);
            return 0; 
        }
    }

    fclose(sourceFile);
    fclose(destinationFile);

    return 1; 
}

/* Backup implementation */
int backupFiles(const char *folderPath) {
    // Create a timestamp for the backup folder
    time_t t = time(NULL);
    struct tm currentTime = *localtime(&t);

    char timestamp[50];
    snprintf(timestamp, sizeof(timestamp), "backup_%d_%02d_%02d_%02d_%02d_%02d",
             currentTime.tm_year + 1900, currentTime.tm_mon + 1, currentTime.tm_mday,
             currentTime.tm_hour, currentTime.tm_min, currentTime.tm_sec);

    // Concatenate the backup folder name with the timestamp
    char backupFolderPath[256];
    snprintf(backupFolderPath, sizeof(backupFolderPath), "%s/%s", folderPath, timestamp);

    // Create the backup folder
    if (mkdir(backupFolderPath) != 0) {
        printf("Failed to create backup folder.\n");
        return 0;
    }

    // Open the source folder
    DIR *sourceDir;
    if ((sourceDir = opendir(folderPath)) == NULL) {
        printf("Failed to open source folder '%s' for backup.\n", folderPath);
        return 0;
    }

    struct dirent *ent;
    while ((ent = readdir(sourceDir)) != NULL) {
        // Skip "." and ".."
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
            continue;
        }

        char sourcePath[256];
        char backupPath[256];

        snprintf(sourcePath, sizeof(sourcePath), "%s/%s", folderPath, ent->d_name);
        snprintf(backupPath, sizeof(backupPath), "%s/%s", backupFolderPath, ent->d_name);

        if (copyFile(sourcePath, backupPath) != 1) {
            printf("Failed to copy file '%s' to backup folder.\n", ent->d_name);
        } else {
            printf("Copied '%s' to backup folder.\n", ent->d_name);
        }
    }

    closedir(sourceDir);

    printf("Backup completed successfully at: %s\n", backupFolderPath);
    return 1;
}

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

int main(int argc, char *argv[]) 
{
    if (argc < 3) {
        printf("Usage: %s <folder_path> -r <new_filename> [-s <start_counter>] [-d] [-l] [-b]\n", argv[0]);
        printf("or\n");
        printf("Usage: %s <folder_path> -t [-l] [-b]\n", argv[0]);
        return 1;
    }

    char folderPath[256];
    char name[256];
    int startCounter = 0;
    int includeTimestamp = false;
    int includeLogging = false;
    int inputFlagChecker = false;
    int inputRename = false;
    int includeBackup = false;

    // Parse command-line arguments
    strncpy(folderPath, argv[1], sizeof(folderPath) - 1);

    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "-r") == 0 && i + 1 < argc) {
            strncpy(name, argv[i + 1], sizeof(name) - 1);
            inputRename = true;
        } else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            startCounter = atoi(argv[i + 1]);
        } else if (strcmp(argv[i], "-d") == 0) {
            includeTimestamp = true;
        } else if (strcmp(argv[i], "-l") == 0) {
            includeLogging = true;
        } else if (strcmp(argv[i], "-t") == 0) {
            inputFlagChecker = true;
        } else if (strcmp(argv[i], "-b") == 0) {
            includeBackup = true;
        }
    }
    
    /* Protection for incorrect use of flags */
    if (
        (inputFlagChecker && (includeTimestamp || startCounter > 0)) 
        || 
        (!inputFlagChecker && (name[0] == '\0' && (includeTimestamp && includeLogging))) 
        ||
        (inputFlagChecker && inputRename)   
        ) 
    {
        printf("Invalid combination of flags!\n");
        return 1;
    }

    if (inputFlagChecker) {
        takeNameFromTxt(folderPath, includeLogging, includeBackup);
    } else {
        renameFilesInFolder(folderPath, name, startCounter, includeTimestamp, includeLogging, includeBackup);
    }

    return 0;
}
