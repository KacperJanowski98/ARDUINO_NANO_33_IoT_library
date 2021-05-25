/**
 * @file Lsm6ds3IoT.h
 * @author Kacper Janowski
 * @brief 
 * @version 0.1
 * @date 2021-05-04
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef _LSM6DS3IOT_H_
#define _LSM6DS3IOT_H_

#include "Arduino.h"
#include "Wire.h"

/************* LSM6DS3 sensor I2C adres **************/
#define LSM6DS3_I2C                             0x6A 

/**************** Adresy w urządzeniu ****************/
#define LSM6DS3_FUNC_CFG_ACCESS 			0X01
#define LSM6DS3_SENSOR_SYNC_TIME  	    	      0X04
#define LSM6DS3_FIFO_CTRL1  		        	0X06
#define LSM6DS3_FIFO_CTRL2  		        	0X07
#define LSM6DS3_FIFO_CTRL3  	        		0X08
#define LSM6DS3_FIFO_CTRL4  	        		0X09
#define LSM6DS3_FIFO_CTRL5  	        		0X0A
#define LSM6DS3_ORIENT_CFG_G  	      	0X0B
#define LSM6DS3_INT1_CTRL  		          	0X0D
#define LSM6DS3_INT2_CTRL  		          	0X0E
#define LSM6DS3_WHO_AM_I_REG  	      	0X0F
#define LSM6DS3_CTRL1_XL  		          	0X10
#define LSM6DS3_CTRL2_G  		            0X11
#define LSM6DS3_CTRL3_C  		            0X12
#define LSM6DS3_CTRL4_C  		            0X13
#define LSM6DS3_CTRL5_C  		            0X14
#define LSM6DS3_CTRL6_C  		            0X15
#define LSM6DS3_CTRL7_G  		            0X16
#define LSM6DS3_CTRL8_XL  		          	0X17
#define LSM6DS3_CTRL9_XL  		          	0X18
#define LSM6DS3_CTRL10_C  		          	0X19
#define LSM6DS3_MASTER_CONFIG  	        	0X1A
#define LSM6DS3_WAKE_UP_SRC  		        	0X1B
#define LSM6DS3_TAP_SRC  		            0X1C
#define LSM6DS3_D6D_SRC  		            0X1D
#define LSM6DS3_STATUS_REG  		        	0X1E
#define LSM6DS3_OUT_TEMP_L  		        	0X20
#define LSM6DS3_OUT_TEMP_H  		        	0X21
#define LSM6DS3_OUTX_L_G  		          	0X22
#define LSM6DS3_OUTX_H_G  		          	0X23
#define LSM6DS3_OUTY_L_G  		          	0X24
#define LSM6DS3_OUTY_H_G  		          	0X25
#define LSM6DS3_OUTZ_L_G  		          	0X26
#define LSM6DS3_OUTZ_H_G  		          	0X27
#define LSM6DS3_OUTX_L_XL  		          	0X28
#define LSM6DS3_OUTX_H_XL  		          	0X29
#define LSM6DS3_OUTY_L_XL  		          	0X2A
#define LSM6DS3_OUTY_H_XL  		          	0X2B
#define LSM6DS3_OUTZ_L_XL  		          	0X2C
#define LSM6DS3_OUTZ_H_XL  			      0X2D
#define LSM6DS3_SENSORHUB1_REG  	      	0X2E
#define LSM6DS3_SENSORHUB2_REG  	      	0X2F
#define LSM6DS3_SENSORHUB3_REG  	      	0X30
#define LSM6DS3_SENSORHUB4_REG  	      	0X31
#define LSM6DS3_SENSORHUB5_REG  	      	0X32
#define LSM6DS3_SENSORHUB6_REG  	      	0X33
#define LSM6DS3_SENSORHUB7_REG  	      	0X34
#define LSM6DS3_SENSORHUB8_REG  	      	0X35
#define LSM6DS3_SENSORHUB9_REG  	      	0X36
#define LSM6DS3_SENSORHUB10_REG  	      	0X37
#define LSM6DS3_SENSORHUB11_REG  		      0X38
#define LSM6DS3_SENSORHUB12_REG  	      	0X39
#define LSM6DS3_FIFO_STATUS1  	        	0X3A
#define LSM6DS3_FIFO_STATUS2  		      0X3B
#define LSM6DS3_FIFO_STATUS3  		      0X3C
#define LSM6DS3_FIFO_STATUS4  		      0X3D
#define LSM6DS3_FIFO_DATA_OUT_L  	      	0X3E
#define LSM6DS3_FIFO_DATA_OUT_H  		      0X3F
#define LSM6DS3_TIMESTAMP0_REG  	      	0X40
#define LSM6DS3_TIMESTAMP1_REG  	      	0X41
#define LSM6DS3_TIMESTAMP2_REG  	      	0X42
#define LSM6DS3_STEP_COUNTER_L  	      	0X4B
#define LSM6DS3_STEP_COUNTER_H  	      	0X4C
#define LSM6DS3_FUNC_SRC  		          	0X53
#define LSM6DS3_TAP_CFG1  		           	0X58
#define LSM6DS3_TAP_THS_6D  	        		0X59
#define LSM6DS3_INT_DUR2  		          	0X5A
#define LSM6DS3_WAKE_UP_THS  		        	0X5B
#define LSM6DS3_WAKE_UP_DUR  		        	0X5C
#define LSM6DS3_FREE_FALL  		          	0X5D
#define LSM6DS3_MD1_CFG  		            0X5E
#define LSM6DS3_MD2_CFG  		            0X5F

