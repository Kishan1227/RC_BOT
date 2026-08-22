#pragma once
#include <Arduino.h>

enum AutoState {
  AUTO_IDLE,
  AUTO_TURN,
  AUTO_MOVE_X,
  AUTO_TURN_Y,
  AUTO_MOVE_Y,
  AUTO_RETURN_HEADING,
  AUTO_DONE
};

extern volatile long leftTicks;
extern volatile long rightTicks;

extern long prevLeftTicks;
extern long prevRightTicks;

extern bool fwd;
extern bool bwd;
extern bool left;
extern bool right;

extern unsigned long lastCmdTime;
extern unsigned long lastControlTime;

extern AutoState autoState;

extern float requestedX;
extern float requestedY;
extern float remainingX;
extern float remainingY;
extern float targetTurnAngle;
extern float autoStartHeading;
extern float autoTargetHeading;

extern long autoStartLeftTicks;
extern long autoStartRightTicks;

extern int autoMoveDirection;
extern bool yTurnRight;
extern bool returnToOriginalHeading;

extern float pidIntegral;
extern float pidLastError;
