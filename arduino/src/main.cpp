// MPU-6050 Short Example Sketch
// By Arduino User JohnChi
// August 17, 2014
// Public Domain
#include <Arduino.h>
#include "MPU6050.h"


#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#define SSID "MOVISTAR_79F5"
#define PASSWORD "43811ab22edb86ee3e65"

#define MPU_ADD 0x68  // I2C address of the MPU-6050
#define SDA_PIN D4
#define SCL_PIN D5

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
#define STOP -1

#define DIR_CHANGE_TIMEOUT 500

#define INVERT_LEFT false
#define INVERT_RIGHT true

#define FAST PWMRANGE
#define LOG_ACC false

#define PWMFREQ 50 // 1khz by default
#define DEBUG false

// Direction of the motors
int dir = STOP;
int oldDir = STOP;

MPU6050 mpu;
ESP8266WebServer server(80);

String style = "<style>body{background-color:#b2dfdb;} a{margin:.8em;display:block;text-align: center;padding: .8em 3em; background: green;width: 150px;padding: .8em 3em;color:white;text-decoration:none;border-radius: 1em;}</style>";

String page = "<html>"+style+
  "<a href=\"forward\">Forward</a><a href=\"backward\">Backward</a><a href=\"stop\">Stop</a></html>";


void writeMotor(int motor, int direction, float speed) {
  if (direction == STOP) {
    writeMotor(motor, FORWARD, 0);
    return;
  }

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

  Serial.print("Writing low: ");
  Serial.print(low);
  Serial.print(", pwm: ");
  Serial.print(pwm);
  Serial.print(", speed: ");
  Serial.println(speed);

  analogWrite(low, 0);
  analogWrite(pwm, (int)(speed));

  /*int min = PWMRANGE * MIN_SPEED;
  int range = (PWMRANGE-min)*speed;
  analogWrite(pwm, (int)(min+range));*/
}

void writeMotors(int direction, float speed) {
  writeMotor(LEFT, direction, speed);
  writeMotor(RIGHT, direction, speed);
}

void moveRobot() {
  if (dir == oldDir) return;
  oldDir = dir;
  Serial.print("New dir: ");
  Serial.println(dir);

  if (DEBUG) {
    Serial.print("Dir: ");
    Serial.print(dir);
  }
  writeMotors(dir, FAST);
}



void setup() {
  Serial.begin(9600);

  //analogWriteFreq(PWMFREQ);

  delay(1000);
  // WiFi
  WiFi.begin(SSID, PASSWORD);
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(SSID);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", [](){
      server.send(200, "text/html", page);
  });

  server.on("/forward", [](){
    dir = FORWARD;
    Serial.println("fwd");
    server.send(200, "text/html", page);
  });

  server.on("/backward", [](){
    dir = BACKWARD;
    Serial.println("back");
    server.send(200, "text/html", page);
  });

  server.on("/stop", [](){
    dir = STOP;
    Serial.println("Stop");
    server.send(200, "text/html", page);
  });

  server.begin();


  mpu.begin(SDA_PIN, SCL_PIN);

  // Motors
  pinMode(MLA, OUTPUT);
  pinMode(MLB, OUTPUT);
  pinMode(MRA, OUTPUT);
  pinMode(MRB, OUTPUT);
}

unsigned long lastPrintAccelTime = 0;
void printAccelerometer() {
  // Don't flood the serial monitor
  if (millis() - lastPrintAccelTime < 5000) return;
  lastPrintAccelTime = millis();

  Serial.print("acc = "); Serial.print(mpu.acc[0]);
  Serial.print(","); Serial.print(mpu.acc[1]);
  Serial.print(","); Serial.print(mpu.acc[2]);
  Serial.print(" | gyro = "); Serial.print(mpu.gyro[0]);
  Serial.print(","); Serial.print(mpu.gyro[1]);
  Serial.print(","); Serial.println(mpu.gyro[2]);
}

void loop(){
  mpu.loop();
  if (LOG_ACC) printAccelerometer();

  moveRobot();
  server.handleClient();
  delay(10);
}
