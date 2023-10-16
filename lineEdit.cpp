#include "lineEdit.h"

LineEdit::LineEdit() {
    running = true;
    saved = true;
    currentLine = 1;
    filename = "";
    cmdData.command = "";
    cmdData.content = "";
    cmdData.index1 = -999;
    cmdData.index2 = -999;
}

LineEdit::LineEdit(char **consoleIn) {
    running = true;
    saved = true;
    createBuffer(consoleIn);
    currentLine = buffer.size() + 1;
    cmdData.command = "";
    cmdData.content = "";
    cmdData.index1 = -999;
    cmdData.index2 = -999;
}

bool LineEdit::active() {
    return running;
}

void LineEdit::addData(LineEdit::userInput command) {
    std::string temp = command.content.empty() ? "" : command.content;
    // handle empty buffer
    if (buffer.empty()) {
        buffer.push_back(command.content);
    }
    else {
        // Edge case: after loading program with a file, currentline is out of bounds
        // Not allowing user to go further out of bounds with append
        if (command.command == "A" && currentLine == buffer.size() + 1) {
            std::cout << "Only insertion is allowed at this index." << std::endl;
        }
        // append after current line
        else if (command.command == "A") {
            buffer.insert(buffer.begin() + currentLine, temp);
        }
        // Insert for the above-mentioned edge case
        else if (command.command == "I" && currentLine == buffer.size() + 1) {
            buffer.push_back(temp);
        }
        // insert at current line
        else {
            buffer.insert(buffer.begin() + (currentLine - 1) , temp);
        }
    }
    saved = false;
}

void LineEdit::createBuffer(char **consoleIn) {
    // if filename given read in
    if (consoleIn[1]) {
        filename = consoleIn[1];
        std::string line;
        std::fstream inFile;
        inFile.open(filename, std::ios::in);

        while (std::getline(inFile, line)) {
            buffer.push_back(line);
        }
        inFile.close();
    }
    else {
        filename = "";
    }
}

void LineEdit::deleteLines(LineEdit::userInput command) {
    // handle empty file
    if (buffer.empty()){
        std::cout << "File empty." << std::endl;
    }
    else {
        // no args given: erase current line
        if (command.index1 == -999 && command.index2 == -999) {
            buffer.erase(buffer.begin() + (currentLine - 1));

            // if current line was deleted go to next available line
            if (currentLine == buffer.size() + 1 && currentLine != 1) {
                currentLine = buffer.size();
            }
            else if (buffer.empty()) {
                currentLine = 1;
            }
            saved = false;
        }
        // invalid characters given for the index
        else if (command.index1 == -555 || command.index2 == -555) {
            std::cout << "Invalid index" << std::endl;
        }
        // index1 given: delete specified line
        else if (command.index2 == -999) {
            if (command.index1 < 1 || command.index1 > buffer.size()) {
                std::cout << "Index out of bounds." << std::endl;
            }
            else {
                buffer.erase(buffer.begin() + (command.index1 - 1));

                //if current line was deleted go to next available line
                if (currentLine == command.index1) {
                    if (currentLine > buffer.size()) {
                        currentLine = buffer.size();
                    }
                }
                else if (buffer.empty()) {
                    currentLine = 1;
                }
            }
            saved = false;
        }
        // index1 & 2 given: delete range
        else {
            // ensure startIndex < endIndex
            int startIndex = std::min(command.index1, command.index2);
            int endIndex = std::max(command.index1, command.index2);

            if (startIndex < 1 || endIndex > buffer.size())
            {
                std::cout << "Index out of bounds." << std::endl;
            }
            else {
                // Verify user wants to delete lines
                std::cout << "Delete lines " << startIndex << " to " << endIndex << "? (Y/N): ";
                std::string choice;
                std::getline(std::cin, choice);
                if (choice == "y" || choice == "Y") {
                    // delete lines; working form endIndex to startIndex
                    for (int i = endIndex - 1; i >= startIndex - 1; i--){
                        buffer.erase(buffer.begin() + i);
                    }

                    // if current line was deleted go to next available line
                    if (buffer.empty()) {
                        currentLine = 1;
                    }
                    else if (currentLine >= command.index1 && currentLine <= command.index2) {
                        if (currentLine > buffer.size()) {
                            currentLine = buffer.size();
                        }
                    }
                    saved = false;
                }
                else {
                    std::cout << "Delete cancelled." << std::endl;
                }
            }
        }
    }
}

