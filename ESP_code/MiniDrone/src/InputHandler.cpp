#include "InputHandler.h"
#include "WiFiInput.h"
#include "SBUSInput.h"

InputHandler* InputHandler::createHandler(DatabaseTool* db) {
    // Lese Methode aus der Datenbank
    std::string method = db->get<std::string>("input/method", "wifi");

    if (method == "wifi") {
        const char* ssid = db->get<const char*>("input/wifi/ssid", "MiniDrone_Network");
        const char* password = db->get<const char*>("input/wifi/password", "12345678");
        int port = db->get<int>("input/wifi/port", 4210);
        
        Serial.printf("WiFi-Methode gewählt. SSID: %s, Port: %d\n", ssid, port);

        return new WiFiInput(ssid, password, port);
    } 
    else if (method == "sbus") {
        int rxPin = db->get<int>("input/sbus/rxPin", 16);
        int txPin = db->get<int>("input/sbus/txPin", 17);

        Serial.printf("SBUS-Methode gewählt. RX: %d, TX: %d\n", rxPin, txPin);

        return new SBUSInput(&Serial1, rxPin, txPin);
    }
    
    Serial.println("Ungültige Eingabemethode.");
    return nullptr;
}
