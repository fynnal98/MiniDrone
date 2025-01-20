#ifndef WIFIINPUT_H
#define WIFIINPUT_H

#include "DatabaseTool.h"
#include <WiFi.h>
#include <WiFiUdp.h>
#include <string>
#include <vector>

class WiFiInput {
public:
    WiFiInput(const char* ssid, const char* password, int port);
    void init(DatabaseTool* db); // Entferne override
    void update();
    std::string getInput();

private:
    const char* ssid;
    const char* password;
    int port;
    WiFiUDP udp;
    std::string receivedData;
};

#endif // WIFIINPUT_H