// This function takes an input string and parses it out into a object
// return object contains: string command, int index2, int index2, string content.
LineEdit::userInput LineEdit::inputHandler(std::string in) {
    std::istringstream iss(in);
    std::istringstream checker(in);
    std::string inputText;
    int indexHolder[2] = {-999, -999};
    std::string indexCheck[2] = {"", ""};
    auto *newCommand = new LineEdit::userInput;
    // code for checker >> newCommand->command;
    iss >> newCommand->command;
    checker >> newCommand->command;

    // I & A commands take no indexes, and have only the command and content
    if (newCommand->command == "I" || newCommand->command == "A" || newCommand->command == "W") {
        //  delete the command prefixes to leave only the text input after commands
        std::string tempContent = in;
        // edge case: check for single char command ie "L", "I"
        if ((tempContent.find_first_of(" \t") + 1) == 0) {
            tempContent = "";
        }
        //  delete the command prefixes to leave only the text input after commands
        else {
            tempContent = tempContent.substr(tempContent.find_first_of(" \t") + 1);
        }
        newCommand->content = tempContent;
    }
    // process indexes for commands that require them
    else {
        // Load indexes
        for (int i = 0; i < 2; i++) {
            iss >> indexHolder[i];
            checker >> indexCheck[i];
        }
        // check if indexes given are digits, if not assign flag value of -555
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < indexCheck[i].size(); j++) {
                if (!std::isdigit(indexCheck[i][j]) && indexHolder[0] != -1) {
                    indexHolder[i] = -555;
                }
            }
        }
        newCommand->index1 = indexHolder[0];
        newCommand->index2 = indexHolder[1];
    }
    return *newCommand;
}

void LineEdit::jumpToLine(LineEdit::userInput command) {
    // no args given or invalid character given
    if (command.index1 == -999 || command.index1 == -555 || command.index2 != -999 || (command.index1 > buffer.size() && command.index1 != -1) ) {
        std::cout << "Invalid index." << std::endl;
    }
        // go to last line
    else if (command.index1 == -1) {
        currentLine = buffer.size();
    }
        // go to first line
    else if (command.index1 == 0) {
        currentLine = 1;
    }
        // go to specific line
    else {
        currentLine = command.index1;
    }
}

void LineEdit::listLines(LineEdit::userInput command) {
    // no args: print full buffer
    int lineCounter = 1;
    // No args given
    if (command.index1 == -999 && command.index2 == -999) {
        for (std::string line : buffer) {
            printLineIndicator(lineCounter);
            std::cout << line << std::endl;
            lineCounter++;
        }
    }
        // invalid characters given for the index
    else if (command.index1 == -555 || command.index2 == -555) {
        std::cout << "Invalid index" << std::endl;
    }
        // only index1 given: print specified line
    else if (command.index2 == -999) {
        if (command.index1 < 1 || command.index1 > buffer.size()) {
            std::cout << "Invalid index." << std::endl;
        }
        else {
            printLineIndicator(command.index1);
            std::cout << buffer[command.index1 - 1] << std::endl;
        }
    }
        // index1 & 2 given: print range
    else {
        // ensure start < end
        int startIndex = std::min(command.index1, command.index2);
        int endIndex = std::max(command.index1, command.index2);
        lineCounter = startIndex;
        startIndex--;

        if (startIndex < 0 || endIndex > buffer.size()) {
            std::cout << "Invalid range." << std::endl;
        }
        else {
            for (int i = startIndex; i < endIndex; i++){
                printLineIndicator(lineCounter);
                std::cout << buffer[i] << std::endl;
                lineCounter++;
            }
        }
    }
}

// line indicator for printed lines
void LineEdit::printLineIndicator(int lineNum) {
    lineNum == currentLine ? std::cout << lineNum << "*| " : std::cout << lineNum << "| ";
}

// line indicator for CURRENT WORKING line
void LineEdit::printWorkingLineIndicator() {
    saved ? std::cout << currentLine << "> " : std::cout << currentLine << "!> ";
}

// Terminate program, prompt user to save if there are any unsaved changes.
void LineEdit::terminate() {
    if (saved) {
        running = false;
    }
    else {
        std::cout << "You have unsaved changes. Save before exit? (Y/N): ";
        std::string saveFile;
        std::getline(std::cin, saveFile);

        if (saveFile == "Y" || saveFile == "y") {
            writeFileOnQ();
            running = false;
        }
        else if (saveFile == "N" || saveFile == "n") {
            running = false;
        }
        else {
            std::cout << "Invalid command." << std::endl;
            running = true;
        }

    }
}

void LineEdit::writeFile(userInput command) {
    std::fstream writeFile;
    if (filename == "" && command.content == "") {
        std::cout << "Save as: ";
        std::getline(std::cin, filename);
    }
    else if (command.content != "") {
        filename = command.content;
    }

    writeFile.open(filename, std::ios::out);

    for (std::string line : buffer) {
        writeFile << line << std::endl;
    }
    writeFile.close();
    saved = true;
}

void LineEdit::writeFileOnQ() {
    std::fstream writeFile;
    if (filename.empty()) {
        std::cout << "Save as: ";
        std::getline(std::cin, filename);
    }
    writeFile.open(filename, std::ios::out);

    for (std::string line : buffer) {
        writeFile << line << std::endl;
    }
    writeFile.close();
}
