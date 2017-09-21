#include "MPU9250.h"

uint8_t MPU9250::begin() {
  Wire.begin();
  Wire.setClock(400000L);
  
  if (__read_byte(MPU9250_ADDRESS, WHO_AM_I) != WHO_AM_I_RESP)
  {
    while(1){
      Serial.print("MPU ID is: 0x");
      Serial.print(WHO_AM_I_RESP, HEX);
      Serial.print(", but the correct should be: 0x");
      Serial.println(__read_byte(MPU9250_ADDRESS, WHO_AM_I), HEX);
      delay(1000);
    }
  }
  
  // Reset the internal registers and restores the default settings.
  __write_byte(MPU9250_ADDRESS, PWR_MGMT_1, 0b10000000);
  
  delay(100);
  
  // Activate all sensors
  __write_byte(MPU9250_ADDRESS, PWR_MGMT_2, 0x00); 


  //Mag startup
  // Activate bypass to access Magnetometer
  __write_byte(MPU9250_ADDRESS, INT_PIN_CFG, 0x02);

  // Verify ID
  if (__read_byte(AK8963_ADDRESS, WHO_AM_I_AK8963) != WHO_AM_I_RESP_MAG)
  {
    while(1){
      Serial.print("MAG ID is: 0x");
      Serial.print(WHO_AM_I_RESP_MAG, HEX);
      Serial.print(", but the correct should be: 0x");
      Serial.println(__read_byte(AK8963_ADDRESS, WHO_AM_I_AK8963), HEX);
      delay(1000);
    }
  }

  // Read factory ajustment data
  __write_byte(AK8963_ADDRESS, AK8963_CNTL, 0x00);
  delay(10);
  // Enter Fuse ROM access mode
  __write_byte(AK8963_ADDRESS, AK8963_CNTL, 0x0F); 
  delay(10);

  // Read the x-, y-, and z-axis calibration values
  __read_bytes(AK8963_ADDRESS, AK8963_ASAX, 3);

  // Return x-axis sensitivity adjustment values, etc.
  MagAdjustment[0] =  (float)(Wire.read() - 128)/256. + 1.;
  MagAdjustment[1] =  (float)(Wire.read() - 128)/256. + 1.;
  MagAdjustment[2] =  (float)(Wire.read() - 128)/256. + 1.;
  __write_byte(AK8963_ADDRESS, AK8963_CNTL, 0x00); // Power down magnetometer
  delay(10);

  // Set magnetometer data resolution and sample ODR
  __write_byte(AK8963_ADDRESS, AK8963_CNTL, 0b00010110);
  delay(10);
  
  return 0;
}

/**************************************
Accel
**************************************/

void MPU9250::get_accel()
{
  __read_bytes(MPU9250_ADDRESS, ACCEL_XOUT_H, 6);

  x = (Wire.read()<<8)|(Wire.read());
  y = (Wire.read()<<8)|(Wire.read());
  z = (Wire.read()<<8)|(Wire.read());
}

void MPU9250::get_accel_g()
{
  get_accel();

  uint8_t range = get_accel_range();
  uint16_t divider;
  if (range == RANGE_16G) divider = 2048;
  if (range == RANGE_8G) divider = 4096;
  if (range == RANGE_4G) divider = 8192;
  if (range == RANGE_2G) divider = 16384;

  x_g = (float)x / divider;
  y_g = (float)y / divider;
  z_g = (float)z / divider;
}

void MPU9250::set_accel_range(accel_range range)
{
  uint8_t reg = __read_byte(MPU9250_ADDRESS, ACCEL_CONFIG);
  __write_byte(MPU9250_ADDRESS, ACCEL_CONFIG, ((reg & 0b11100111)|(range<<3)));
}

accel_range MPU9250::get_accel_range()
{
  /* Read the data format register to preserve bits */
  return (accel_range)((__read_byte(MPU9250_ADDRESS, ACCEL_CONFIG) & 0b00011000)>>3);
}

/**************************************
Gyro
**************************************/

void  MPU9250::get_gyro()
{
  while(!(__read_byte(MPU9250_ADDRESS, INT_STATUS)&0b1));
  
  __read_bytes(MPU9250_ADDRESS, GYRO_XOUT_H, 6);
  
  gx = (Wire.read()<<8)|(Wire.read());
  gy = (Wire.read()<<8)|(Wire.read());
  gz = (Wire.read()<<8)|(Wire.read());
}

void MPU9250::get_gyro_d()
{
  get_gyro();

  uint8_t range = get_gyro_range();
  float divider;
  if (range == RANGE_GYRO_2000) divider = 16.4;
  if (range == RANGE_GYRO_1000) divider = 32.8;
  if (range == RANGE_GYRO_500) divider = 65.5;
  if (range == RANGE_GYRO_250) divider = 131.0;

  gx_d = (float)gx / divider;
  gy_d = (float)gy / divider;
  gz_d = (float)gz / divider;
}

