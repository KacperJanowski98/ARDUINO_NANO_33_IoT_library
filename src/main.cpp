/**
 * @file main.cpp
 * @author Kacper Janowski
 * @brief 
 * @version 0.1
 * @date 2021-05-04
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "Lsm6ds3IoT.h"

AccelOutput_t Output;

SensorSettings_t AccelSet;

LSM6DS3 Sensor;

void setup() {
  Serial.begin(9600); // Inicjalizacja komunikacji szeregowej 
  Wire.begin(); // Inicjalizacja biblioteki Wire.h odpowiedzialnej za komunikację po I2C

  Sensor.ACC_Mode_Init(&AccelSet, NORMAL_MODE_208, ACC_2G, ACC_FILTER_400);

  Sensor.Accelerometer_Init(AccelSet.accelInitialVal);

  Sensor.Accelerometer_register_write(LSM6DS3_I2C, LSM6DS3_CTRL3_C, 0x04);

  Sensor.Accelerometer_register_write(LSM6DS3_I2C, LSM6DS3_CTRL5_C, 0x20);

  Sensor.Accelerometer_High_perf_Disable(Sensor);

  Sensor.Accelerometer_XYZ_Output_open();
}

void loop() {
  Sensor.Accelerometer_XYZ_read_value(&Output, &AccelSet);

  Serial.print("Xa= ");
  Serial.print(Output.Xa);
  Serial.print("      Ya= ");
  Serial.print(Output.Ya);
  Serial.print("      Za= ");
  Serial.println(Output.Za);
  
  delay(1000);
}