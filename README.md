# 🚗 Arduino Autonomous Car Project

A multi-mode autonomous car built with Arduino, controlled via Bluetooth using the **Dabble** mobile app. The car supports three operational modes: **Line Tracking**, **Obstacle Avoidance**, and **Gyroscope (Phone Tilt) Control**.

> 📚 Built as a final year Computer Engineering school project by [21-Twentyone](https://github.com/21-Twentyone)

---

## 📋 Table of Contents

- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Pin Configuration](#pin-configuration)
- [Modes of Operation](#modes-of-operation)
- [How to Use](#how-to-use)
- [Dependencies](#dependencies)
- [Project Structure](#project-structure)

---

## ✨ Features

- 🔵 **Bluetooth control** via the Dabble mobile app (HC-05 module)
- 📏 **Ultrasonic obstacle detection** with auto-avoidance
- ➡️ **IR-based line following** with last-position memory
- 📱 **Phone tilt (gyroscope) driving** using the phone's accelerometer
- 🔄 **Hot-swap modes** from the Dabble Terminal — no restart needed

---

## 🛠️ Hardware Requirements

| Component | Quantity |
|---|---|
| Arduino Uno / Nano | 1 |
| HC-05 Bluetooth Module | 1 |
| L298N Motor Driver | 1 |
| DC Gear Motors | 2 |
| HC-SR04 Ultrasonic Sensor | 1 |
| IR Obstacle Sensors | 2 |
| Line Tracking Sensors | 2 |
| 9V or 12V Battery Pack | 1 |
| Chassis + Wheels | 1 set |

---

## 📌 Pin Configuration

| Pin | Component |
|---|---|
| 6 | Motor A – Forward (a1) |
| 5 | Motor A – Backward (a2) |
| 10 | Motor B – Forward (b1A) |
| 9 | Motor B – Backward (b2A) |
| 4 | Line Tracking Sensor – Left |
| 13 | Line Tracking Sensor – Right |
| 8 | IR Sensor – Left |
| 7 | IR Sensor – Right |
| A0 | Ultrasonic Trigger |
| A1 | Ultrasonic Echo |
| 12 (RX) / 11 (TX) | HC-05 Bluetooth (Dabble) |

---

## 🎮 Modes of Operation

Switch modes anytime by sending a command through the **Dabble Terminal module**:

| Command | Mode | Description |
|---|---|---|
| `1` | Line Tracking | Follows a black line on a white surface using IR sensors |
| `2` | Obstacle Avoidance | Detects and navigates around objects using ultrasonic + IR |
| `3` | Gyro Control | Drive by tilting your phone (uses phone accelerometer) |
| `0` | Standby | Stops all movement |

### Mode Details

**🔲 Line Tracking**
Uses two line sensors to follow a track. If both sensors lose the line, the car uses its last known turn direction to recover.

**🚧 Obstacle Avoidance**
The ultrasonic sensor detects objects closer than 15 cm. The car reverses and steers away using IR sensors to choose direction.

**📱 Gyro Control**
Tilt your phone **forward/backward** to drive, and **left/right** to steer. Laying the phone flat stops the car.

---

## 🚀 How to Use

1. **Wire up** the components according to the pin configuration above.
2. **Install the Dabble library** in the Arduino IDE (see Dependencies).
3. **Upload** `arduino_car_project_code.ino` to your Arduino board.
4. **Pair** your phone with the HC-05 Bluetooth module (default PIN: `1234` or `0000`).
5. **Open the Dabble app**, connect to the HC-05, and open the **Terminal module**.
6. **Send `1`, `2`, or `3`** to activate a mode. Send `0` to stop.

---

## 📦 Dependencies

- [Dabble Library for Arduino](https://github.com/STEMpedia/Dabble) — install via Arduino IDE Library Manager
  - Search: `Dabble` by STEMpedia

---

## 📁 Project Structure

```
Arduino_autonomous-car-project/
│
└── arduino_car_project_code.ino   # Main Arduino sketch
```

---

## 👥 Team

Built by **21-Twentyone** — Information and Telecommunication Student.

---

## 📄 License

This project is open-source and available for educational use.
