# RC_ROBO 

ESP32 Robot Control Project

This project is an ESP32-based two-wheel robot controlled through a web page.
The robot can be controlled manually using buttons or can perform programmed movements such as:
Forward and backward movement
• Left and right turns
• Turning by a specific angle
• Moving a specific distance in X/Y directions
• Encoder-based distance measurement
• Encoder-based heading measurement
• Straight-line PID correction
• Real-time robot status through a web page
The ESP32 creates its own Wi-Fi network, so the robot can be controlled directly from a phone or computer without needing an external Wi-Fi router. The original project uses the SSID ROBOT_ESP32.

The original monolithic sketch was split by responsibility:

- `robot.ino` — setup, loop, globals, Wi-Fi initialization
- `config.h` — pin, geometry, PWM, speed, PID constants
- `state.h` — shared robot/autonomous/PID state declarations
- `encoders.cpp/.h` — encoder ISRs, distance, heading
- `motors.cpp/.h` — motor PWM, PID, manual motor control
- `autonomous.cpp/.h` — programmed turns and X/Y autonomous movement
- `web.cpp/.h` — HTTP handlers and embedded control webpage

Keep all files in the same Arduino/PlatformIO project directory. The code intentionally preserves the behavior and constants of the supplied sketch rather than redesigning the control logic.

How to Use the Robot
1. Turn on the ESP32
The ESP32 creates its own Wi-Fi network.
Connect your phone or computer to:
Wi-Fi: ROBOT_ESP32
Password: roboy1234
The ESP32 then prints its IP address through the Serial Monitor. 
Pasted code.cpp
2. Open the Robot Control Page
Enter the ESP32's IP address in your browser.
The robot control page will appear.
