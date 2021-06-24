/**
 ***********************************************************************************************************************
 * @file main.cpp
 * @author Kacper Janowski
 * @brief Plik z testowym uruchomieniem biblioteki
 * @version 0.1
 * @date 2021-05-04
 ***********************************************************************************************************************
 */
#include "Lsm6ds3IoT.h"

AccelOutput_t Output;         /**< Struktura przechowująca wartości wyjściowe akcelerometru */

GyroOutput_t Gyro_Output;     /**< Struktura przechowująca wartości wyjściowe żyroskopu */

TempOutput_t Temp_Output;     /**< Struktura przechowująca wartości wyjściowe termometru */

SensorSettings_t LSM6DS3Set;  /**< Struktura przechowująca ustawienia używane do obliczeń końcowych wyników pomiarów */

LSM6DS3 Sensor;               /**< Obiekt klasy LSM6DS3 która dzieczy po klasie LSM6DS3Core */

void setup() {
  Serial.begin(9600); /**< Inicjalizacja komunikacji szeregowej */
  Wire.begin(); /**< Inicjalizacja biblioteki Wire.h odpowiedzialnej za komunikację po I2C */

  /** Inicjalizacja parametrów akcelerometru
   */
  Sensor.ACC_Mode_Init(&LSM6DS3Set, NORMAL_MODE_208, ACC_2G, ACC_FILTER_400);

  /** Inicjalizacja akcelerometru 
   */
  Sensor.Accelerometer_Init(LSM6DS3Set.accelInitialVal);

  /** Inicjalizacja parametrów żyroskopu
   */
  Sensor.Gyro_Mode_Init(&LSM6DS3Set, NORMAL_MODE_208_G, G_500, 0);

   /** Inicjalizacja żyroskopu
    */
  Sensor.Gyroscope_Init(LSM6DS3Set.gyroInitialVal);

  /** Dokumentacja str. 56 - Ustawienie automatycznej inkremetacji adresów rejestrów w przypadku odczytu wielu rejestrów
   */
  Sensor.LSM6DS3_register_write(LSM6DS3_I2C, LSM6DS3_CTRL3_C, ACC_AUTO_INC_ADDR);

  /** Dokumentacja str. 57 - Wybór pasama akcelerometru
   */
  Sensor.LSM6DS3_register_write(LSM6DS3_I2C, LSM6DS3_CTRL4_C, ACC_BANDWIDTH_BW_XL);

  /** Dokumentacja str. 57/58 - Ustawienie zaokrąglania wartości dla akcelerometru oraz żyroskopu, ustawinie self-test w trybie normal mode
   */
  Sensor.LSM6DS3_register_write(LSM6DS3_I2C, LSM6DS3_CTRL5_C, Sensor.Config_register_CTRL5(GYRO_ACC_ROUND, NORMAL_MODE_ST_ANG, NORAML_MODE_ST_ACC));

  /** Wyłączenie tryb high performance dla akcelerometru
   */
  Sensor.Accelerometer_High_perf_Disable(Sensor);

  /** Aktywacja rejestrów odpowiedzialnych za odczyt wyniku pomiarów akcelerometru
   */
  Sensor.Accelerometer_XYZ_Output_open();

  /** Aktywacja rejestrów odpowiedzialnych za odczyt wyniku pomiarów żyroskopu
   */
  Sensor.Gyroscope_XYZ_Output_open();
}

void loop() {
  /** Odczyt i wyświetlenie wartości pomiarów
   */
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

/********************************** (C) COPYRIGHT Kacper Janowski 2021 *********** END OF FILE ******/