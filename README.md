# ARDUINO_NANO_33_UoT_library
My own library to support the LSM6DS3 accelerometer built into the Arduino NANO 33 IoT board.


To run the library, you need the Visual Studio Code environment with the platform.io extension installed. A test program is written in the main.c file, running this file will run the sensor and read the measurement results, to see the results, install a program that reads data from the serial port

The entire library is based on communication with the LSM6DS3 sensor via the I2C interface. The Wire.h libraries from Arduino were used to support this interface.
Doxygen documentation: In the project folder, doxygen directory -> html -> index.html
System documentation:
https://content.arduino.cc/assets/st_imu_lsm6ds3_datasheet.pdf

All available registers related to the LSM6DS3 sensor have been declared in the Lsm6ds3IoT.h header file. The structures of variables storing the results and configuration parameters were created. The basic functions of the sensor have been declared in the LSM6DS3Core class. In the LSM6DS3 class there are additional functions to support the sensor, this class is inherited from the LSM6DS3Core class. All functions are described with appropriate comments in the code so that there is no problem with understanding the program's operation.