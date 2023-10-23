# RCX-File-Organizer
## Organize your files
works on linux (Debian 12) and windows 10<br>
available options for now is changing names of all images in folder in a way that it gets incremented by 1.<br>
# How to
_./main(.exe if windows) /path/to/folder_ <br>
this will rename files to filename_0, filename_1, filename_2 and so. <br>
### flags
-r [filename] : rename the files in the folder <br>
_./main(.exe if windows) /path/to/folder -r filename__ <br>
this will rename files to filename_0, filename_1, filename_2 and so. <br>
-s [number] : increment from given number <br>
_./main(.exe if windows) /path/to/folder -r filename -s 34_ <br>
this will rename files to filename_34, filename_35, filename_36 and so. <br>
