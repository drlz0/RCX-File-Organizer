#include "../include/utilities.h"
#include "../include/main.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>

// Function to check if a file has an image extension
int isImageFile(const char *fileName) 
{
    const char *imageExtensions[] = {".jpg", ".jpeg", ".png", ".bmp", ".mp4", ".webm", ".gif"};
    int numExtensions = sizeof(imageExtensions) / sizeof(imageExtensions[0]);

    char lowercaseFileName[256];
    strncpy(lowercaseFileName, fileName, sizeof(lowercaseFileName) - 1);
    lowercaseFileName[sizeof(lowercaseFileName) - 1] = '\0';
    for (int i = 0; i < strlen(lowercaseFileName); i++) {
        lowercaseFileName[i] = tolower(lowercaseFileName[i]);
    }

    for (int i = 0; i < numExtensions; i++) {
        if (strstr(lowercaseFileName, imageExtensions[i]) != NULL) {
            return 1;
        }
    }

    return 0;
}

// Logging implementation
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
        printf("Failed to open log file: %s\n", logFilePath);
    }
}

// File copying function
int copyFile(const char *sourcePath, const char *destinationPath) 
{
    FILE *sourceFile = fopen(sourcePath, "rb");
    if (sourceFile == NULL) {
        printf("Failed to open source file '%s' for reading.\n", sourcePath);
        return 0;
    }

    FILE *destinationFile = fopen(destinationPath, "wb");
    if (destinationFile == NULL) {
        fclose(sourceFile);
        printf("Failed to open destination file '%s' for writing.\n", destinationPath);
        return 0;
    }

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
