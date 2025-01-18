#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <string>
#include "DatabaseTool.h"

class InputHandler {
public:
    virtual void init(DatabaseTool* db) = 0;
    virtual void update() = 0;
    virtual std::string getInput() = 0;
    virtual ~InputHandler() {}

protected:
    InputHandler* currentInput = nullptr;  // Zeiger auf die aktuelle Eingabemethode
};

#endif
