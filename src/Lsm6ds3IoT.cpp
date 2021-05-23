/**
 * @file Lsm6ds3IoT.cpp
 * @author Kacper Janowski
 * @brief 
 * @version 0.1
 * @date 2021-05-04
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "Lsm6ds3IoT.h"

void Accelerometer_Init(){
  Wire.beginTransmission(LSM6DS3_I2C);  // Start communicating with the device
  Wire.write(0x10); // CTRL1_XL register (53.s)
  Wire.write(0x50); // Normal mode 208Hz, full-scale mode 2g 
  Wire.endTransmission();
}

void Accelerometer_Output_open(){
  Wire.beginTransmission(LSM6DS3_I2C);  // Start communicating with the device
  Wire.write(0x18); // CTRL9_XL register (61.s)
  Wire.write(0x38); // X, Y, Z -axis output enable
  Wire.endTransmission();
}