# RCX-File-Organizer
## Organize your files
works on linux (Debian 12) and windows 10
available options for now is changing names of all images in folder in a way that it gets incremented by 1.<br>
./main(.exe if windows) /path/to/folder filename_ <br>
this will rename files to filename_0, filename_1, filename_2 and so. <br>
### flags
-c [number] <br> 
increment number starting from given number <br>
./main(.exe if windows) /path/to/folder filename_ -c 34 <br>
this will rename files to filename_34, filename_35, filename_36 and so. <br>
