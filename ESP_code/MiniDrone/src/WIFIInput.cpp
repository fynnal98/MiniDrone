#include "WiFiInput.h"

WiFiInput::WiFiInput(const char* ssid, const char* password, int port)
    : ssid(ssid), password(password), port(port) {
    Serial.println("WiFiInput Konstruktor aufgerufen.");
    }

void WiFiInput::init(DatabaseTool* db) {
    Serial.println("Initialisiere WiFiInput...");
    
    WiFi.disconnect(true, true); 
    delay(100);
    WiFi.mode(WIFI_AP);
    WiFi.printDiag(Serial);


    if (WiFi.softAP(ssid, password)) {
        Serial.println("Access Point erfolgreich gestartet.");
    } else {
        Serial.println("Fehler beim Starten des Access Points!");
    }

    if (udp.begin(port)) {
        Serial.printf("UDP-Server gestartet auf Port %d\n", port);
    } else {
        Serial.println("Fehler beim Starten des UDP-Servers!");
    }

    Serial.print("WiFiInput gestartet. IP-Adresse: ");
    Serial.println(WiFi.softAPIP());
}

void WiFiInput::update() {
    int packetSize = udp.parsePacket();
    if (packetSize > 0) {
        char incomingPacket[255];
        int len = udp.read(incomingPacket, 255);
        if (len > 0) {
            incomingPacket[len] = '\0';
            receivedData = std::string(incomingPacket);
        }
    }
}

std::string WiFiInput::getInput() {
    return receivedData;
}