/***** Tryby pracy akcelerometru (ODR_XL - CTRL1_XL) ******/
#define POWER_DOWN                              0x0
#define LOW_POWER_12_5                          0x1
#define LOW_POWER_26                            0x2
#define LOW_POWER_52                            0x3
#define NORMAL_MODE_104                         0x4
#define NORMAL_MODE_208                         0x5
#define HIGH_PERFORMANCE_416                    0x6
#define HIGH_PERFORMANCE_833                    0x7
#define HIGH_PERFORMANCE_1_66                   0x8
#define HIGH_PERFORMANCE_3_33                   0x9
#define HIGH_PERFORMANCE_6_66                   0xA

/********** Przyśpieszenie (FS_XL - CTRL1_XL) *************/
#define ACC_2G                                  0x0
#define ACC_4G                                  0x2
#define ACC_8G                                  0x3
#define ACC_16G                                 0x1

/*********** Pasmo filtru (BW_XL - CTRL1_XL) **************/
#define ACC_FILTER_400                          0x0
#define ACC_FILTER_200                          0x1
#define ACC_FILTER_100                          0x2
#define ACC_FILTER_50                           0x3

/******* Inkrementacja adresów (IF_INC - CTRL3_C) ********/
#define ACC_AUTO_INC_ADDR                       0x04

/* Wybór pasma akcelerometru (XL_BW_SCAL_ORD - CTRL4_C) */
#define ACC_BANDWIDTH_ODR                       ~(1 << 7)
#define ACC_BANDWIDTH_BW_XL                     (1 << 7)

/** Ta struktura zawiera ustawienia używane do obliczeń **/
typedef struct {
  public:
        // Gyroscope
        uint8_t gyroEnable;
        uint16_t gyroRange;
        uint16_t gyroSampleRate;
        uint16_t gyroBandWidth;

        // Accelerometer
        uint8_t accelEnable;
        uint8_t accelODROff;
        uint16_t accelRange;
        uint16_t accelSampleRate;
        uint16_t accelBandWidth;
        uint16_t accelInitialVal;

        // Temerature
        uint8_t tempEnable;
}SensorSettings_t;

/** Ta struktura zawiera zmienne dla dane wyjściowe **/
typedef struct {
public:
      float Xa;
      float Ya;
      float Za;
}AccelOutput_t;

/**
 * @brief Klasa zawierająca podstawowe funkcje/metody sensora LSM6DS3
 * 
 */
class LSM6DS3Core
{
public:
      void ACC_Mode_Init(SensorSettings_t *Settings, uint16_t ODR_XL, uint16_t FS_XL, uint16_t BW_XL);

      void Accelerometer_Init(uint16_t Value);

      uint16_t Accelerometer_one_register_read(uint16_t DeviceAddr, uint16_t RegisterAddr);

      void Accelerometer_multiple_register_read(uint16_t DeviceAddr, uint16_t RegisterAddr, uint16_t numOfReg);

      void Accelerometer_register_write(uint16_t DeviceAddr, uint16_t RegisterAddr, uint16_t Value);
};

/**
 * @brief Klasa rozszerzająca podstawowe funkcje/metody sensora LSM6DS3
 * 
 */
class LSM6DS3 : public LSM6DS3Core
{
public:
      SensorSettings_t settings;

      void Accelerometer_XYZ_Output_open();
      
      void Accelerometer_XYZ_read_value(AccelOutput_t *OutData, SensorSettings_t *range);

      void Accelerometer_High_perf_Disable(LSM6DS3 lsm6ds3);

      void Accelerometer_High_perf_Enable(LSM6DS3 lsm6ds3);
};

#endif //Accelerometer