# Smart-Door-Security-System
# 🔒 Arduino Smart Door Lock System (Keypad + RFID + Safety)

An enhanced security system for doors built on Arduino, featuring dual authentication (Keypad and RFID) and an integrated obstacle safety check using an IR sensor before auto-locking.

## ✨ Features

* **Dual Authentication:** Access granted via a $\mathbf{4x4}$ Keypad password or an authorized RFID tag.
* **Servo Control:** Uses a Servo Motor (SG90/MG996R) to physically lock and unlock the door.
* **Auto-Lock Function:** Automatically relocks the door after **5 seconds** of inactivity (adjustable).
* **IR Safety Check:** Prevents the auto-lock if an obstacle is detected in the doorway (via IR sensor), ensuring safety.
* **User Feedback:** Provides system status, access attempts, and alerts on a **16x2 I2C LCD** and a **Buzzer**.
* **Code Hiding:** Keypad input is displayed as $\mathbf{*}$ on the LCD for security.

## 🛠️ Hardware Components

This project requires the following components:

| Component | Quantity | Purpose |
| :--- | :--- | :--- |
| **Microcontroller** | 1 | Arduino Uno / Nano |
| **Keypad** | 1 | 4x4 Matrix Keypad |
| **RFID Module** | 1 | MFRC522 RFID Reader |
| **Display** | 1 | 16x2 LCD with I2C module (Address $\mathbf{0x27}$ or $\mathbf{0x3F}$) |
| **Actuator** | 1 | Servo Motor (SG90 or similar) |
| **Sensor** | 1 | IR Obstacle Sensor |
| **Alert** | 1 | Active or Passive Buzzer |
| **Other** | - | Jumper Wires, Breadboard (optional), Power Supply |

## 📌 Pin Configuration

| Component | Pin (Signal) | Arduino Pin |
| :--- | :--- | :--- |
| **Keypad (Rows)** | R1, R2, R3, R4 | D2, D3, D4, D5 |
| **Keypad (Cols)** | C1, C2, C3, C4 | D6, D7, D8, A0 |
| **RFID (RST)** | RST | A1 |
| **RFID (SS/SDA)** | SS | D10 |
| **RFID (SCK)** | SCK | D13 (SPI) |
| **RFID (MOSI)** | MOSI | D11 (SPI) |
| **RFID (MISO)** | MISO | D12 (SPI) |
| **Buzzer** | Signal | A2 |
| **IR Sensor** | Signal | A3 |
| **Servo Motor** | Signal | D9 |
| **LCD (I2C)** | SDA, SCL | A4 (SDA), A5 (SCL) |

## ⚙️ Software Setup

1.  **Install Arduino IDE.**
2.  **Install Libraries:** Open the Arduino Library Manager and install the following libraries:
    * `Keypad` by Mark Stanley, Alexander Brevig
    * `MFRC522` by Ujjwal Kumar
    * `LiquidCrystal_I2C` by Frank de Brabander (Make sure your I2C address is correctly set to `0x27` or adjust it in the code).
    * `Servo` (Usually pre-installed)

## 🔑 Default Access Credentials

For testing and initial setup, the following credentials are used:

| Method | Credential | Variable Name (Code) |
| :--- | :--- | :--- |
| **Keypad** | `1234` + `#` | `MASTER_PASS` |
| **RFID Card 1** | `A3 22 FF A2` | `AUTHORIZED_UID_1` |
| **RFID Card 2** | `52 08 1A 4D` | `AUTHORIZED_UID_2` |

> **Note:** Remember to replace the `AUTHORIZED_UID` variables in the code with the actual UIDs of your authorized cards.

## 💡 How to Use

1.  **Wiring:** Connect all components according to the Pin Configuration table above.
2.  **Upload:** Upload the provided `.ino` code to your Arduino board.
3.  **Operation:**
    * The LCD will display **"SYSTEM READY"**.
    * Enter the password (`1234`) on the keypad followed by the submit key (`#`), **OR** scan an authorized RFID card.
    * Upon successful access, the Servo motor will move to `90` degrees (Unlock), and a confirmation message will display.
    * After the `AUTOLOCK_DELAY` (5000 ms), the system checks the IR sensor. If **NO OBSTACLE** is present, the door automatically locks (`0` degrees).
    * If the IR sensor detects an obstacle during the auto-lock period, an alarm sounds, and the door remains unlocked until the path is clear.

## 📄 License

MIT License

Copyright (c) 2025 [Niloy Barua]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
