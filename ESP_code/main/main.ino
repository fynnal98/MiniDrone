#include <WiFi.h>
#include <WiFiUdp.h>

// WLAN-Zugangsdaten
const char* ssid = "ESP32_Netzwerk";       // Name des WLANs (Access Point)
const char* password = "12345678";         // Passwort des WLANs

// UDP-Konfiguration
WiFiUDP udp;
const int localPort = 4210;                // Port, auf dem der ESP32 lauscht

void setup() {
  Serial.begin(115200);

  // WLAN als Access Point starten
  Serial.println("Starte Access Point...");
  WiFi.softAP(ssid, password, 1);

  // IP-Adresse ausgeben
  Serial.println("Access Point gestartet.");
  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("IP-Adresse: ");
  Serial.println(WiFi.softAPIP());         // Standard-IP: 192.168.4.1

  // UDP-Server starten
  udp.begin(localPort);
  Serial.print("UDP-Server gestartet auf Port: ");
  Serial.println(localPort);
}

void loop() {
    int packetSize = udp.parsePacket();
    if (packetSize > 0) {
        char incomingPacket[255];
        int len = udp.read(incomingPacket, 255);
        if (len > 0) {
            incomingPacket[len] = '\0';
        }
        Serial.print("Paketinhalt: ");
        Serial.println(incomingPacket);
    }
  delay(100);
}

