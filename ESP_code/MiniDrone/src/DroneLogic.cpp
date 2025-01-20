#include "DroneLogic.h"
#include <Arduino.h>

DroneLogic::DroneLogic() {
    for (int i = 0; i < 4; i++) {
        motorPins[i] = -1;
        motorSpeeds[i] = 0;
    }
}

void DroneLogic::init(DatabaseTool* db) {
    Serial.println("Initialisiere DroneLogic...");

    // Lese Motorpins aus der JSON-Datei
    motorPins[0] = db->get<int>("mode/drone/Motor01pin", 13);
    motorPins[1] = db->get<int>("mode/drone/Motor02pin", 12);
    motorPins[2] = db->get<int>("mode/drone/Motor03pin", 14);
    motorPins[3] = db->get<int>("mode/drone/Motor04pin", 27);

    // PID-Parameter laden
    kp = db->get<float>("mode/drone/pid/kp", 1.0f);
    ki = db->get<float>("mode/drone/pid/ki", 0.0f);
    kd = db->get<float>("mode/drone/pid/kd", 0.0f);

    // Motoren initialisieren
    for (int i = 0; i < 4; i++) {
        if (motorPins[i] != -1) {
            motors[i].attach(motorPins[i]);
            Serial.printf("Motor %d: Pin %d erfolgreich verbunden.\n", i + 1, motorPins[i]);
        }
    }
}

void DroneLogic::update(SensorHandler* sensors, InputHandler* input) {
    // Rohdaten abrufen
    int rawThrottle = input->getChannelValue("ChannelThrottle");
    int rawYaw = input->getChannelValue("ChannelYaw");
    int rawPitch = input->getChannelValue("ChannelPitch");
    int rawRoll = input->getChannelValue("ChannelRoll");

    // Sensordaten abrufen
    float rollAngle, pitchAngle, yawAngle;
    sensors->getFilteredData(rollAngle, pitchAngle, yawAngle);

    // Berechnung der Fehler
    float rollError = map(rawRoll, 1000, 2000, -30, 30) - rollAngle;   // Roll-Fehler
    float pitchError = map(rawPitch, 1000, 2000, -30, 30) - pitchAngle; // Pitch-Fehler

    // PID-Korrekturen berechnen
    float rollCorrection = kp * rollError + ki * rollErrorSum + kd * (rollError - lastRollError);
    float pitchCorrection = kp * pitchError + ki * pitchErrorSum + kd * (pitchError - lastPitchError);

    // Fehler summieren (I-Anteil) und speichern
    rollErrorSum += rollError;
    pitchErrorSum += pitchError;
    lastRollError = rollError;
    lastPitchError = pitchError;

    // Motorberechnung
    motorSpeeds[0] = rawThrottle + pitchCorrection - rollCorrection; // Front-Left
    motorSpeeds[1] = rawThrottle + pitchCorrection + rollCorrection; // Front-Right
    motorSpeeds[2] = rawThrottle - pitchCorrection - rollCorrection; // Back-Left
    motorSpeeds[3] = rawThrottle - pitchCorrection + rollCorrection; // Back-Right

    // Begrenzung der Motorwerte auf 1000–2000 µs
    for (int i = 0; i < 4; i++) {
        motorSpeeds[i] = constrain(motorSpeeds[i], 1000, 2000);
        motors[i].writeMicroseconds(motorSpeeds[i]);
    }

    // Debugging
    for (int i = 0; i < 4; i++) {
        Serial.printf("Motor %d Speed: %d\n", i + 1, motorSpeeds[i]);
    }

    Serial.println("---------------------------");
}
