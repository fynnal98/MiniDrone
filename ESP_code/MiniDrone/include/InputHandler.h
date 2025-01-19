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

    static InputHandler* createHandler(DatabaseTool* db); // Factory-Methode zur Erstellung des Handlers
};

#endif
