#include "DatabaseTool.h"
#include "InputHandler.h"
#include "WiFiInput.h"

// Globale Module
DatabaseTool database;
InputHandler* inputHandler = nullptr;  // Zeiger auf die Basisklasse

void setup() {
    Serial.begin(115200);

    // Initialisiere die Datenbank
    database.init();

    // Initialisiere die Eingabemethode
    inputHandler = new WiFiInput("MiniDrone_Network", "12345678", 4210);
    inputHandler->init(&database);
}

void loop() {
    inputHandler->update();

    // Empfangene Daten ausgeben
    std::string input = inputHandler->getInput();
    if (!input.empty()) {
        Serial.print("Empfangene Daten: ");
        Serial.println(input.c_str());
    }
    delay(10);
}
