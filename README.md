# GY91 - 10 dof - MPU9250 (MPU9255) + BMP280

The GY91 board have 2 sensors: an InvenSense MPU9255 (yes! It's not the MPU9250) and a Bosch BMP280.

The MPU9250 is a tri-axial accelerometer, gyroscope and magnetometer, being a 9-dof sensor. It's use a 2 die solution in the same package, the accelerometer and gyroscope is an InvenSense MPU-6500 and the magnetometer is an Asahi Kasei AK8963.

The chips sold in online vendors like Aliexpress are the MPU9255, that is almost the same from MPU9250. The main diference for our porpoise is de ID, that is 0x71 for MPU9250 and 0x73 for MPU9255. So you need to change this ID in the header from Library (the default is for MPU9255). The diference between the two solutions is that de MPU9255 is for use within Automatic Activity Recognition library.

The datasheets for MPU9250 are:
* [InvenSense MPU-9250 Product Specification](https://github.com/ricardozago/GY91-MPU9250-BMP280/blob/master/Datasheet/PS-MPU-9250A-01-v1.1.pdf)
* [InvenSense MPU-9250 Register Map](https://github.com/ricardozago/GY91-MPU9250-BMP280/blob/master/Datasheet/RM-MPU-9250A-00-v1.6.pdf)
* [InvenSense MPU-9255 Register Map](https://github.com/ricardozago/GY91-MPU9250-BMP280/blob/master/Datasheet/MPU-9255-Register-Map.pdf)
* [Asahi Kasei AK8963](https://github.com/ricardozago/GY91-MPU9250-BMP280/blob/master/Datasheet/ak8963c-datasheet.pdf)

For the Bosch BMP280 sensor, we are using the [Adafruit Library BMP280](https://github.com/adafruit/Adafruit_BMP280_Library).

![GY91](https://github.com/ricardozago/GY91-MPU9250-BMP280/blob/master/Images/GY91.jpg)  
GY-91

![MPU9255](https://github.com/ricardozago/GY91-MPU9250-BMP280/blob/master/Images/MPU9255.jpg)  
InvenSense MPU9255

![BMP280](https://github.com/ricardozago/GY91-MPU9250-BMP280/blob/master/Images/BMP280.jpg)  
Bosch BMP280
