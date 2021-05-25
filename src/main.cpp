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

GyroOutput_t Gyro_Output;

TempOutput_t Temp_Output;

SensorSettings_t LSM6DS3Set;

LSM6DS3 Sensor;

void setup() {
  Serial.begin(9600); // Inicjalizacja komunikacji szeregowej 
  Wire.begin(); // Inicjalizacja biblioteki Wire.h odpowiedzialnej za komunikację po I2C

  Sensor.ACC_Mode_Init(&LSM6DS3Set, NORMAL_MODE_208, ACC_2G, ACC_FILTER_400);

  Sensor.Accelerometer_Init(LSM6DS3Set.accelInitialVal);

  Sensor.Gyro_Mode_Init(&LSM6DS3Set, NORMAL_MODE_208_G, G_500, 0);

  Sensor.Gyroscope_Init(LSM6DS3Set.gyroInitialVal);

  Sensor.Accelerometer_register_write(LSM6DS3_I2C, LSM6DS3_CTRL3_C, ACC_AUTO_INC_ADDR);

  // Dokumentacja str. 57
  Sensor.Accelerometer_register_write(LSM6DS3_I2C, LSM6DS3_CTRL4_C, ACC_BANDWIDTH_BW_XL);

  Sensor.Accelerometer_register_write(LSM6DS3_I2C, LSM6DS3_CTRL5_C, Sensor.Config_register_CTRL5(GYRO_ACC_ROUND, NORMAL_MODE_ST_ANG, NORAML_MODE_ST_ACC));

  Sensor.Accelerometer_High_perf_Disable(Sensor);

  Sensor.Accelerometer_XYZ_Output_open();

  Sensor.Gyroscope_XYZ_Output_open();
}

void loop() {
  Sensor.Accelerometer_XYZ_read_value(&Output, &LSM6DS3Set);
  
  Sensor.Gyroscope_XYZ_read_value(&Gyro_Output, &LSM6DS3Set);

  Sensor.Temperature_read_value(&Temp_Output);

  Serial.print("Accel[g]: ");
  Serial.print("       Xa= ");
  Serial.print(Output.Xa);
  Serial.print("       Ya= ");
  Serial.print(Output.Ya);
  Serial.print("       Za= ");
  Serial.println(Output.Za);

  Serial.println("");

  Serial.print("Gyro[dps]: ");
  Serial.print("       Xa= ");
  Serial.print(Gyro_Output.Xa);
  Serial.print("       Ya= ");
  Serial.print(Gyro_Output.Ya);
  Serial.print("       Za= ");
  Serial.println(Gyro_Output.Za);

  Serial.println("");

  Serial.print("Temp[ C]: ");
  Serial.print("       Ta= ");
  Serial.println(Temp_Output.Ta);  

  Serial.println("");
  
  delay(1000);
}