# LineEditor
This is a line editor that allows you to create/edit/save txt files. The editing works by deleting lines and adding lines, you cannot edit an existing line.

Commands:
W <filename> - Writes the file to disk
J <line number> - Jump to any line in the file. J 0 jumps to end, J -1 jumps to beginning.
I <some text> - Insert text at current line. 
A <some text> - Insert text after current line.
L <line number> <line number> - L: list all lines, L <line number>: list given line, L <line number> <line number>: list lines in range
D <line number> <line number> - D: delete current line, D <line number>: delete given line, L <line number> <line number>: delete lines in range
Q - Quit program
