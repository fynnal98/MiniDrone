#ifndef HELILOGIC_H
#define HELILOGIC_H

#include "SensorHandler.h"
#include "InputHandler.h"

class HeliLogic {
public:
    void init(DatabaseTool* db) {
        Serial.println("Helikoptermodus initialisiert.");
    }

    void update(SensorHandler* sensors, InputHandler* input) {
        Serial.println("Helikoptermodus wird aktualisiert.");
    }
};

#endif
