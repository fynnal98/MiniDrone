#ifndef DATABASETOOL_H
#define DATABASETOOL_H

#include <ArduinoJson.h>
#include <SPIFFS.h>
#include <string>

class DatabaseTool {
public:
    void init();  // Initialisiert das DatabaseTool und lädt die JSON-Datei

    template <typename T>
    T get(const char* path, T defaultValue);  // Gibt den Wert zu einem JSON-Pfad zurück

    template <typename T>
    void set(const char* path, T value);  // Setzt einen Wert in der JSON-Datenbank

private:
    StaticJsonDocument<2048> jsonDoc;  // Speicher für die JSON-Daten

    JsonVariant resolvePath(const char* path, bool createMissing = false);
    void saveToFile();
};

#endif
