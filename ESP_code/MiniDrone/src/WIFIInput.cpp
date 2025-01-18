#include "WiFiInput.h"

WiFiInput::WiFiInput(const char* ssid, const char* password, int port)
    : ssid(ssid), password(password), port(port) {}

void WiFiInput::init(DatabaseTool* db) {
    // WiFi Access Point starten
    WiFi.softAP(ssid, password);
    udp.begin(port);

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
