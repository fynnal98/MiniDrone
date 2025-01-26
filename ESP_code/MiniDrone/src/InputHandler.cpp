#include "InputHandler.h"
#include <sstream>


InputHandler* InputHandler::createHandler(DatabaseTool* db) {
    InputHandler* handler = new InputHandler();
    handler->init(db);
    return handler;
}

void InputHandler::init(DatabaseTool* db) {
    std::string method = db->get<std::string>("input/method", "wifi");

    if (method == "wifi") {
        const char* ssid = db->get<const char*>("input/wifi/ssid", "MiniDrone_Network");
        const char* password = db->get<const char*>("input/wifi/password", "12345678");
        int port = db->get<int>("input/wifi/port", 4210);
        wifiInput = new WiFiInput(ssid, password, port);
        wifiInput->init(db);
    } else if (method == "sbus") {
        int rxPin = db->get<int>("input/sbus/rxPin", 16);
        int txPin = db->get<int>("input/sbus/txPin", 17);
        sbusInput = new SBUSInput(&Serial1, rxPin, txPin);
        sbusInput->init(db);
    }

    std::string mode = db->get<std::string>("mode/type", "drone");

    // Dynamische Kanalzuordnung basierend auf dem aktuellen Modus
    std::string channelPath = "mode/" + mode + "/Channels";
    JsonVariant channels = db->resolvePath(channelPath.c_str());

    if (channels.is<JsonObject>()) {
        for (JsonPair kv : channels.as<JsonObject>()) {
            channelMapping[kv.key().c_str()] = kv.value().as<int>();
        }
    } else {
        Serial.println("Kanalzuordnung nicht gefunden oder ungültig!");
    }

    // Standardwerte für alle Kanäle initialisieren
    for (const auto& pair : channelMapping) {
        channelValues[pair.first] = 1024;
    }
}

void InputHandler::update() {
    std::string rawData;
    if (wifiInput) {
        wifiInput->update();
        rawData = wifiInput->getInput();
    } else if (sbusInput) {
        sbusInput->update();
        rawData = sbusInput->getInput();
    }
    if (!rawData.empty()) {
        Serial.print("Empfangene Rohdaten: ");
        Serial.println(rawData.c_str());
        // Rohdaten parsen
        std::vector<int> parsedValues;
        std::istringstream ss(rawData);
        std::string token;
        while (std::getline(ss, token, ',')) {
            parsedValues.push_back(std::stoi(token));
        }
        // Serial.println("Geparste Werte:");
        for (size_t i = 0; i < parsedValues.size(); ++i) {
            // Serial.printf("Wert %d: %d\n", i + 1, parsedValues[i]);
        }
        for (const auto& pair : channelMapping) {
            int index = pair.second - 1;  // JSON-Index ist 1-basiert
            if (index >= 0 && index < parsedValues.size()) {
                channelValues[pair.first] = parsedValues[index];
                // Serial.printf("Kanal: %s, Index: %d, Wert: %d\n", pair.first.c_str(), index + 1, channelValues[pair.first]);
            } else {
                // Serial.printf("Kanal: %s, Index %d außerhalb des Bereichs!\n", pair.first.c_str(), index + 1);
            }
        }
    }
}


std::string InputHandler::getInput() {
    if (wifiInput) return wifiInput->getInput();
    if (sbusInput) return sbusInput->getInput();
    return "";
}

int InputHandler::getChannelValue(const std::string& channel) {
    if (channelValues.find(channel) != channelValues.end()) {
        return channelValues[channel];
    }
    return 1024;  
}
