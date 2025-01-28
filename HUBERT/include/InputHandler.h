#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "WiFiInput.h"
#include "SBUSInput.h"
#include "DatabaseTool.h"
#include <map>
#include <string>
#include <vector>


class InputHandler {
public:
    static InputHandler* createHandler(DatabaseTool* db);
    void init(DatabaseTool* db);
    void update();
    std::string getInput();
    int getChannelValue(const std::string& channel);

private:
    std::map<std::string, int> channelMapping;
    std::map<std::string, int> channelValues;
    WiFiInput* wifiInput = nullptr;
    SBUSInput* sbusInput = nullptr;
};

#endif // INPUTHANDLER_H
