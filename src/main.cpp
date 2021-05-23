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

void setup() {
  Serial.begin(9600); // Inicjalizacja komunikacji szeregowej 
  Wire.begin(); // Inicjalizacja biblioteki Wire.h odpowiedzialnej za komunikację po I2C

  AccelSet.accelRange = 2;  // Przyspieszenie ziemskie 2g, takie zostanie ustwione w metodzie Accelerometer_Init()

  Accelerometer_Init(0x50);

  // Set LSM6DS3 in measuring mode
  // Wire.beginTransmission(LSM6DS3_I2C);  // Start communicating with the device
  // Wire.write(0x10); // CTRL1_XL register (53.s)
  // Wire.write(0x50); // Normal mode 208Hz, full-scale mode 2g 
  // Wire.endTransmission();

  Accelerometer_register_write(LSM6DS3_I2C, LSM6DS3_CTRL3_C, 0x04);

  // Wire.beginTransmission(LSM6DS3_I2C);  // Start communicating with the device
  // Wire.write(0x12); // CTRL3_C register (61.s)
  // Wire.write(0x04); // Register address automatically incremented during a multiple byte access with a serial interface
  // Wire.endTransmission();

  Accelerometer_register_write(LSM6DS3_I2C, LSM6DS3_CTRL5_C, 0x20);

  // Wire.beginTransmission(LSM6DS3_I2C);  // Start communicating with the device
  // Wire.write(0x14); // CTRL5_c register (57.s)
  // Wire.write(0x20); // Accelerometer only rounding pattern
  // Wire.endTransmission();

  Accelerometer_register_write(LSM6DS3_I2C, LSM6DS3_CTRL5_C, 0x10);

  // Wire.beginTransmission(LSM6DS3_I2C);  // Start communicating with the device
  // Wire.write(LSM6DS3_CTRL6_C); // CTRL6_c register (57.s)
  // Wire.write(0x10); // high-performance operating mode disable
  // Wire.endTransmission();

  Accelerometer_XYZ_Output_open();

  // Wire.beginTransmission(LSM6DS3_I2C);  // Start communicating with the device
  // Wire.write(0x18); // CTRL9_XL register (61.s)
  // Wire.write(0x38); // X, Y, Z -axis output enable
  // Wire.endTransmission();
}

void loop() {
  // Read accelerometer data 
  // Wire.beginTransmission(LSM6DS3_I2C);  // Start communicating with the device
  // Wire.write(0x28); // Linear acceleration sensor X-axis output register, start (66.s)
  // Wire.endTransmission(false);
  // Wire.requestFrom(LSM6DS3_I2C, 6, true); // Read 6 registers total, each axis value is stored in 2 registers

  // X = (Wire.read() | Wire.read() << 8); // X-axis value
  // X_out = (float)X * 0.000061; // * 0,061 / 1000; // For a range of +-2g, 4g 0.122, 8g 0.244, 16g 0.488 
  // Y = (Wire.read() | Wire.read() << 8); // Y-axis value
  // Y_out = (float)Y * 0.000061; // * 0,061 / 1000; // For a range of +-2g, 
  // Z = (Wire.read() | Wire.read() << 8); // Z-axis value
  // Z_out = (float)Z * 0.000061; // * 0,061 / 1000; // For a range of +-2g, 

  // Serial.print("Xa= ");
  // Serial.print(X_out);
  // Serial.print("     Ya= ");
  // Serial.print(Y_out);
  // Serial.print("     Za= ");
  // Serial.println(Z_out);

  Accelerometer_XYZ_read_value(&Output, &AccelSet);

  Serial.print("Xa= ");
  Serial.print(Output.Xa);
  Serial.print("     Ya= ");
  Serial.print(Output.Ya);
  Serial.print("     Za= ");
  Serial.println(Output.Za);
  
  delay(1000);
}