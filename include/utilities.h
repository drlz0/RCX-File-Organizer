#ifndef UTILITIES_H
#define UTILITIES_H

int isImageFile(const char *fileName);
void logToFile(const char *logMessage, const char *logFilePath);
int copyFile(const char *sourcePath, const char *destinationPath);
int backupFiles(const char *folderPath);

#endif