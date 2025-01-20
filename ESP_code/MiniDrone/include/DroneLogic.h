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
    float kp = 0.0f;   
    float ki = 0.0f;   
    float kd = 0.0f;    
    float integralRoll, integralPitch, integralYaw; 
    float previousRollError, previousPitchError, previousYawError;

    float pitchErrorSum = 0.0f;
    float rollErrorSum = 0.0f;
    float yawErrorSum = 0.0f;

    float lastPitchError = 0.0f;
    float lastRollError = 0.0f;
    float lastYawError = 0.0f;


    void setMotorSpeeds();  // Sendet PWM-Signale an die Motoren
    float computePID(float target, float current, float& integral, float& previousError);

};

#endif
