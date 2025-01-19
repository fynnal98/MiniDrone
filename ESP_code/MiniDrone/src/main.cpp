#include "DatabaseTool.h"
#include "InputHandler.h"
#include "SensorHandler.h"

// Globale Module
DatabaseTool database;
InputHandler* inputHandler = nullptr;
SensorHandler sensorHandler; 

void setup() {
    Serial.begin(115200);

    database.init();

    inputHandler = InputHandler::createHandler(&database);
    if (inputHandler) {
        Serial.println("InputHandler erfolgreich erstellt.");
        inputHandler->init(&database);  // WiFiInput::init sollte hier aufgerufen werden
    } else {
        Serial.println("Fehler: Keine gültige Eingabemethode gefunden.");
    }


    sensorHandler.begin(&database);
}

void loop() {
    if (inputHandler) {
        inputHandler->update();

        std::string input = inputHandler->getInput();
        if (!input.empty()) {
            Serial.print("Empfangene Daten: ");
            Serial.println(input.c_str());
        } else {
            Serial.println("Keine Eingabedaten empfangen.");
        }

        sensorHandler.update();    
        sensorHandler.printData();    
    } else {
        Serial.println("InputHandler nicht verfügbar.");
    }
    delay(1000);
}
