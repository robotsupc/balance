#ifndef __ACCELEROMETER_H__
#define __ACCELEROMETER_H__

#include <Arduino.h>

class MPU6050 {
public:
  short acc[3];
  short gyro[3];

  MPU6050();
  void begin(int sda, int scl);
  void loop();
  float roll;
  float pitch;
  // accelerometer vs gyro factor
  float f;


private:
  int lastUpdate;
  int addr;
  int sda;
  int scl;

  void complementaryFilter();
};




#endif /* __ACCELEROMETER_H__ */
