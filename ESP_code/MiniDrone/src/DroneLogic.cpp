#include "DroneLogic.h"
#include <Arduino.h>

DroneLogic::DroneLogic() {
    for (int i = 0; i < 4; i++) {
        motorPins[i] = -1;    // Initialisiere Pins mit ungültigem Wert
        motorSpeeds[i] = 0;   // Initialisiere Motoren auf 0% Geschwindigkeit
    }
}

void DroneLogic::init(DatabaseTool* db) {
    Serial.println("Initialisiere DroneLogic...");

    // Lese Motorpins aus der JSON-Datei
    motorPins[0] = db->get<int>("mode/drone/Motor01pin", 5);
    motorPins[1] = db->get<int>("mode/drone/Motor02pin", 18);
    motorPins[2] = db->get<int>("mode/drone/Motor03pin", 19);
    motorPins[3] = db->get<int>("mode/drone/Motor04pin", 21);

    pwmFrequency = db->get<int>("mode/drone/pwmFrequency", 50);
    pwmResolution = db->get<int>("mode/drone/pwmResolution", 10);

    // Initialisiere PWM für alle Motoren
    for (int i = 0; i < 4; i++) {
        if (motorPins[i] != -1) {
            ledcSetup(i, pwmFrequency, pwmResolution); // PWM-Kanal, Frequenz, Auflösung
            ledcAttachPin(motorPins[i], i); // Motorpin mit PWM-Kanal verknüpfen
            Serial.printf("Motor %d auf Pin %d initialisiert (Frequenz: %d Hz, Auflösung: %d Bit).\n", i + 1, motorPins[i], pwmFrequency, pwmResolution);        
            }
    }
}

void DroneLogic::update(SensorHandler* sensors, InputHandler* input) {
    // Beispiel: Dummy-Logik für die Motorsteuerung
    for (int i = 0; i < 4; i++) {
        motorSpeeds[i] = 50;  // Setze alle Motoren auf 50% Geschwindigkeit
    }

    setMotorSpeeds();
}

void DroneLogic::setMotorSpeeds() {
    for (int i = 0; i < 4; i++) {
        int pwmValue = map(motorSpeeds[i], 0, 100, 1000, 2000); // Konvertiere Geschwindigkeit in PWM-Signal
        ledcWrite(i, pwmValue);  // Schreibe PWM-Wert auf Kanal i
        // Serial.printf("Motor %d: Geschwindigkeit %d%%, PWM %d\n", i + 1, motorSpeeds[i], pwmValue);
    }
}
