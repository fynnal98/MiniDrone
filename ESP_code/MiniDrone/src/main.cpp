#include "DatabaseTool.h"
#include "InputHandler.h"
#include "SensorHandler.h"
#include "ModeManager.h"

// Globale Module
DatabaseTool database;
InputHandler* inputHandler = nullptr;
SensorHandler sensorHandler; 
ModeManager modeManager(&database);

void setup() {
    Serial.begin(115200);

    database.init();

    inputHandler = InputHandler::createHandler(&database);
    if (inputHandler) {
        Serial.println("InputHandler erfolgreich erstellt.");
        inputHandler->init(&database); 
    } else {
        Serial.println("Fehler: Keine gültige Eingabemethode gefunden.");
    }


    sensorHandler.begin(&database);

    modeManager.init();
}

void loop() {
    if (inputHandler) {
        inputHandler->update();
    }
    sensorHandler.update();    
    modeManager.update(&sensorHandler, inputHandler);

    delay(700);
}
