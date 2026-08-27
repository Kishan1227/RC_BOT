# ESP32 Robot X/Y/Z Shortest-Path Control

This project controls a two-wheel ESP32 robot from a web page.

## X/Y/Z movement

The robot now treats X and Y as a target point measured from the position where the command starts. Z is the final heading.

Example:

```text
X = 100 mm
Y = 150 mm
Z = 60°
```

The robot calculates the shortest straight-line path using trigonometry:

```text
Distance = sqrt(X² + Y²)
Angle    = atan2(Y, X)
```

It then:

1. Saves the starting heading.
2. Calculates the angle to the target with `atan2(Y, X)`.
3. Calculates the shortest distance with the Pythagorean theorem.
4. Turns through the shortest angular direction to face the target.
5. Drives straight along the hypotenuse to the target.
6. Uses encoder feedback and PID correction to stay straight.
7. After reaching the target, turns to Z.
8. Stops.

Z is relative to the heading at the beginning of the command.

Examples when the starting heading is 0°:

- X=100, Y=0, Z=0 → drive 100 mm and finish at 0°
- X=0, Y=100, Z=0 → turn 90° right, drive 100 mm, turn back to 0°
- X=100, Y=100, Z=90 → turn 45° right, drive about 141.4 mm, then finish at 90°
- X=-100, Y=0, Z=0 → turn 180°, drive 100 mm, then finish at 0°

`atan2()` is used instead of `atan(Y/X)` because it correctly handles all quadrants and the case where X is zero.

## Project files

- `robot.ino` — main setup and loop
- `config.h` — robot configuration
- `state.h` — shared robot state
- `encoders.cpp/.h` — encoder and heading calculations
- `motors.cpp/.h` — motor and PID control
- `autonomous.cpp/.h` — shortest-path X/Y movement and final Z turn
- `web.cpp/.h` — web interface and HTTP handlers

Keep all files in the same Arduino/PlatformIO project folder.
