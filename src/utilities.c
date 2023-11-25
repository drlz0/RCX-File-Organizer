#include "utilities.h"
#include "main.h"

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
int copyFile(const char *sourcePath, const char *destinationPath) 
{
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
int backupFiles(const char *folderPath) 
{
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