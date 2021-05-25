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
 * @brief Definicja metod i funkcji odpowiedzialnych za akcelerometr
 * 
 */

/**
 * @brief Metoda ustawiająca tryb pracy akcelerometru rejestr CTRL1_XL (dokumentacja str. 54)
 * 
 * @param Settings Struktura inicjalizująca parametry sensora
 * @param ODR_XL Tryb pracy
 * @param FS_XL Skala przyśpieszenia
 * @param BW_XL Pasmo filtru antyaliasingowego
 */
void LSM6DS3Core::ACC_Mode_Init(SensorSettings_t *Settings, uint16_t ODR_XL, uint16_t FS_XL, uint16_t BW_XL){
     Settings->accelInitialVal = ((ODR_XL << 4 )| (FS_XL << 2) | (BW_XL << 0));

     Settings->accelRange = FS_XL;

     Settings->accelBandWidth = BW_XL;
}

/**
 * @brief Metoda inicjalizująca akcelerometr LSM6DS3 
 * 
 * @param Value Wartość szesnastkowa odpowiedzialna za konfiguracja trybu pracy akcelerometru (dokumentacja str. 54)
 */
void LSM6DS3Core::Accelerometer_Init( uint16_t Value){
  Wire.beginTransmission(LSM6DS3_I2C);  // Adres urządzenia 0x6A
  Wire.write(LSM6DS3_CTRL1_XL); // Rejest kontrolny CTRL1_XL register (53.s)
  Wire.write(Value); // bity [7 6 5 4] Wyjściowa prędkość transmisji danych i wybór trybu zasilania, bity [3 2] przyśpieszenie, bity [1 0] filtr 
  Wire.endTransmission();
}

/**
 * @brief Metoda wyłaczajaca tryb high performance akcelerometru (dokumentacja str. 59)
 * 
 * @param lsm6ds3 Obekt klasy LSM6DS3
 */
void LSM6DS3::Accelerometer_High_perf_Disable(LSM6DS3 lsm6ds3) {
  // Ustawienie '1' w G_HM_MODE, wyłączenie trybu high_performance (dokumentacja str. 59)
  lsm6ds3.LSM6DS3_register_write(LSM6DS3_I2C, LSM6DS3_CTRL7_G, (1 << 7));
  // Włączenie trybu oszczędzania energii oraz normalnego (dokumentacja str. 59)
  lsm6ds3.LSM6DS3_register_write(LSM6DS3_I2C, LSM6DS3_CTRL6_C, (1 << 4));
}

/**
 * @brief Metoda włączająca tryb high performance akcelerometru (dokumentacja str. 59)
 * 
 * @param lsm6ds3 Obekt klasy LSM6DS3
 */
void LSM6DS3::Accelerometer_High_perf_Enable(LSM6DS3 lsm6ds3) {
  // Ustawienie '1' w G_HM_MODE, wyłączenie trybu high_performance (dokumentacja str. 59)
  lsm6ds3.LSM6DS3_register_write(LSM6DS3_I2C, LSM6DS3_CTRL7_G, ~(1 << 7));
  // Włączenie trybu oszczędzania energii oraz normalnego (dokumentacja str. 59)
  lsm6ds3.LSM6DS3_register_write(LSM6DS3_I2C, LSM6DS3_CTRL6_C, ~(1 << 4));
}

/**
 * @brief Metoda konfigurująca rejestr odpiewiedzialny za aktywowanie odczytywania rejestrów, w których znajdują sie wyniki pomiaru
 * 
 */
