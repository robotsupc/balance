// MPU-6050 Short Example Sketch
// By Arduino User JohnChi
// August 17, 2014
// Public Domain
#include <Arduino.h>
#include <MadgwickAHRS.h>
#include "MPU6050.h"


#define FREQ 100

Madgwick filter;
MPU6050 mpu;
unsigned long microsPerReading, microsPrevious;
float accelScale, gyroScale;

void setup() {
  Serial.begin(115200);
  mpu.begin(D4, D5);
  filter.begin(FREQ);


  microsPerReading = 1000000 / FREQ;
  microsPrevious = micros();
}

void loop() {
  int aix, aiy, aiz;
  int gix, giy, giz;
  float ax, ay, az;
  float gx, gy, gz;
  float roll, pitch, heading;
  unsigned long microsNow;

  // check if it's time to read data and update the filter
  microsNow = micros();
  if (microsNow - microsPrevious >= microsPerReading) {

    mpu.loop();

    // update the filter, which computes orientation
    //filter.updateIMU(gx, gy, gz, ax, ay, az);
    filter.updateIMU(
      mpu.gyro[0],
      mpu.gyro[1],
      mpu.gyro[2],
      mpu.acc[0],
      mpu.acc[1],
      mpu.acc[2]
    );

    Serial.print("Acc: ");
    Serial.print(mpu.acc[0]);
    Serial.print(" ");
    Serial.print(mpu.acc[1]);
    Serial.print(" ");
    Serial.print(mpu.acc[2]);
    Serial.print(" Gyro ");
    Serial.print(mpu.acc[0]);
    Serial.print(" ");
    Serial.print(mpu.acc[1]);
    Serial.print(" ");
    Serial.print(mpu.acc[2]);
    Serial.println();

    // print the heading, pitch and roll
    roll = filter.getRoll();
    pitch = filter.getPitch();
    heading = filter.getYaw();
    Serial.print("Orientation: ");
    Serial.print(heading);
    Serial.print(" ");
    Serial.print(pitch);
    Serial.print(" ");
    Serial.println(roll);

    // increment previous time, so we keep proper pace
    microsPrevious = microsPrevious + microsPerReading;
  }
}
