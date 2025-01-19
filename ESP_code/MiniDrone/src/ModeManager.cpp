#include "ModeManager.h"
#include <Arduino.h>
#include "DatabaseTool.h"

ModeManager::ModeManager(DatabaseTool* db) : database(db), droneLogic(nullptr), planeLogic(nullptr), heliLogic(nullptr) {}



void ModeManager::init() {
    cleanupDroneMode();
    cleanupPlaneMode();
    cleanupHeliMode();

    // Lese den Modus aus der JSON-Datenbank
    currentMode = database->get<std::string>("mode/type", "drone");
    Serial.printf("Modus gewählt: %s\n", currentMode.c_str());

    // Modus-spezifische Initialisierung
    if (currentMode == "drone") {
        setupDroneMode();
    } else if (currentMode == "plane") {
        setupPlaneMode();
    } else if (currentMode == "heli") {
        setupHeliMode();
    } else {
        Serial.println("Unbekannter Modus oder nicht implementiert.");
    }
}


void ModeManager::setupDroneMode() {
    Serial.println("Initialisiere Drohnenmodus...");
    droneLogic = new DroneLogic();
    droneLogic->init(database);  // Übergib die JSON-Datenbank
}

void ModeManager::setupPlaneMode() {
    Serial.println("Initialisiere Flugzeugmodus...");
    planeLogic = new PlaneLogic();
    planeLogic->init(database);
}

void ModeManager::setupHeliMode() {
    Serial.println("Initialisiere Helikoptermodus...");
    heliLogic = new HeliLogic();
    heliLogic->init(database);
}


void ModeManager::cleanupDroneMode() {
    if (droneLogic) {
        delete droneLogic;  // Lösche die Drohnenlogik
        droneLogic = nullptr;
    }
}

void ModeManager::cleanupPlaneMode() {
    if (planeLogic) {
        delete planeLogic;
        planeLogic = nullptr;
        Serial.println("Flugzeugmodus bereinigt.");
    }
}

void ModeManager::cleanupHeliMode() {
    if (heliLogic) {
        delete heliLogic;
        heliLogic = nullptr;
        Serial.println("Helikoptermodus bereinigt.");
    }
}


void ModeManager::update(SensorHandler* sensors, InputHandler* input) {
    if (currentMode == "drone" && droneLogic) {
        droneLogic->update(sensors, input);
    } else if (currentMode == "plane" && planeLogic) {
        planeLogic->update(sensors, input);
    } else if (currentMode == "heli" && heliLogic) {
        heliLogic->update(sensors, input);
    } else {
        Serial.println("Kein gültiger Modus gesetzt oder keine Aktion definiert.");
    }
}
