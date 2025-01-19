#ifndef MODEMANAGER_H
#define MODEMANAGER_H

#include <string>
#include "DatabaseTool.h"
#include "DroneLogic.h"
#include "PlaneLogic.h"
#include "HeliLogic.h"

class ModeManager {
public:
    ModeManager(DatabaseTool* db);  // Konstruktor mit DatabaseTool
    void init();                   // Initialisiere den Modus
    void update(SensorHandler* sensors, InputHandler* input);

private:
    DatabaseTool* database;  // Zeiger auf die JSON-Datenbank
    std::string currentMode;

    DroneLogic* droneLogic;
    PlaneLogic* planeLogic;
    HeliLogic* heliLogic;

    void setupDroneMode();
    void setupPlaneMode();
    void setupHeliMode();

    void cleanupDroneMode();  // Bereinigt den Drohnenmodus
    void cleanupPlaneMode();  // Bereinigt den Flugzeugmodus
    void cleanupHeliMode();   // Bereinigt den Helikoptermodus
};

#endif
