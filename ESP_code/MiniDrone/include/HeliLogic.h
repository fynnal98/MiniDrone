#ifndef HELILOGIC_H
#define HELILOGIC_H

#include <ESP32Servo.h>
#include "DatabaseTool.h"
#include "SensorHandler.h"
#include "InputHandler.h"

class HeliLogic {
public:
    HeliLogic(DatabaseTool* db);

    void init();
    void update(SensorHandler* sensors, InputHandler* input);

private:
    // Berechnung der Swashplate-Positionen
    void calculateSwashplatePositions(unsigned long rollInput, unsigned long pitchInput, unsigned long thrustInput,
                                       float rollCorrection, float pitchCorrection,
                                       unsigned long& servoBackPulse, unsigned long& servoLeftPulse, unsigned long& servoRightPulse);

    // Verarbeitet nur die Eingaben
    void applyInputs(unsigned long rollInput, unsigned long pitchInput, unsigned long thrustInput,
                     unsigned long& servoBackPulse, unsigned long& servoLeftPulse, unsigned long& servoRightPulse);

    // Verarbeitet nur die Korrekturen
    void applyCorrections(float rollCorrection, float pitchCorrection,
                          unsigned long& servoBackPulse, unsigned long& servoLeftPulse, unsigned long& servoRightPulse);

    // Steuerung des Hauptmotors
    void controlMainMotor(unsigned long channel8Pulse);

    // PID-Berechnung
    float calculatePID(float error, float& lastError, float dt);

    // PID-Parameter und Variablen
    float kp, ki, kd;
    float iTerm, lastRollError, lastPitchError;

    // Zeitvariable für PID
    unsigned long lastUpdateTime;

    // Servos
    Servo servoBack, servoLeft, servoRight;
    Servo mainMotor;  // Servo für den Hauptmotor

    // Servo- und Motor-Pins
    int motorMainPin, motorAftPin, servoBackPin, servoLeftPin, servoRightPin;

    DatabaseTool* database;
};

#endif
