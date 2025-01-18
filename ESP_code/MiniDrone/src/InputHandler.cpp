#include "InputHandler.h"
#include "WiFiInput.h"

void InputHandler::init(DatabaseTool* db) {
    // Lese Methode aus der Datenbank
    std::string method = db->get<std::string>("input/method", "wifi");

    if (method == "wifi") {
        const char* ssid = db->get<const char*>("input/wifi/ssid", nullptr);
        const char* password = db->get<const char*>("input/wifi/password", nullptr);
        int port = db->get<int>("input/wifi/port", 4210);

        currentInput = new WiFiInput(ssid, password, port);  // Erstelle WiFiInput
    }

    if (currentInput) {
        currentInput->init(db);  // Initialisiere die gewählte Eingabemethode
    } else {
        Serial.println("Keine gültige Eingabemethode gefunden.");
    }
}

void InputHandler::update() {
    if (currentInput) {
        currentInput->update();  // Aktualisiere die Eingabedaten
    }
}

std::string InputHandler::getInput() {
    if (currentInput) {
        return currentInput->getInput();  // Rückgabe der empfangenen Daten
    }
    return "";
}
