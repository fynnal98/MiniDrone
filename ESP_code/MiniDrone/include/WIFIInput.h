#ifndef WIFIINPUT_H
#define WIFIINPUT_H

#include "InputHandler.h"
#include <WiFi.h>
#include <WiFiUdp.h>
#include <string>

class WiFiInput : public InputHandler {
public:
    WiFiInput(const char* ssid, const char* password, int port);
    void init(DatabaseTool* db) override;  // Korrigierte Signatur
    void update() override;
    std::string getInput() override;

private:
    WiFiUDP udp;
    const char* ssid;
    const char* password;
    int port;
    std::string receivedData;
};

#endif
