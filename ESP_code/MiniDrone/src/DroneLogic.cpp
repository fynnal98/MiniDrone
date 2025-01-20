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

    pwmFrequency = db->get<int>("mode/drone/pwmFrequency", 50);
    pwmResolution = db->get<int>("mode/drone/pwmResolution", 10);
    
    kp = db->get<int>("mode/drone/pid/kp", 0.0f);
    ki = db->get<int>("mode/drone/pid/ki", 0.0f);
    kd = db->get<int>("mode/drone/pid/kd", 0.0f);

    for (int i = 0; i < 4; i++) {
        if (motorPins[i] != -1) {
            if (ledcSetup(i, pwmFrequency, pwmResolution)) {
                ledcAttachPin(motorPins[i], i);
                Serial.printf("Motor %d: Pin %d erfolgreich mit Kanal %d verbunden.\n", i + 1, motorPins[i], i);
            } else {
                Serial.printf("Fehler: Motor %d konnte nicht auf Pin %d mit Kanal %d initialisiert werden.\n", i + 1, motorPins[i], i);
            }
        }
    }

}
void DroneLogic::update(SensorHandler* sensors, InputHandler* input) {
    // Rohdaten abrufen
    int rawThrottle = input->getChannelValue("ChannelThrottle");
    int rawYaw = input->getChannelValue("ChannelYaw");
    int rawPitch = input->getChannelValue("ChannelPitch");
    int rawRoll = input->getChannelValue("ChannelRoll");

    // Debugging der Inputs
    Serial.printf("Inputs -> Throttle: %d, Yaw: %d, Pitch: %d, Roll: %d\n", 
                  rawThrottle, rawYaw, rawPitch, rawRoll);

    // Inputs verwenden
    int throttleInput = rawThrottle;
    int yawInput = map(rawYaw, 1000, 2000, -500, 500);    // Skalieren auf -500 bis 500
    int pitchInput = map(rawPitch, 1000, 2000, -500, 500); // Skalieren auf -500 bis 500
    int rollInput = map(rawRoll, 1000, 2000, -500, 500);  // Skalieren auf -500 bis 500

    // Motorberechnung
    motorSpeeds[0] = throttleInput + pitchInput - rollInput - yawInput; // Front-Left
    motorSpeeds[1] = throttleInput + pitchInput - rollInput - yawInput; // Front-Right
    motorSpeeds[2] = throttleInput - pitchInput + rollInput - yawInput; // Back-Left
    motorSpeeds[3] = throttleInput - pitchInput - rollInput + yawInput; // Back-Right

    Serial.print("Motor 2 Speed Before Constraint: ");
    Serial.println(motorSpeeds[1]); // Vor Begrenzung
    Serial.print("Motor 2 Speed After Constraint: ");
    Serial.println(constrain(motorSpeeds[1], 1000, 2000)); // Nach Begrenzung


    // Debugging vor Begrenzung
    for (int i = 0; i < 4; i++) {
        Serial.printf("Motor %d Speed Before Constraint: %.2f\n", i + 1, motorSpeeds[i]);
    }

    // Begrenzung der Motorwerte auf 1000–2000 µs
    for (int i = 0; i < 4; i++) {
        motorSpeeds[i] = constrain(motorSpeeds[i], 1000, 2000);
    }

    // Debugging nach Begrenzung
    for (int i = 0; i < 4; i++) {
        Serial.printf("Motor %d Speed After Constraint: %d\n", i + 1, motorSpeeds[i]);
    }

    // Motor-PWM-Werte anwenden
    for (int i = 0; i < 4; i++) {
        ledcWrite(i, motorSpeeds[i]);
        Serial.printf("PWM Output -> Motor %d: %d\n", i + 1, motorSpeeds[i]);
    }

    Serial.println("---------------------------");
}


void DroneLogic::setMotorSpeeds() {
    for (int i = 0; i < 4; i++) {
        ledcWrite(i, motorSpeeds[i]); // Direkt den PWM-Wert schreiben
        Serial.printf("Motor %d -> PWM: %d\n", i + 1, motorSpeeds[i]);
    }
}