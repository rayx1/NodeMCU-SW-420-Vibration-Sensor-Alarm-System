# NodeMCU-SW-420-Vibration-Sensor-Alarm-System
📝 Description
This project creates a smart vibration detection system using:

NodeMCU ESP8266

SW-420 Vibration Sensor Module

Buzzer for local alarm

SMTP email notifications

When vibrations are detected, the system triggers both a local audible alarm and sends email alerts to specified recipients.

📦 Hardware Components
Component	Quantity
NodeMCU ESP8266	1
SW-420 Vibration Sensor Module	1
Active Buzzer	1
LEDs (Status and Alarm)	2
220Ω Resistors	2
Breadboard	1
Jumper Wires	Several
🔌 Circuit Diagram
SW-420 Module      NodeMCU
   VCC ----------- 3.3V or 5V
   GND ----------- GND
   DO (Digital Out) -- D5

Buzzer ----------- D1
Status LED ------- D2 (with 220Ω resistor)
Alarm LED -------- D6 (with 220Ω resistor)
⚙️ Features
Real-time vibration detection

Adjustable sensitivity via SW-420 potentiometer

Local audible and visual alarms

Email notifications via SMTP

Built-in debounce to prevent false triggers

Email rate limiting to prevent spam

WiFi status indicator LED

🛠️ Installation
Hardware Setup:

Connect components as shown in the circuit diagram

Adjust SW-420 sensitivity using the blue potentiometer

Software Setup:

Install required libraries:

bash
ESP8266WiFi
ESP_Mail_Client
Clone this repository:

Download the .ino file 

Open the project in Arduino IDE

Configuration:

Update WIFI_SSID and WIFI_PASSWORD in the code

Configure SMTP settings:

cpp
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465
#define AUTHOR_EMAIL "your_email@gmail.com"
#define AUTHOR_PASSWORD "your_app_password"
#define RECIPIENT_EMAIL "recipient@example.com"
🚀 Usage
Upload the sketch to your NodeMCU

Open Serial Monitor (115200 baud) to monitor sensor status

Test by gently tapping the vibration sensor

Adjust sensitivity as needed using the potentiometer

📧 Email Configuration Notes
For Gmail users:

Enable "Less secure app access" OR

Create an App Password:

Go to Google Account → Security → App Passwords

Generate a password for "Mail" on "ESP8266"

Use this password in AUTHOR_PASSWORD

📊 Troubleshooting
Issue	Solution
Constant false alarms	Adjust sensitivity potentiometer counter-clockwise
No detection	Check wiring, try powering sensor with 5V
Email not sending	Verify SMTP settings, check WiFi connection
Sensor always HIGH/LOW	Check for short circuits, test with multimeter
📜 License
MIT License - See LICENSE file for details

🤝 Contributing
Contributions are welcome! Please open an issue or submit a pull request.
