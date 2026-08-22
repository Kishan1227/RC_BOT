# Split ESP32 Robot Project

The original monolithic sketch was split by responsibility:

- `robot.ino` — setup, loop, globals, Wi-Fi initialization
- `config.h` — pin, geometry, PWM, speed, PID constants
- `state.h` — shared robot/autonomous/PID state declarations
- `encoders.cpp/.h` — encoder ISRs, distance, heading
- `motors.cpp/.h` — motor PWM, PID, manual motor control
- `autonomous.cpp/.h` — programmed turns and X/Y autonomous movement
- `web.cpp/.h` — HTTP handlers and embedded control webpage

Keep all files in the same Arduino/PlatformIO project directory. The code intentionally preserves the behavior and constants of the supplied sketch rather than redesigning the control logic.
