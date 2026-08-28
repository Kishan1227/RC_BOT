#pragma once
#include <Arduino.h>

extern const char* ssid;
extern const char* password;

#define L_C1 16
#define L_C2 17
#define R_C1 18
#define R_C2 19

#define L_IN1 32
#define L_IN2 33
#define R_IN1 25
#define R_IN2 26

#define PWM_FREQ 5000
#define PWM_RES 8

#define WHEEL_CIRCUMFERENCE_MM 100.53f   // circumference of the actual wheel
#define WHEEL_BASE_MM 130.50f            // changed according to requirment
#define LEFT_TICKS_PER_REV 2020.0f        //manually check for individual motors
#define RIGHT_TICKS_PER_REV 2000.0f

#define BASE_SPEED 200
#define TURN_OFFSET 50
#define PIVOT_SPEED 190

#define CONTROL_INTERVAL_MS 50
#define CMD_TIMEOUT_MS 1500

extern float Kp;
extern float Ki;
extern float Kd;
extern float RIGHT_MOTOR_SCALE;
