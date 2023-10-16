# LineEditor
This is a line editor that allows you to create/edit/save txt files. The editing works by deleting lines and adding lines, you cannot edit an existing line.<br>
<br>
## To open file from disk: <br>
File must be in the same directory as the executable. Provide filename when starting program.<br>
Example: ./foo fileToOpen.txt
<br>
<br>
## Commands:<br>
W <filename> - Writes the file to disk<br>
J <line number> - Jump to any line in the file. J 0 jumps to end, J -1 jumps to beginning.<br>
I <some text> - Insert text at current line. <br>
A <some text> - Insert text after current line.<br>
L <line number> <line number> - L: list all lines, L <line number>: list given line, L <line number> <line number>: list lines in range<br>
D <line number> <line number> - D: delete current line, D <line number>: delete given line, L <line number> <line number>: delete lines in range<br>
Q - Quit program<br>
