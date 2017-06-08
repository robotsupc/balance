#include <MPU6050.h>
#include <Wire.h>

#define ACCELEROMETER_SENSITIVITY 8192.0
#define GYROSCOPE_SENSITIVITY 65.536

#define M_PI 3.14159265359
#define INVERT_AXIS false


float dt;

/*
// http://www.pieter-jan.com/node/11
void MPU6050::complementaryFilter() {
    float pitchAcc, rollAcc;

    // Integrate the gyroscope data -> int(angularSpeed) = angle
    this->pitch += ((float)this->gyro[0]) * dt; // Angle around the X-axis
    this->roll -= ((float)this->gyro[1]) * dt;    // Angle around the Y-axis

    // Compensate for drift with accelerometer data if !bullshit
    // Sensitivity = -2 to 2 G at 16Bit -> 2G = 32768 && 0.5G = 8192
    int forceMagnitudeApprox = abs(this->acc[0]) + abs(this->acc[1]) + abs(this->acc[2]);
    if (forceMagnitudeApprox > 8192 && forceMagnitudeApprox < 32768)
    {
	// Turning around the X axis results in a vector on the Y-axis
        pitchAcc = atan2f((float)this->acc[1], (float)this->acc[2]) * 180 / M_PI;
        this->pitch = this->pitch * this->f + pitchAcc * (1-this->f);

	// Turning around the Y axis results in a vector on the X-axis
        rollAcc = atan2f((float)this->acc[0], (float)this->acc[2]) * 180 / M_PI;
        this->roll = this->roll * this->f + rollAcc * (1-this->f);
    }
}
*/


MPU6050::MPU6050() {
  this-> acc[0] = this-> acc[1] = this-> acc[2] = 0;
  this-> gyro[0] = this-> gyro[1] = this-> gyro[2] = 0;
  this-> pitch = this->roll = 0;
  this->lastUpdate = -1;
}

void MPU6050::begin(int sda, int scl) {
  this->sda = sda;
  this->scl = scl;
  this->addr = 0x68;

  Wire.begin(sda, scl); // sda, scl
  Wire.beginTransmission(this->addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
}



void MPU6050::loop() {
  if (this->lastUpdate == -1) {
    this->lastUpdate = millis();
  }
  Wire.beginTransmission(this->addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(this->addr,14,true);  // request a total of 14 registers
  acc[0]=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  acc[1]=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  acc[2]=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  int temperature =Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  gyro[0]=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  gyro[1]=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  gyro[2]=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

  if (INVERT_AXIS) {
    for (int i = 0; i < 3; i++) {
      acc[i] *= -1; gyro[i] *= -1;
    }
  }

  for (int i = 0; i < 3; i++) {
    acc[i] /= ACCELEROMETER_SENSITIVITY;
    gyro[i] /= GYROSCOPE_SENSITIVITY;
  }

  // process the data
  unsigned long t = millis();
  dt = (t - this->lastUpdate) / 1000.0;
  this->lastUpdate = t;

  //complementaryFilter();
}
