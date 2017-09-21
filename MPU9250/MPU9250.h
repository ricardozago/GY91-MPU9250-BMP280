#ifndef MPU9250_h
#define MPU9250_h

#include <Wire.h>
#include <arduino.h>

#define MPU9250_ADDRESS  0x68 // Change to 0x69 if AD0 is in high state
#define WHO_AM_I_RESP    0x73 // MPU9250 is 0x71 and MPU9255 is 0x73

#define AK8963_ADDRESS   0x0C // MPU9250 must bypass to access the AK8963 in INT_PIN_CFG
#define WHO_AM_I_RESP_MAG 0x48

//Accel and Gyro registers
#define SMPLRT_DIV       0x19
#define CONFIG           0x1A
#define GYRO_CONFIG      0x1B
#define ACCEL_CONFIG     0x1C
#define ACCEL_CONFIG2    0x1D
#define INT_PIN_CFG      0x37
#define INT_ENABLE       0x38
#define INT_STATUS       0x3A
#define ACCEL_XOUT_H     0x3B
#define TEMP_OUT_H       0x41
#define GYRO_XOUT_H      0x43
#define PWR_MGMT_1       0x6B
#define PWR_MGMT_2       0x6C
#define WHO_AM_I         0x75

//Magnetometer Registers
#define WHO_AM_I_AK8963  0x00 // should return 0x48
#define AK8963_ST1       0x02  // data status
#define AK8963_XOUT_L    0x03  // data
#define AK8963_CNTL      0x0A  // Power down (0000), Continuous measurement mode 1 (0010), CMM2 (0110), Fuse ROM (1111) on bits 3:0
#define AK8963_ASAX      0x10  // Fuse ROM x-axis sensitivity adjustment value
#define AK8963_ASAY      0x11  // Fuse ROM y-axis sensitivity adjustment value
#define AK8963_ASAZ      0x12  // Fuse ROM z-axis sensitivity adjustment value

typedef enum
{
  RANGE_16G          = 0b11,
  RANGE_8G           = 0b10,
  RANGE_4G           = 0b01,
  RANGE_2G           = 0b00
} accel_range;

typedef enum
{
  RANGE_GYRO_2000    = 0b11,
  RANGE_GYRO_1000    = 0b10,
  RANGE_GYRO_500     = 0b01,
  RANGE_GYRO_250     = 0b00
} gyro_range;

typedef enum
{
  SCALE_14_BITS      = 0,
  SCALE_16_BITS      = 1
} mag_scale;

typedef enum
{
  MAG_8_Hz           = 0,
  MAG_100_Hz         = 1
} mag_speed;


class MPU9250
{
  public:
    uint8_t begin();
  
    //Accel
    void get_accel();
    void get_accel_g();
    int16_t x, y, z;
    float x_g, y_g, z_g;
    void set_accel_range(accel_range range);
    accel_range get_accel_range();
  
    //Gyro
    void get_gyro();
    void get_gyro_d();
    int16_t gx, gy, gz;
    float gx_d, gy_d, gz_d;
    void set_gyro_range(gyro_range range);
    gyro_range get_gyro_range();
  
    //Mag
    int get_mag();
    int get_mag_t();
    int16_t mx, my, mz;
    float mx_t, my_t, mz_t;
    void set_mag_scale(mag_scale scale);
    mag_scale get_mag_scale();
    void set_mag_speed(mag_speed mspeed);
    mag_speed get_mag_speed();
    float MagAdjustment[3] = {0, 0, 0};

    //Temp
    int16_t get_temp();
  
  private:
    void __write_byte(uint8_t adress, uint8_t reg, uint8_t value);
    uint8_t __read_byte(uint8_t adress, uint8_t reg);
    inline void __read_bytes(uint8_t adress, uint8_t reg, int qty);
};

#endif