void MPU9250::set_gyro_range(gyro_range range)
{
  uint8_t reg = __read_byte(MPU9250_ADDRESS, GYRO_CONFIG);
  __write_byte(MPU9250_ADDRESS, GYRO_CONFIG, ((reg & 0b11100111)|(range<<3)));
}

gyro_range MPU9250::get_gyro_range()
{
  return (gyro_range)((__read_byte(MPU9250_ADDRESS, ACCEL_CONFIG) & 0b00011000)>>3);
}

/**************************************
Mag
**************************************/

int MPU9250::get_mag()
{
  uint8_t temp[7];
  
  while(!(__read_byte(AK8963_ADDRESS, AK8963_ST1)&0x01));
  
  __read_bytes(AK8963_ADDRESS, AK8963_XOUT_L, 7);

  for(int i=0; i<=6; i++)
    temp[i] = Wire.read();

  if(!(temp[6] & 0x08)){
    //Data stored as little endian
    mx = (temp[0])|(temp[1]<<8);
    my = (temp[2])|(temp[3]<<8);
    mz = (temp[4])|(temp[5]<<8);
  }
  else
  {
    return 1;
  }
  return 0;
}

int MPU9250::get_mag_t()
{
  if(get_mag()){
    return 1;
  }

  uint8_t range = get_mag_scale();
  float divider;
  if (range == SCALE_14_BITS) divider = 8190.0;
  if (range == SCALE_16_BITS) divider = 32760.0;
    
  mx_t = (float)mx*4912.0*MagAdjustment[0] / divider;
  my_t = (float)my*4912.0*MagAdjustment[1] / divider;
  mz_t = (float)mz*4912.0*MagAdjustment[2] / divider;

  return 0;
}

void MPU9250::set_mag_scale(mag_scale scale)
{
  // Save mag configs
  uint8_t temp = __read_byte(AK8963_ADDRESS, AK8963_CNTL) & 0b00000110;
  // disable mag
  __write_byte(AK8963_ADDRESS, AK8963_CNTL, 0);
  delay(10);
  __write_byte(AK8963_ADDRESS, AK8963_CNTL, (scale << 4) | temp);
}

mag_scale MPU9250::get_mag_scale()
{
  return((__read_byte(AK8963_ADDRESS, AK8963_CNTL)>>4)&0x01);
}

void MPU9250::set_mag_speed(mag_speed mspeed)
{
  // Save mag configs
  uint8_t temp = __read_byte(AK8963_ADDRESS, AK8963_CNTL) & 0b00010000;
  // disable mag
  __write_byte(AK8963_ADDRESS, AK8963_CNTL, 0);
  delay(10);
  if(mspeed==MAG_8_Hz)
    __write_byte(AK8963_ADDRESS, AK8963_CNTL, 0b0010| temp);
  else
    __write_byte(AK8963_ADDRESS, AK8963_CNTL, 0b0110| temp);
}

mag_speed MPU9250::get_mag_speed()
{
  uint8_t temp = __read_byte(AK8963_ADDRESS, AK8963_CNTL)&0x0F;
  if (temp==0b0010)
    return MAG_8_Hz;
  else
    return MAG_100_Hz;
}

/**************************************
Temperature
**************************************/

int16_t MPU9250::get_temp(){
  __read_bytes(MPU9250_ADDRESS, TEMP_OUT_H, 2);

  return ((Wire.read()<<8)|Wire.read());
}

/**************************************
Functions to read and write in I2C
**************************************/

void MPU9250::__write_byte(uint8_t adress, uint8_t reg, uint8_t value)
{
  Wire.beginTransmission(adress);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

uint8_t MPU9250::__read_byte(uint8_t adress, uint8_t reg)
{
    #if defined(__SAM3X8E__)
      // For Arduino Due bug
      Wire.requestFrom(adress, 1, reg, 1, true);
    #else
      Wire.beginTransmission(adress);
      Wire.write(reg);
      Wire.endTransmission(false);
  
      Wire.requestFrom(adress, 1);
    #endif
    
    if (!Wire.available()){
      return 0;
    }
    return Wire.read();
}

inline void MPU9250::__read_bytes(uint8_t adress, uint8_t reg, int qty){
  #if defined(__SAM3X8E__)
    // For Arduino Due bug
    Wire.requestFrom(adress, qty, reg, 1, true);
  #else
    Wire.beginTransmission(adress);
    Wire.write(reg);
    Wire.endTransmission(false);

    Wire.requestFrom(adress, qty);
  #endif
}
