## Organize your files
A command-line utility written in C to batch rename files in a directory. The program allows users to rename files with a specified prefix, start counter, and additional options such as including timestamps, logging, and backup.<br>
Works on Linux (tested only on Debian) and Windows 10.<br>
## Features
- Rename multiple files in a directory with a specified prefix and start counter<br>
- Timestamps: Include timestamps in the new file names<br>
- Logging: Log the renaming process, including success and failure messages<br>
- Backup: Create a backup of the files before renaming<br>
## How to
```
./build/rcx_fo.exe <folder_path> -r <new_filename> [-s <start_counter>] [-d] [-l] [-b]
```
or
```
./build/rcx_fo.exe <folder_path -t [-l] [-b]
```
## Usage
- **<folder_path>** - Folder which content files will be renamed
- **-r <new_filename>** - Rename the files in the folder with given name incrementaly. For example -r filename_ will rename files to filename_0, filename_1, filename_2 and so.
- **-s <start_number>** - Increment from given number. For example -s 34 will rename files to filename_34, filename_35, filename_36 and so.
- **-t** - Get number from text file, rename file according to this number, increment number in file by 1 and rename another file in folder with new number. For example if number in file is 12345, the files will be renamed to 12345, 12346, 12347 and so.
- **-d** - Include timestamps in the new filenames. This will rename files to filename_x_year_month_day_hour_min_sec.
- **-l** - Enable logging of the renaming process. Logs text file will be created in the same directory as renamed files.
- **-b** - Create a backup of files before renaming. Backup folder will be created in the same directory as renamed files.

