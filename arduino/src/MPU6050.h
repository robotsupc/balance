#ifndef __ACCELEROMETER_H__
#define __ACCELEROMETER_H__

#include <Arduino.h>

class MPU6050 {
public:
  int16_t acc[3];
  int16_t gyro[3];

  MPU6050();

  void begin(int sda, int scl);


  void loop();

  inline void begin(int rate) {
    this->sampleRate = rate;
  };
private:
  int addr;
  int sda;
  int scl;
  int sampleRate;

};


#endif /* __ACCELEROMETER_H__ */
