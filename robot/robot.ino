#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

#include "config.h"
#include "state.h"
#include "encoders.h"
#include "motors.h"
#include "autonomous.h"
#include "web.h"

const char* ssid = "ROBOT_ESP32";
const char* password = "roboy1234";

float Kp = 150.0f;
float Ki = 25.0f;
float Kd = 0.0f;
float RIGHT_MOTOR_SCALE = 0.90f;

volatile long leftTicks = 0;
volatile long rightTicks = 0;
long prevLeftTicks = 0;
long prevRightTicks = 0;

bool fwd = false;
bool bwd = false;
bool left = false;
bool right = false;

unsigned long lastCmdTime = 0;
unsigned long lastControlTime = 0;

AutoState autoState = AUTO_IDLE;

float requestedX = 0;
float requestedY = 0;
float remainingX = 0;
float remainingY = 0;
float targetTurnAngle = 0;
float autoStartHeading = 0;
float autoTargetHeading = 0;

long autoStartLeftTicks = 0;
long autoStartRightTicks = 0;

int autoMoveDirection = 1;
bool yTurnRight = false;
bool returnToOriginalHeading = false;

float pidIntegral = 0;
float pidLastError = 0;

WebServer server(80);

void setup() {
  Serial.begin(115200);

  pinMode(L_C1, INPUT_PULLUP);
  pinMode(L_C2, INPUT_PULLUP);
  pinMode(R_C1, INPUT_PULLUP);
  pinMode(R_C2, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(L_C1), leftISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(R_C1), rightISR, CHANGE);

  ledcAttach(L_IN1, PWM_FREQ, PWM_RES);
  ledcAttach(L_IN2, PWM_FREQ, PWM_RES);
  ledcAttach(R_IN1, PWM_FREQ, PWM_RES);
  ledcAttach(R_IN2, PWM_FREQ, PWM_RES);

  setMotor(0, 0);

  WiFi.softAP(ssid, password);

  Serial.println("ESP32 WiFi started");
  Serial.print("WiFi: ");
  Serial.println(ssid);
  Serial.print("Password: ");
  Serial.println(password);
  Serial.print("IP: ");
  Serial.println(WiFi.softAPIP());

  setupWebServer();

  lastCmdTime = millis();
  lastControlTime = millis();
}

void loop() {
  handleWebClient();

  if (millis() - lastControlTime >= CONTROL_INTERVAL_MS) {
    lastControlTime = millis();

    if (autoState != AUTO_IDLE && autoState != AUTO_DONE) {
      updateAutonomous();
    } else if (autoState == AUTO_IDLE) {
      if (millis() - lastCmdTime > CMD_TIMEOUT_MS) {
        fwd = bwd = left = right = false;
      }
      updateManualMotors();
    }
  }
}
