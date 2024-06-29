#include "../include/main.h"
#include "../include/main_functionality.h"
#include "../include/utilities.h"

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
