#ifndef MAIN_FUNCTIONALITY_H
#define MAIN_FUNCTIONALITY_H

void renameFilesInFolder(const char *folderPath, const char *name, int startCounter, int includeTimestamp, int includeLogging, int includeBackup);
void takeNameFromTxt(const char *folderPath, int includeLogging, int includeBackup);

#endif