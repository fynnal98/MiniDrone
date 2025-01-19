#ifndef SBUSINPUT_H
#define SBUSINPUT_H

#include "InputHandler.h"
#include "sbus.h"  // Einbinden der SBUS-Bibliothek

class SBUSInput : public InputHandler {
public:
    SBUSInput(HardwareSerial* serial, int rxPin, int txPin);
    void init(DatabaseTool* db) override;
    void update() override;
    std::string getInput() override;

private:
    bfs::SbusRx sbus;
    std::string receivedData;
};

#endif
