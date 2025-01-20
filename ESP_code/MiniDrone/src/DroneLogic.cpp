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
    motorPins[0] = db->get<int>("mode/drone/Motor01pin", 5);
    motorPins[1] = db->get<int>("mode/drone/Motor02pin", 18);
    motorPins[2] = db->get<int>("mode/drone/Motor03pin", 19);
    motorPins[3] = db->get<int>("mode/drone/Motor04pin", 21);

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
    
    int rawThrottle = input->getChannelValue("ChannelThrottle");
    int rawYaw = input->getChannelValue("ChannelYaw");
    int rawPitch = input->getChannelValue("ChannelPitch");
    int rawRoll = input->getChannelValue("ChannelRoll");

    // Debugging der Rohwerte
    Serial.printf("Raw Inputs -> Throttle: %d, Yaw: %d, Pitch: %d, Roll: %d\n", 
                  rawThrottle, rawYaw, rawPitch, rawRoll);    


    
    // SBUS-Eingaben skalieren


    int throttleInput = map(input->getChannelValue("ChannelThrottle"), 1000, 2000, 0, 100);
    int yawInput = map(input->getChannelValue("ChannelYaw"), 1000, 2000, 0, 100);
    int pitchInput = map(input->getChannelValue("ChannelPitch"), 1000, 2000, 0, 100);
    int rollInput = map(input->getChannelValue("ChannelRoll"), 1000, 2000, 0, 100);

    // Motorsteuerung nur basierend auf Stick-Inputs
    motorSpeeds[0] = throttleInput + pitchInput - rollInput - yawInput; // Front-Left
    motorSpeeds[1] = throttleInput + pitchInput + rollInput + yawInput; // Front-Right
    motorSpeeds[2] = throttleInput - pitchInput + rollInput - yawInput; // Back-Left
    motorSpeeds[3] = throttleInput - pitchInput - rollInput + yawInput; // Back-Right

    // Begrenzung der Motorwerte auf 0–100%
    for (int i = 0; i < 4; i++) {
        motorSpeeds[i] = constrain(motorSpeeds[i], 0, 100);
    }

    // Debugging-Infos ausgeben
    Serial.printf("Inputs -> Throttle: %d, Pitch: %d, Roll: %d, Yaw: %d\n", throttleInput, pitchInput, rollInput, yawInput);
    for (int i = 0; i < 4; i++) {
        int pwmValue = map(motorSpeeds[i], 0, 100, 1000, 2000);
        ledcWrite(i, pwmValue);
        Serial.printf("Motor %d -> Speed: %d%%, PWM: %d\n", i + 1, motorSpeeds[i], pwmValue);
    }
    Serial.println("---------------------------");
}



void DroneLogic::setMotorSpeeds() {
    for (int i = 0; i < 4; i++) {
        int pwmValue = map(motorSpeeds[i], 0, 100, 1000, 2000);  // 0% = 1000 µs, 100% = 2000 µs
        ledcWrite(i, pwmValue);  // PWM-Wert auf den Motor-Kanal schreiben
        Serial.printf("Motor %d: Geschwindigkeit %d%%, PWM %d\n", i + 1, motorSpeeds[i], pwmValue);
    }
}
