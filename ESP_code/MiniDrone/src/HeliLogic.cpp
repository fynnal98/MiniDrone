#include "HeliLogic.h"
#include <Arduino.h>

HeliLogic::HeliLogic(DatabaseTool* db) : database(db), lastUpdateTime(0), iTerm(0.0f), lastRollError(0.0f), lastPitchError(0.0f) {}

void HeliLogic::init() {
    Serial.println("Initialisiere HeliLogic...");

    // Pins aus der Datenbank laden
    motorMainPin = database->get<int>("mode/heli/MotorMain", 5);
    motorAftPin = database->get<int>("mode/heli/MotorAft", 17);
    servoBackPin = database->get<int>("mode/heli/ServoBack", 13); // Servo 2
    servoLeftPin = database->get<int>("mode/heli/ServoLeft", 14); // Servo 6
    servoRightPin = database->get<int>("mode/heli/ServoRight", 15); // Servo 1

    // PID-Parameter laden
    kp = database->get<float>("mode/heli/pid/kp", 1.0f);
    ki = database->get<float>("mode/heli/pid/ki", 0.0f);
    kd = database->get<float>("mode/heli/pid/kd", 0.0f);

    // Servos initialisieren
    servoBack.attach(servoBackPin);
    servoLeft.attach(servoLeftPin);
    servoRight.attach(servoRightPin);

    Serial.println("HeliLogic initialisiert.");
}

void HeliLogic::update(SensorHandler* sensors, InputHandler* input) {
    unsigned long currentTime = millis();
    float dt = (currentTime - lastUpdateTime) / 1000.0f;
    lastUpdateTime = currentTime;

    // Eingabekanäle
    unsigned long rollInput = map(input->getChannelValue("Channel1"), 172, 1811, -500, 500);  // Roll
    unsigned long pitchInput = map(input->getChannelValue("Channel2"), 172, 1811, -500, 500); // Pitch
    unsigned long thrustInput = map(input->getChannelValue("Channel6"), 172, 1811, 1000, 2000); // Thrust

    // Sensordaten für Roll und Pitch-Korrekturen
    float roll, pitch, yaw;
    sensors->getFilteredData(roll, pitch, yaw);

    // PID-Korrekturen berechnen
    float rollCorrection = calculatePID(roll, lastRollError, dt);
    float pitchCorrection = calculatePID(pitch, lastPitchError, dt);

    // Swashplate-Positionen berechnen
    unsigned long servoBackPulse, servoLeftPulse, servoRightPulse;
    calculateSwashplatePositions(rollInput, pitchInput, thrustInput, rollCorrection, pitchCorrection,
                                  servoBackPulse, servoLeftPulse, servoRightPulse);

    // Servos ansteuern
    servoBack.writeMicroseconds(servoBackPulse);
    servoLeft.writeMicroseconds(servoLeftPulse);
    servoRight.writeMicroseconds(servoRightPulse);

    // Debugging
    Serial.printf("Inputs -> Roll: %lu, Pitch: %lu, Thrust: %lu\n", rollInput, pitchInput, thrustInput);
    Serial.printf("Swashplate -> Back: %lu, Left: %lu, Right: %lu\n", servoBackPulse, servoLeftPulse, servoRightPulse);
}

float HeliLogic::calculatePID(float error, float& lastError, float dt) {
    float pTerm = kp * error;
    iTerm += ki * error * dt;
    float dTerm = kd * (error - lastError) / dt;
    lastError = error;
    return pTerm + iTerm + dTerm;
}

void HeliLogic::calculateSwashplatePositions(unsigned long rollInput, unsigned long pitchInput, unsigned long thrustInput,
                                              float rollCorrection, float pitchCorrection,
                                              unsigned long& servoBackPulse, unsigned long& servoLeftPulse, unsigned long& servoRightPulse) {
    // Basisbewegungen durch Inputs berechnen
    applyInputs(rollInput, pitchInput, thrustInput, servoBackPulse, servoLeftPulse, servoRightPulse);

    // Debugging: Nach Inputs
    Serial.printf("Nach Inputs -> Back: %lu, Left: %lu, Right: %lu\n", servoBackPulse, servoLeftPulse, servoRightPulse);

    // Korrekturen anwenden
    applyCorrections(rollCorrection, pitchCorrection, servoBackPulse, servoLeftPulse, servoRightPulse);

    // Debugging: Nach Korrekturen
    Serial.printf("Nach Korrekturen -> Back: %lu, Left: %lu, Right: %lu\n", servoBackPulse, servoLeftPulse, servoRightPulse);

    // Begrenzung der Servoimpulse auf 1000–2000 µs
    servoBackPulse = constrain(servoBackPulse, 1000, 2000);
    servoLeftPulse = constrain(servoLeftPulse, 1000, 2000);
    servoRightPulse = constrain(servoRightPulse, 1000, 2000);
}

void HeliLogic::applyInputs(unsigned long rollInput, unsigned long pitchInput, unsigned long thrustInput,
                             unsigned long& servoBackPulse, unsigned long& servoLeftPulse, unsigned long& servoRightPulse) {
    servoBackPulse = thrustInput;
    servoLeftPulse = thrustInput;
    servoRightPulse = 2000 - (thrustInput - 1000);

    servoLeftPulse -= rollInput;
    servoRightPulse -= rollInput;

    servoBackPulse += pitchInput;
    servoLeftPulse -= pitchInput;
    servoRightPulse += pitchInput;
}

void HeliLogic::applyCorrections(float rollCorrection, float pitchCorrection,
                                  unsigned long& servoBackPulse, unsigned long& servoLeftPulse, unsigned long& servoRightPulse) {
    servoLeftPulse -= rollCorrection;
    servoRightPulse -= rollCorrection;  

    servoBackPulse -= pitchCorrection;
    servoLeftPulse += pitchCorrection;
    servoRightPulse -= pitchCorrection; 
}
