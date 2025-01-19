#ifndef SENSORHANDLER_H
#define SENSORHANDLER_H

#include "MPU6050.h"
#include "DatabaseTool.h"

class SensorHandler {
public:
    SensorHandler();
    void begin(DatabaseTool* db);  
    void update();
    void printData();
    bool isAllConnected();

private:
    MPU6050 mpu6050; 
};

#endif 
