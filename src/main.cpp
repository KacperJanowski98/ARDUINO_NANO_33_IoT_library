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

//int LSM6DS3 = 0x6A; // The LSM6DS3 sensor I2C address 

float X_out = 0.0f, Y_out = 0.0f, Z_out = 0.0f;  // Outputs
int16_t X = 0, Y = 0, Z = 0;

void setup() {
  Serial.begin(9600); // Initate serial communication for printing the results on the Serial monitor
  Wire.begin(); // Initiate the Wire library

  // Wire.beginTransmission(LSM6DS3w);  // Start communicating with the device
  // Wire.write(0x01); // FUNC_CFG_ACCESS (01h) (47.s)
  // Wire.write(0x80); // Enable access to embedded functions configuration registers
  // Wire.endTransmission();

  Accelerometer_Init();

  // Set LSM6DS3 in measuring mode
  // Wire.beginTransmission(LSM6DS3_I2C);  // Start communicating with the device
  // Wire.write(0x10); // CTRL1_XL register (53.s)
  // Wire.write(0x50); // Normal mode 208Hz, full-scale mode 2g 
  // Wire.endTransmission();

  Wire.beginTransmission(LSM6DS3_I2C);  // Start communicating with the device
  Wire.write(0x12); // CTRL3_C register (61.s)
  Wire.write(0x04); // Register address automatically incremented during a multiple byte access with a serial interface
  Wire.endTransmission();

  Wire.beginTransmission(LSM6DS3_I2C);  // Start communicating with the device
  Wire.write(0x14); // CTRL5_c register (57.s)
  Wire.write(0x20); // Accelerometer only rounding pattern
  Wire.endTransmission();

  Wire.beginTransmission(LSM6DS3_I2C);  // Start communicating with the device
  Wire.write(0x15); // CTRL6_c register (57.s)
  Wire.write(0x10); // high-performance operating mode disable
  Wire.endTransmission();

  Accelerometer_Output_open();

  // Wire.beginTransmission(LSM6DS3_I2C);  // Start communicating with the device
  // Wire.write(0x18); // CTRL9_XL register (61.s)
  // Wire.write(0x38); // X, Y, Z -axis output enable
  // Wire.endTransmission();
}

void loop() {
  // Read accelerometer data 
  Wire.beginTransmission(LSM6DS3_I2C);  // Start communicating with the device
  Wire.write(0x28); // Linear acceleration sensor X-axis output register, start (66.s)
  Wire.endTransmission(false);
  Wire.requestFrom(LSM6DS3_I2C, 6, true); // Read 6 registers total, each axis value is stored in 2 registers

  X = (Wire.read() | Wire.read() << 8); // X-axis value
  X_out = (float)X * 0.000061; // * 0,061 / 1000; // For a range of +-2g, 4g 0.122, 8g 0.244, 16g 0.488 
  Y = (Wire.read() | Wire.read() << 8); // Y-axis value
  Y_out = (float)Y * 0.000061; // * 0,061 / 1000; // For a range of +-2g, 
  Z = (Wire.read() | Wire.read() << 8); // Z-axis value
  Z_out = (float)Z * 0.000061; // * 0,061 / 1000; // For a range of +-2g, 

  Serial.print("Xa= ");
  Serial.println(X_out);
  Serial.print("Ya= ");
  Serial.println(Y_out);
  Serial.print("Za= ");
  Serial.println(Z_out);
  Serial.println("");

  delay(1000);
}