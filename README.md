# RCX-File-Organizer
## Organize your files
works on Linux (tested only on Debian) and Windows 10<br>
available options for now is changing names of all images in folder in a way that it gets incremented by 1.<br>
# How to
_./main(.exe if windows) </path/to/folder> -r <new_filename>_ <br>
this will rename files to filename_0, filename_1, filename_2 and so. <br>
### flags
-r <new_filename> : rename the files in the folder with given name incrementaly<br>
_./main(.exe if windows) </path/to/folder>  -r <new_filename>__ <br>
this will rename files to filename_0, filename_1, filename_2 and so. <br>
-s <start_number> : increment from given number <br>
_./main(.exe if windows) </path/to/folder>  -r <new_filename> -s 34_ <br>
this will rename files to filename_34, filename_35, filename_36 and so. <br>
-t : get number from txt file, rename file according to this number, increment number in file by 1 and rename another file in folder with new number <br>
_./main(.exe if windows) </path/to/folder>  -t_ <br>
If number in file is 12345, the files will be renamed to 12345, 12346, 12347 and so. <br>
