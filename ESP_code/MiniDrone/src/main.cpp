#include <Arduino.h>
#include "ModeManager.h"
#include "SensorHandler.h"
#include "InputHandler.h"
#include "DatabaseTool.h"

DatabaseTool database;
SensorHandler sensorHandler;
InputHandler* inputHandler;
ModeManager* modeManager;

void setup() {
    Serial.begin(115200);

    database.init();
    sensorHandler.begin(&database);
    inputHandler = InputHandler::createHandler(&database);
    modeManager = new ModeManager(&database);
    modeManager->init();
}

void loop() {
    inputHandler->update();
    sensorHandler.update();
    modeManager->update(&sensorHandler, inputHandler);
    delay(10);
}
