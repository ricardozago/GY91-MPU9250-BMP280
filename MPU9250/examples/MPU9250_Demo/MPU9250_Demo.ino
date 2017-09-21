#include "MPU9250.h"

MPU9250 mpu = MPU9250();

void setup(void) {
  Serial.begin(9600);  
  uint8_t temp = mpu.begin();
}

void loop() {
  //Accel
  mpu.set_accel_range(RANGE_4G);
  mpu.get_accel();
  Serial.print("X: ");  Serial.print(mpu.x);
  Serial.print(" Y: "); Serial.print(mpu.y);
  Serial.print(" Z: "); Serial.print(mpu.z);
  
  mpu.get_accel_g();
  Serial.print(" X_g: "); Serial.print(mpu.x_g,2); 
  Serial.print(" Y_g: "); Serial.print(mpu.y_g,2); 
  Serial.print(" Z_g: "); Serial.print(mpu.z_g,2);  Serial.println(" G");

  //Gyro
  mpu.set_gyro_range(RANGE_GYRO_250);
  mpu.get_gyro();
  Serial.print("GX: ");  Serial.print(mpu.gx); 
  Serial.print(" GY: "); Serial.print(mpu.gy); 
  Serial.print(" GZ: "); Serial.print(mpu.gz);

  mpu.get_gyro_d();
  Serial.print(" GX_g: "); Serial.print(mpu.gx_d,2); 
  Serial.print(" GY_g: "); Serial.print(mpu.gy_d,2); 
  Serial.print(" GZ_g: "); Serial.print(mpu.gz_d,2); Serial.println(" º/s");

  //Mag
  mpu.set_mag_scale(SCALE_14_BITS);
  mpu.set_mag_speed(MAG_8_Hz);
  if(!mpu.get_mag()){
    Serial.print("MX: ");  Serial.print(mpu.mx); 
    Serial.print(" MY: "); Serial.print(mpu.my); 
    Serial.print(" MZ: "); Serial.print(mpu.mz);

    mpu.get_mag_t();
    Serial.print(" MX_t: "); Serial.print(mpu.mx_t,2); 
    Serial.print(" MY_t: "); Serial.print(mpu.my_t,2); 
    Serial.print(" MZ_t: "); Serial.print(mpu.mz_t,2); Serial.println(" uT");
  }
  else{
    // |X|+|Y|+|Z| must be < 4912μT to sensor measure correctly 
    Serial.println("Overflow no magnetometro.");
  }
  
  // Temp     
  Serial.print("Temperature is ");  Serial.print((((float) mpu.get_temp()) / 333.87 + 21.0), 1);  Serial.println(" degrees C");
  
  delay(500);
}
