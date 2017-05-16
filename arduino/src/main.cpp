#include <Arduino.h>
#include <Wire.h>

#define MLA D6 // Left Motor direction (A)
#define MLB D7 // Left Motor speed (B)
#define MRA D2 // Right Motor direction (A)
#define MRB D3 // Right Motor speed (B)
#define MPU_SDA D4 // Data line for I2C
#define MPU_SCL D5 // Clock line for I2C

#define LEFT 0
#define RIGHT 1
#define FORWARD 0
#define BACKWARD 1

#define MIN_SPEED 0.8
#define DIR_CHANGE_TIMEOUT 500

#define INVERT_LEFT false
#define INVERT_RIGHT true

#define FAST PWMRANGE




void setup() {
  Serial.begin(115200);
  pinMode(MLA, OUTPUT);
  pinMode(MLB, OUTPUT);
  pinMode(MRA, OUTPUT);
  pinMode(MRB, OUTPUT);
}

void writeMotor(int motor, int direction, float speed) {
  int low = MLA;
  int pwm = MLB;
  if (motor == RIGHT) {
    low = MRA;
    pwm = MRB;
  }
  if (direction == BACKWARD) { int tmp = low; low = pwm; pwm = tmp; }
  if (motor == LEFT && INVERT_LEFT
    || motor == RIGHT && INVERT_RIGHT) {
      int tmp = low; low = pwm; pwm = tmp;
  }

  digitalWrite(low, LOW);
  /*int min = PWMRANGE * MIN_SPEED;
  int range = (PWMRANGE-min)*speed;
  analogWrite(pwm, (int)(min+range));*/
  analogWrite(pwm, (int)(speed));
}

void writeMotors(int direction, float speed) {
  writeMotor(LEFT, direction, speed);
  writeMotor(RIGHT, direction, speed);
}
void loop() {
  Serial.println("Hi");
  int t = 1000;
  int u = DIR_CHANGE_TIMEOUT;


  writeMotors(BACKWARD, FAST); delay(t);
  writeMotors(BACKWARD, 0); delay(u);

  writeMotors(FORWARD, FAST); delay(t);
  writeMotors(FORWARD, 0); delay(u);
}
