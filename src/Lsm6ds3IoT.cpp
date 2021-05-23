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

/**
 * @brief Metoda inicjalizująca akcelerometr LSM6DS3 
 * 
 * @param Value Wartość szesnastkowa odpowiedzialna za konfiguracja trybu pracy akcelerometru (dokumentacja str. 54)
 */
void Accelerometer_Init( uint16_t Value){
  Wire.beginTransmission(LSM6DS3_I2C);  // Adres urządzenia 0x6A
  Wire.write(LSM6DS3_CTRL1_XL); // Rejest kontrolny CTRL1_XL register (53.s)
  Wire.write(Value); // bity [7 6 5 4] Wyjściowa prędkość transmisji danych i wybór trybu zasilania, bity [3 2] przyśpieszenie, bity [1 0] filtr 
  Wire.endTransmission();
}

/**
 * @brief Metoda konfigurująca rejestr odpiewiedzialny za aktywowanie odczytywania rejestrów, w których znajdują sie wyniki pomiaru
 * 
 */
void Accelerometer_XYZ_Output_open(){
  Wire.beginTransmission(LSM6DS3_I2C);  // Adres urządzenia 0x6A
  Wire.write(LSM6DS3_CTRL9_XL); // Rejest kontrolny CTRL9_XL register (61.s)
  Wire.write(0x38); // osie X, Y, Z wyjściowe dane odblokowane 
  Wire.endTransmission();
}

/**
 * @brief Metoda odpowiedzialna za zapis Value do rejestru RegisterAddr w urządzeniu o adresie DeviceAddr
 * 
 * @param DeviceAddr Adres urządzenia
 * @param RegisterAddr Adres rejestru który ma zostać skonfigurowany
 * @param Value Wartość odpowiedzialna za konfiguracje rejestru
 */
void Accelerometer_register_write(uint16_t DeviceAddr, uint16_t RegisterAddr, uint16_t Value){
  Wire.beginTransmission(DeviceAddr);  
  Wire.write(RegisterAddr); 
  Wire.write(Value); 
  Wire.endTransmission();
}

/**
 * @brief Metoda odczytująca wartość z rejestru
 * 
 * @param DeviceAddr Adres urządzenia
 * @param RegisterAddr Adres rejestru z którego ma zostać odczytana wartość
 * @return uint16_t 
 */
uint16_t Accelerometer_one_register_read(uint16_t DeviceAddr, uint16_t RegisterAddr){
  Wire.beginTransmission(DeviceAddr); 
  Wire.write(RegisterAddr); 
  Wire.endTransmission(true); // Zakończenie transmisji

  return Wire.read();
}

/**
 * @brief Metoda odczytująca wartości z wielu rejestrów jeden po drugim
 * 
 * @param DeviceAddr Adres urządzenia
 * @param RegisterAddr Adres rejestru od które ma zostać rozpoczete odczytywanie
 * @param numOfReg Liczba kolejnych rejestrów do
 */
void Accelerometer_multiple_register_read(uint16_t DeviceAddr, uint16_t RegisterAddr, uint16_t numOfReg){
  Wire.beginTransmission(DeviceAddr); 
  Wire.write(RegisterAddr); 
  Wire.endTransmission(false); // Nie kończ transmisji na jednym rejestrze 
  Wire.requestFrom(LSM6DS3_I2C, numOfReg, true); // Odczytanie danych z numOfReg rejestrów
}

/**
 * @brief Metoda wyznaczające końcowe wartości pomiaru przyśpieszenia we wszystkich osiach X, Y, Z
 * 
 * @param OutData Struktura ze zmiennymi do zapisu wyników pomiaru przyśpieszenia
 * @param range Struktura inicjalizująca parametry sensora, w tej metodzie potrzebny jest tylko zakres przyśpieszenia (accelRange)
 */
void Accelerometer_XYZ_read_value(AccelOutput_t *OutData, SensorSettings_t *range){
  int16_t X = 0, Y = 0, Z = 0;  // zmienne lokalne pomocne w obliczeniu wartości końcowych pomiaru
  double sensitivity; // zmienna lokalna do przetrzymania rozdzielczosci potrzebnej do przeskalowania wyników odczytachy z rejestrów

  switch (range->accelRange)
  {
  case 2:  // +-2g
    sensitivity = (0.061 / 1000);
    break;
  case 4:  // +-4g
    sensitivity = (0.122 / 1000);
    break;
  case 8:  // +-8g
    sensitivity = (0.244 / 1000);
    break;
  case 16:  // +-16g
    sensitivity = (0.488 / 1000);
    break;
  default:
    break;
  }

  Wire.beginTransmission(LSM6DS3_I2C); 
  Wire.write(0x28); // Adres rozpoczynający odczytywanie danych z akcelerometru  (Dokumentacja str. 66)
  Wire.endTransmission(false); // Nie kończ transmisji na jednym rejestrze 
  Wire.requestFrom(LSM6DS3_I2C, 6, true); // Odczytanie danych z 6 rejestrów, po dwa rejestry dla każdej z osi

  X = (Wire.read() | Wire.read() << 8); // oś X
  OutData->Xa = ((float)X * sensitivity); 
  Y = (Wire.read() | Wire.read() << 8); // oś Y
  OutData->Ya = ((float)Y * sensitivity); 
  Z = (Wire.read() | Wire.read() << 8); // oś Z
  OutData->Za = ((float)Z * sensitivity); 
}

