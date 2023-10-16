// Igor Baranov
// Line editor program


#include <iostream>
#include "lineEdit.h"

int main(int argc, char* argv[]) {
    std::string input;
    LineEdit program(argv);

    std::cout << "Enter a command: " << std::endl;

    while (program.active()) {
        program.printWorkingLineIndicator();
        std::getline(std::cin, input);
        auto commandData = program.inputHandler(input);

        if (commandData.command == "W") {
            program.writeFile(commandData);
        }
        else if (commandData.command == "J") {
            program.jumpToLine(commandData);
        }
        else if  (commandData.command == "I" || commandData.command == "A") {
            program.addData(commandData);
        }
        else if (commandData.command == "L") {
            program.listLines(commandData);
        }
        else if (commandData.command == "D") {
            program.deleteLines(commandData);
        }
        else if (commandData.command == "Q") {
            program.terminate();
        }
        else {
            std::cout << "Invalid command." << std::endl;
        }
    }
    return 0;
}
