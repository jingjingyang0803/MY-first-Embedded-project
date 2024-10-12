# Lego Robot Car Project

## Project Overview
The Lego Robot Car Project was a team effort to build and program a remote-controlled car using IoT technologies. The project was carried out by a team of three and involved using **C++**, **MQTT protocol**, **REST-API**, **Arduino**, **ESP8266**, and various **sensors**. The main goal of the project was to develop a robot car capable of adjusting its direction, speed, and distance traveled while integrating sensors for autonomous obstacle detection and response.

## Key Features
- **Remote Control**: The car can be controlled in real-time through a web-based interface or a mobile device, adjusting speed and direction as needed.
- **Autonomous Obstacle Detection**: Equipped with distance detection sensors, the car identifies and reacts to nearby objects. If the object is too close, the car will stop to avoid collisions and ensure safe operation. The distance threshold can be configured based on specific requirements for the environment.
- **Real-time Communication**: Using the MQTT protocol, the car sends and receives control commands instantly, allowing smooth and responsive remote operation.
- **ESP8266 Wi-Fi Module**: The ESP8266 module provides wireless communication between the car and the control system.
- **REST-API Integration**: For additional functionality and data exchange, the car communicates with external systems through a REST API.

## Technologies Used
- **C++**: Core programming language for controlling the car's hardware and sensors.
- **MQTT Protocol**: Lightweight messaging protocol for IoT communication, used to transmit control commands.
- **REST API**: Enables communication between the robot car and external systems for additional functionality.
- **Arduino**: Microcontroller used to control the car’s movements and sensor inputs.
- **ESP8266**: Wi-Fi module that connects the car to the internet for remote control.
- **Sensors**: Distance detection sensors are used for obstacle detection and avoidance.

## How to Use

1. Power on the Lego robot car and ensure it's connected to Wi-Fi through the **ESP8266** module.
2. Use the web-based interface or mobile app to control the car's movements in real-time.
3. The car will autonomously detect nearby objects using its distance sensors and adjust its speed or direction as needed.
4. Monitor the status and receive real-time updates through the MQTT protocol.

## Acknowledgments
Thanks to **Tampere University of Applied Sciences (TAMK)** for providing the opportunity to work on this project.

Special thanks to the **open-source community** for the libraries and tools that supported this project.

