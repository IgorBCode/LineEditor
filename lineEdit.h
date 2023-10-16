#ifndef UNTITLED_LINEEDIT_H
#define UNTITLED_LINEEDIT_H
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>

class LineEdit {
private:
    bool running;
    bool saved;
    int currentLine;
    std::string filename;
    std::vector<std::string>buffer;
    typedef struct{
        std::string command;
        std::string content;
        int index1;
        int index2;
    }userInput;
    userInput cmdData;

    void createBuffer(char* consoleIn[]);
    void printLineIndicator(int lineNum);
    void writeFileOnQ();

public:
    LineEdit();
    explicit LineEdit(char* consoleIn[]);

    void addData(userInput command);
    bool active();
    void deleteLines(userInput command);
    userInput inputHandler(std::string in);
    void jumpToLine(userInput command);
    void listLines(userInput command);
    void printWorkingLineIndicator();
    void terminate();
    void writeFile(userInput command);
};


#endif //UNTITLED_LINEEDIT_H
