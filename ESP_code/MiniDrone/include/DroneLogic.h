#ifndef DRONELOGIC_H
#define DRONELOGIC_H

#include "SensorHandler.h"
#include "InputHandler.h"
#include "DatabaseTool.h"

class DroneLogic {
public:
    DroneLogic();
    void init(DatabaseTool* db);  // Initialisiert die Motoren basierend auf JSON
    void update(SensorHandler* sensors, InputHandler* input);

private:
    int motorPins[4];   // Pins der Motoren
    int motorSpeeds[4]; // Geschwindigkeit der Motoren (0–100%)
    int pwmFrequency;       // PWM-Frequenz
    int pwmResolution;      // PWM-Auflösung

    void setMotorSpeeds();  // Sendet PWM-Signale an die Motoren
};

#endif