void LSM6DS3::Accelerometer_XYZ_Output_open(){
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
void LSM6DS3Core::LSM6DS3_register_write(uint16_t DeviceAddr, uint16_t RegisterAddr, uint16_t Value){
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
uint16_t LSM6DS3Core::LSM6DS3_one_register_read(uint16_t DeviceAddr, uint16_t RegisterAddr){
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
void LSM6DS3Core::LSM6DS3_multiple_register_read(uint16_t DeviceAddr, uint16_t RegisterAddr, uint16_t numOfReg){
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
void LSM6DS3::Accelerometer_XYZ_read_value(AccelOutput_t *OutData, SensorSettings_t *range){
  int16_t X = 0, Y = 0, Z = 0;  // zmienne lokalne pomocne w obliczeniu wartości końcowych pomiaru
  double sensitivity; // zmienna lokalna do przetrzymania rozdzielczosci potrzebnej do przeskalowania wyników odczytachy z rejestrów

  switch (range->accelRange)
  {
  case 0x0:  // +-2g
    sensitivity = (0.061 / 1000);
    break;
  case 0x2:  // +-4g
    sensitivity = (0.122 / 1000);
    break;
  case 0x3:  // +-8g
    sensitivity = (0.244 / 1000);
    break;
  case 0x1:  // +-16g
    sensitivity = (0.488 / 1000);
    break;
  default:
    break;
  }

  // Sprawdzenie czy w STATUS_REG bit nr.0 jest ustawiony na 1, jezeli tak to znaczy że jest nowa wartosć 
  // do odczytania w wyjsciowych rejestrach akcelerometry.
  while (!(LSM6DS3Core::LSM6DS3_one_register_read(LSM6DS3_I2C, LSM6DS3_STATUS_REG) & (1 << 0))){;}

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

/**
 * @brief Funkcja konfigurujaca rejestr CTRL5_C (zdefiniowane wartości w pliku Lsm6ds3IoT.h)
 *        dokumentacja strona 57.
 * @param ROUNDING bity 7 6 5 
 * @param ST_G bity 3 2
 * @param ST_XL bity 1 0
 * @return uint16_t 
 */
uint16_t LSM6DS3::Config_register_CTRL5(uint16_t ROUNDING, uint16_t ST_G, uint16_t ST_XL) {
  return ((ROUNDING << 5 )| (ST_G << 2) | (ST_XL << 0));
}

/**
 * @brief Definicja metod i funkcji odpowiedzialnych za żyroskop
 * 
 */

/**
 * @brief Metoda ustawiająca tryb pracy żyroskopu rejestr CTRL2_G (dokumentacja str. 55)
 * 
 * @param Settings Struktura inicjalizująca parametry sensora
 * @param ODR_G Tryb pracy żyroskopu
 * @param FS_G Ustawienie skali dps (250, 500, 1000, 2000)
 * @param FS_125 Ustawienie skali dps 125 (wartość 0 - disable)
 */
void LSM6DS3Core::Gyro_Mode_Init(SensorSettings_t *Settings, uint16_t ODR_G, uint16_t FS_G, uint16_t FS_125) {
  Settings->gyroInitialVal = ((ODR_G << 4) | (FS_G << 2) | (FS_125 << 1));

  Settings->gyroRange = FS_G;
}

/**
 * @brief Metoda inicjalizująca żyroskop LSM6DS3 
 * 
 * @param Value Wartość szesnastkowa odpowiedzialna za konfiguracja trybu pracy żyroskopu (dokumentacja str. 55)
 */
void LSM6DS3Core::Gyroscope_Init( uint16_t Value) {
  Wire.beginTransmission(LSM6DS3_I2C);  // Adres urządzenia 0x6A
  Wire.write(LSM6DS3_CTRL2_G); // Rejest kontrolny CTRL2_G register (55.s)
  Wire.write(Value); // bity [7 6 5 4] Wyjściowa prędkość transmisji danych i wybór trybu zasilania, bity [3 2] wartość dps, bity [1] ful scale 
  Wire.endTransmission();
}

/**
 * @brief Metoda konfigurująca rejestr odpiewiedzialny za aktywowanie odczytywania rejestrów, w których znajdują sie wyniki pomiaru
 * 
 */
void LSM6DS3::Gyroscope_XYZ_Output_open(){
  Wire.beginTransmission(LSM6DS3_I2C);  // Adres urządzenia 0x6A
  Wire.write(LSM6DS3_CTRL10_C); // Rejest kontrolny CTRL10_C register (61.s)
  Wire.write(0x38); // osie X, Y, Z wyjściowe dane odblokowane 
  Wire.endTransmission();
}

/**
 * @brief Metoda wyznaczające końcowe wartości pomiaru żyroskopem we wszystkich osiach X, Y, Z
 * 
 * @param OutData Struktura ze zmiennymi do zapisu wyników pomiaru 
 * @param range Struktura inicjalizująca parametry sensora, w tej metodzie potrzebny jest tylko zakres pracy żyroskopu (gyroRange)
 */
void LSM6DS3::Gyroscope_XYZ_read_value(GyroOutput_t *OutData, SensorSettings_t *range) {
  int16_t X = 0, Y = 0, Z = 0;  // zmienne lokalne pomocne w obliczeniu wartości końcowych pomiaru
  double sensitivity; // zmienna lokalna do przetrzymania rozdzielczosci potrzebnej do przeskalowania wyników odczytachy z rejestrów

  switch (range->gyroRange)
  {
  case 0x0: // +-250
    sensitivity = (8.75 / 1000);
    break;
  case 0x1: // +-500
    sensitivity = (17.5 / 1000);
    break;
  case 0x2: // +-1000
    sensitivity = (35 / 1000);
    break;
  case 0x3: // +-2000
    sensitivity = (70 / 1000);
    break;
  default:
    break;
  }

  // Sprawdzenie czy w STATUS_REG bit nr.1 jest ustawiony na 1, jezeli tak to znaczy że jest nowa wartosć 
  // do odczytania w wyjsciowych rejestrach akcelerometry.
  while (!(LSM6DS3Core::LSM6DS3_one_register_read(LSM6DS3_I2C, LSM6DS3_STATUS_REG) & (1 << 1))){;}

  Wire.beginTransmission(LSM6DS3_I2C); 
  Wire.write(0x22); // Adres rozpoczynający odczytywanie danych z żyroskopu  (Dokumentacja str. 66)
  Wire.endTransmission(false); // Nie kończ transmisji na jednym rejestrze 
  Wire.requestFrom(LSM6DS3_I2C, 6, true); // Odczytanie danych z 6 rejestrów, po dwa rejestry dla każdej z osi

  X = (Wire.read() | Wire.read() << 8); // oś X
  OutData->Xa = ((float)X * sensitivity); 
  Y = (Wire.read() | Wire.read() << 8); // oś Y
  OutData->Ya = ((float)Y * sensitivity); 
  Z = (Wire.read() | Wire.read() << 8); // oś Z
  OutData->Za = ((float)Z * sensitivity); 
}

/**
 * @brief Definicja metod i funkcji odpowiedzialnych za termometr
 * 
 */

/**
 * @brief Metoda zwracająca wartosc temeratury w skali stopni celsjusza
 * 
 * @param OutData Struktura do przechowyywania oblicznej temperatury
 */
void LSM6DS3::Temperature_read_value(TempOutput_t *OutData) {
  int16_t T = 0;  // zmienna lokalna pomocnicza w obliczeniu wartości końcowej pomiaru

  // Sprawdzenie czy w STATUS_REG bit nr.2 jest ustawiony na 1, jezeli tak to znaczy że jest nowa wartosć 
  // do odczytania w wyjsciowych rejestrach termometru.
  while (!(LSM6DS3Core::LSM6DS3_one_register_read(LSM6DS3_I2C, LSM6DS3_STATUS_REG) & (1 << 2))){;}

  Wire.beginTransmission(LSM6DS3_I2C); 
  Wire.write(0x20); // Adres rozpoczynający odczytywanie danych z żyroskopu  (Dokumentacja str. 66)
  Wire.endTransmission(false); // Nie kończ transmisji na jednym rejestrze 
  Wire.requestFrom(LSM6DS3_I2C, 2, true); // Odczytanie danych z 6 rejestrów, po dwa rejestry dla każdej z osi

  T = (Wire.read() | Wire.read() << 8); // oś X
  OutData->Ta = ((float)T / 16.0f); // dzielenie przez 16 aby przeskalowac wynik
  OutData->Ta += 25; // dodanie 25 stopni do wyniku, jest to ustawiony offset
}

