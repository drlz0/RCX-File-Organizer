#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ctype.h>

// Function to check if a file has an image extension
int isImageFile(const char *fileName) {
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

// Function to rename files in a folder
void renameFilesInFolder(const char *folderPath, const char *name, int startCounter) {
    DIR *dir;
    struct dirent *ent;
    int counter = startCounter; // Initialize the counter with the startCounter value

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
                    snprintf(newName, sizeof(newName), "%s/%s%d%s", folderPath, name, counter, strrchr(ent->d_name, '.'));

                    // Rename the file
                    if (rename(oldName, newName) == 0) {
                        printf("Renamed %s to %s\n", oldName, newName);
                    } else {
                        printf("Failed to rename %s\n", oldName);
                    }

                    counter++;
                }
            }
        }
        closedir(dir);
    } else {
        printf("Error opening directory.\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Usage: %s <folder_path> -r <new_filename> [-s <start_counter>]\n", argv[0]);
        return 1;
    }

    char folderPath[256];
    char name[256];
    int startCounter = 0; // Default startCounter value

    // Parse command-line arguments
    strncpy(folderPath, argv[1], sizeof(folderPath) - 1);

    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "-r") == 0 && i + 1 < argc) {
            strncpy(name, argv[i + 1], sizeof(name) - 1);
        } else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            startCounter = atoi(argv[i + 1]);
        }
    }

    renameFilesInFolder(folderPath, name, startCounter);

    return 0;
}
