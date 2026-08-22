#include <Arduino.h>
#include "config.h"
#include "state.h"
#include "encoders.h"
#include "motors.h"
#include "autonomous.h"

void startTurn(float degrees) {
  targetTurnAngle = degrees;
  autoStartHeading = getHeadingDeg();
  autoTargetHeading = autoStartHeading + targetTurnAngle;
  autoState = AUTO_TURN;
  resetPID();
}

void startXY(float x, float y) {
  requestedX = x;
  requestedY = y;

  remainingX = fabs(x);
  remainingY = fabs(y);

  if (remainingX < 0.5f && remainingY < 0.5f) {
    autoState = AUTO_DONE;
    setMotor(0, 0);
    return;
  }

  autoStartHeading = getHeadingDeg();

  if (remainingX >= 0.5f) {
    autoMoveDirection = (x >= 0) ? 1 : -1;
    autoStartLeftTicks = leftTicks;
    autoStartRightTicks = rightTicks;
    resetPID();
    autoState = AUTO_MOVE_X;
    return;
  }

  if (remainingY >= 0.5f) {
    yTurnRight = (y > 0);
    targetTurnAngle = yTurnRight ? 90.0f : -90.0f;
    autoTargetHeading = autoStartHeading + targetTurnAngle;
    autoState = AUTO_TURN_Y;
    resetPID();
  }
}

void updateMoveX() {
  float leftDistance =
    ((float)(leftTicks - autoStartLeftTicks) / LEFT_TICKS_PER_REV) *
    WHEEL_CIRCUMFERENCE_MM;

  float rightDistance =
    ((float)(rightTicks - autoStartRightTicks) / RIGHT_TICKS_PER_REV) *
    WHEEL_CIRCUMFERENCE_MM;

  float travelled = (fabs(leftDistance) + fabs(rightDistance)) / 2.0f;

  if (travelled >= remainingX) {
    setMotor(0, 0);
    resetPID();

    if (remainingY >= 0.5f) {
      yTurnRight = (requestedY > 0);
      targetTurnAngle = yTurnRight ? 90.0f : -90.0f;
      autoStartHeading = getHeadingDeg();
      autoTargetHeading = autoStartHeading + targetTurnAngle;
      autoState = AUTO_TURN_Y;
    } else {
      autoState = AUTO_DONE;
    }
    return;
  }

  int speed = BASE_SPEED;
  float remaining = remainingX - travelled;

  if (remaining < 100) speed = 120;
  if (remaining < 50) speed = 90;
  if (remaining < 20) speed = 65;

  int speedL = speed * autoMoveDirection;
  int speedR = speed * autoMoveDirection;

  applyStraightPID(speedL, speedR);
  setMotor(speedL, speedR);
}

void updateTurnY() {
  float currentHeading = getHeadingDeg();
  float error = autoTargetHeading - currentHeading;

  while (error > 180) error -= 360;
  while (error < -180) error += 360;

  if (fabs(error) <= 1.0f) {
    setMotor(0, 0);
    delay(100);

    autoStartLeftTicks = leftTicks;
    autoStartRightTicks = rightTicks;
    resetPID();

    autoMoveDirection = 1;
    autoState = AUTO_MOVE_Y;
    return;
  }

  int turnSpeed = 120;
  if (fabs(error) < 15) turnSpeed = 80;

  if (error > 0)
    setMotor(-turnSpeed, turnSpeed);
  else
    setMotor(turnSpeed, -turnSpeed);
}

void updateMoveY() {
  float leftDistance =
    ((float)(leftTicks - autoStartLeftTicks) / LEFT_TICKS_PER_REV) *
    WHEEL_CIRCUMFERENCE_MM;

  float rightDistance =
    ((float)(rightTicks - autoStartRightTicks) / RIGHT_TICKS_PER_REV) *
    WHEEL_CIRCUMFERENCE_MM;

  float travelled = (fabs(leftDistance) + fabs(rightDistance)) / 2.0f;

  if (travelled >= remainingY) {
    setMotor(0, 0);
    resetPID();
    autoState = AUTO_DONE;
    return;
  }

  float remaining = remainingY - travelled;
  int speed = BASE_SPEED;

  if (remaining < 100) speed = 120;
  if (remaining < 50) speed = 90;
  if (remaining < 20) speed = 65;

  int speedL = speed;
  int speedR = speed;

  applyStraightPID(speedL, speedR);
  setMotor(speedL, speedR);
}

void updateAutonomous() {
  if (autoState == AUTO_TURN) {
    float currentHeading = getHeadingDeg();
    float error = autoTargetHeading - currentHeading;

    while (error > 180) error -= 360;
    while (error < -180) error += 360;

    if (fabs(error) <= 1.0f) {
      setMotor(0, 0);
      delay(100);
      autoState = AUTO_DONE;
      resetPID();
      return;
    }

    int turnSpeed = (fabs(error) < 15) ? 80 : 120;

    if (error > 0)
      setMotor(-turnSpeed, turnSpeed);
    else
      setMotor(turnSpeed, -turnSpeed);

  } else if (autoState == AUTO_MOVE_X) {
    updateMoveX();
  } else if (autoState == AUTO_TURN_Y) {
    updateTurnY();
  } else if (autoState == AUTO_MOVE_Y) {
    updateMoveY();
  } else if (autoState == AUTO_DONE) {
    setMotor(0, 0);
  }
}
