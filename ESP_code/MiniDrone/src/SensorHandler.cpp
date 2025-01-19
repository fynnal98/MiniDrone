#include "SensorHandler.h"

SensorHandler::SensorHandler() : mpu6050() {}

void SensorHandler::begin(DatabaseTool* db) {
    Serial.println("Initialisiere Sensoren...");
    
    // MPU6050 initialisieren mit Datenbank
    mpu6050.begin(db);

    if (mpu6050.isConnected()) {
        Serial.println("MPU6050 erfolgreich verbunden.");
    } else {
        Serial.println("Fehler: MPU6050 nicht verbunden!");
    }
}

void SensorHandler::update() {
    mpu6050.checkConnectionAndBlink();
}

void SensorHandler::printData() {
    sensors_event_t accel, gyro, temp;
    mpu6050.getEvent(&accel, &gyro, &temp);

    // Serial.println("MPU6050 Daten:");
    // Serial.print("  Accel X: "); Serial.print(accel.acceleration.x); Serial.println(" m/s^2");
    // Serial.print("  Accel Y: "); Serial.print(accel.acceleration.y); Serial.println(" m/s^2");
    // Serial.print("  Accel Z: "); Serial.print(accel.acceleration.z); Serial.println(" m/s^2");
    // Serial.print("  Gyro X: "); Serial.print(gyro.gyro.x); Serial.println(" rad/s");
    // Serial.print("  Gyro Y: "); Serial.print(gyro.gyro.y); Serial.println(" rad/s");
    // Serial.print("  Gyro Z: "); Serial.print(gyro.gyro.z); Serial.println(" rad/s");
    // Serial.print("  Temperatur: "); Serial.print(temp.temperature); Serial.println(" °C");
}

bool SensorHandler::isAllConnected() {
    return mpu6050.isConnected();
}



