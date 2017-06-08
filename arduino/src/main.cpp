// MPU-6050 Short Example Sketch
// By Arduino User JohnChi
// August 17, 2014
// Public Domain
#include <Arduino.h>
#include "MPU6050.h"


#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#define SSID "vodafoneC8B8"
#define PASSWORD "R6VPZF9N7D6FZP"

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

#define FAST 100
#define SLOW 50
#define LOG_ACC false
#define ACC_PRINT_FREQ 20

#define PWMFREQ 10 // 1khz by default
#define DEBUG false

#define ROLL_CALIB 0

#define SKIP_WIFI_SETUP true

#define F 0.93

MPU6050 mpu;
ESP8266WebServer server(80);

String userCommand;



String webCommand(String cmd);
String style = "<style>body{background-color:#b2dfdb;} a{margin:.8em;display:block;text-align: center;padding: .8em 3em; background: green;width: 150px;padding: .8em 3em;color:white;text-decoration:none;border-radius: 1em;}</style>";
String page = "<html>"+style
  + webCommand("forward")
  + webCommand("backward")
  + webCommand("stop")
  + webCommand("pulse_fwd_fast")
  + webCommand("pulse_bwd_fast")
  + webCommand("pulse_fwd_slow")
  + webCommand("pulse_bwd_slow")
  + "</html>";


String webCommand(String cmd) {
  server.on(("/"+cmd).c_str(), [cmd](){
    userCommand = cmd;
    Serial.println("CMD:"+cmd);
    server.send(200, "text/html", page);
  });
  return "<a href=\""+cmd+"\">"+cmd+"</a>";
}


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
  analogWrite(low, 0);
  analogWrite(pwm, (int)(speed));
}

void writeMotors(int direction, float speed) {
  writeMotor(LEFT, direction, speed);
  writeMotor(RIGHT, direction, speed);
}

void setupWifi() {
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
  server.begin();

}

void setup() {

  writeMotors(FORWARD, 0);
  Serial.begin(115200);

  analogWriteFreq(PWMFREQ);

  delay(1000);
  if (!SKIP_WIFI_SETUP) setupWifi();

  mpu.begin(SDA_PIN, SCL_PIN);
  mpu.f = F;

  // Motors
  pinMode(MLA, OUTPUT);
  pinMode(MLB, OUTPUT);
  pinMode(MRA, OUTPUT);
  pinMode(MRB, OUTPUT);



  Serial.println();
  Serial.println("OK");
}


unsigned long lastPrintAccelTime = 0;
void printAccelerometer() {
  // Don't flood the serial monitor
  if (millis() - lastPrintAccelTime < ACC_PRINT_FREQ) return;
  lastPrintAccelTime = millis();

  if (false) {
    Serial.print("acc "); Serial.print(mpu.acc[0]);
    Serial.print(","); Serial.print(mpu.acc[1]);
    Serial.print(","); Serial.print(mpu.acc[2]);
    Serial.print(", gyro "); Serial.print(mpu.gyro[0]);
    Serial.print(","); Serial.print(mpu.gyro[1]);
    Serial.print(","); Serial.print(mpu.gyro[2]);
    Serial.print(", pitch "); Serial.print(mpu.pitch);
    Serial.print(", roll "); Serial.println(mpu.roll);
  }

  float angle = mpu.roll-ROLL_CALIB;
/*
  Serial.print("roll "); Serial.print(mpu.roll);
  Serial.print(", "); Serial.print(angle);
  Serial.print(" speed = ");
  Serial.print(mpu.gyro[0] / 131);
  */
  //Serial.print(", ");
  Serial.println(mpu.gyro[1] / 131);
  /*Serial.print(", ");  Serial.print(mpu.gyro[2] / 131);
  Serial.println();
  */
}




void setMotors(int mla, int mlb, int mra, int mrb) {
  digitalWrite(MLA, mla);
  digitalWrite(MLB, mlb);
  digitalWrite(MRA, mra);
  digitalWrite(MRB, mrb);
}
void setMotorsForward() {
  setMotors(HIGH, LOW, LOW, HIGH);
}

void setMotorsBackward() {
  setMotors(LOW, HIGH, HIGH, LOW);
}

void setMotorsOff() {
  setMotors(LOW, LOW, LOW, LOW);
}

unsigned long lastMotorWrite = 0;
int pendingMotorDirection = STOP;


void detectFall() {
  float angle = mpu.roll;
  Serial.print(angle);
  Serial.print("    -   ");
  Serial.println(mpu.gyro[0]);

  int threshold = 4;
  if (angle >  threshold) pendingMotorDirection = FORWARD;
  if (angle < -threshold) pendingMotorDirection = BACKWARD;
}

bool active = false;
void updateMotors() {
  unsigned long t = millis();

  if (pendingMotorDirection != STOP && !active) {
    if (pendingMotorDirection == FORWARD) setMotorsBackward();
    if (pendingMotorDirection == BACKWARD) setMotorsForward();
    lastMotorWrite = t;
    active = true;
    return;
  }

  if (t-lastMotorWrite > 30) {
    active = false;
    pendingMotorDirection = STOP;
    setMotorsOff();
    delay(30);
  }
}

String readCommand() {
  if (!Serial.available()) return "";
  //Serial.println("available");
  Serial.setTimeout(5 * 1000);
  String s = Serial.readStringUntil(' ');
  s.trim();
  s.toLowerCase();

  return s;
}

void processCommand() {
  String s = readCommand();
  if (s == "") return;
  if (s == "fwd") setMotorsForward();
  else if (s == "bwd") setMotorsBackward();
  else if (s == "off") setMotorsOff();
  else {
    Serial.println("Invalid command: " + s);
    return;
  }
  Serial.println("OK: " + s);
}



void loop() {
  detectFall();
  //updateMotors();
  mpu.loop();

  processCommand();

  if (!SKIP_WIFI_SETUP) server.handleClient();
  delay(5);
}
