#include <ESP8266WiFi.h>
#include <ESP_Mail_Client.h>

// WiFi credentials
#define WIFI_SSID "WIFI"
#define WIFI_PASSWORD "12345678"

// SMTP settings
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465
#define AUTHOR_EMAIL "your_email@gmail.com"
#define AUTHOR_PASSWORD "your_app_password"
#define RECIPIENT_EMAIL "recipient@example.com"

// Pin definitions
const int vibrationSensor = D5;  // SW-420 DO pin
const int buzzerPin = D1;
const int statusLed = D2;
const int alarmLed = D6;

// Detection parameters
unsigned long lastVibrationTime = 0;
const unsigned long debounceTime = 500;  // 500ms minimum between detections
const unsigned long emailCooldown = 30000; // 30s between emails
unsigned long lastEmailTime = 0;

// SMTP client
SMTPSession smtp;

void setup() {
  Serial.begin(9600);
  
  // Configure pins
  pinMode(vibrationSensor, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(statusLed, OUTPUT);
  pinMode(alarmLed, OUTPUT);
  
  // Initial state
  digitalWrite(buzzerPin, HIGH);
  digitalWrite(alarmLed, LOW);
  digitalWrite(statusLed, LOW);

  // Connect to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    digitalWrite(statusLed, !digitalRead(statusLed));
    delay(200);
  }
  digitalWrite(statusLed, HIGH);
  Serial.println("\nWiFi connected");
  Serial.println("IP address: " + WiFi.localIP().toString());

  // Print initial sensor state - FIXED VERSION
  Serial.print("Initial sensor state: ");
  Serial.println(digitalRead(vibrationSensor) ? "HIGH (active)" : "LOW (inactive)");
}

void loop() {
  // Current sensor reading
  int sensorState = digitalRead(vibrationSensor);
  
  // Debug output every 2 seconds
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 2000) {
    Serial.print("Sensor state: ");
    Serial.println(sensorState);
    lastPrint = millis();
  }

  // Detect vibration (SW-420 goes HIGH when vibrating)
  if (sensorState == HIGH && millis() - lastVibrationTime > debounceTime) {
    lastVibrationTime = millis();
    Serial.println("Vibration detected!");
    
    triggerAlarm();
    
    // Only send email if cooldown period has passed
    if (millis() - lastEmailTime > emailCooldown) {
      sendEmailAlert();
      lastEmailTime = millis();
    }
  }
}

void triggerAlarm() {
  // Visual and audible alert
  for (int i = 0; i < 5; i++) {
    digitalWrite(alarmLed, HIGH);
    digitalWrite(buzzerPin, LOW);
    delay(100);
    digitalWrite(alarmLed, LOW);
    digitalWrite(buzzerPin, HIGH);
    delay(100);
  }
}

void sendEmailAlert() {
  // Configure SMTP session
  ESP_Mail_Session session;
  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;
  session.login.user_domain = "";

  // Create message
  SMTP_Message message;
  message.sender.name = "Vibration Alarm";
  message.sender.email = AUTHOR_EMAIL;
  message.subject = "Vibration Detected!";
  message.addRecipient("Recipient", RECIPIENT_EMAIL);
  
  // Email content
  String htmlMsg = R"(
  <div style="font-family:Arial,sans-serif;max-width:600px">
    <h2 style="color:#d32f2f">Vibration Alert</h2>
    <p>Your SW-420 vibration sensor detected movement.</p>
    <div style="background:#f5f5f5;padding:10px;border-left:4px solid #d32f2f">
      <p><strong>Event Details:</strong></p>
      <ul>
        <li>Time: )" + getTimeString() + R"(</li>
        <li>Device IP: )" + WiFi.localIP().toString() + R"(</li>
        <li>Sensor: SW-420 Vibration Module</li>
      </ul>
    </div>
    <p>Please investigate if this was unexpected.</p>
  </div>
  )";

  message.html.content = htmlMsg.c_str();
  message.text.charSet = "us-ascii";
  message.html.transfer_encoding = Content_Transfer_Encoding::enc_7bit;

  // Connect and send
  if (!smtp.connect(&session)) {
    Serial.println("SMTP connection failed: " + smtp.errorReason());
    return;
  }

  if (!MailClient.sendMail(&smtp, &message)) {
    Serial.println("Error sending email: " + smtp.errorReason());
  } else {
    Serial.println("Email alert sent successfully");
  }
}

String getTimeString() {
  unsigned long totalSeconds = millis() / 1000;
  unsigned long hours = totalSeconds / 3600;
  unsigned long minutes = (totalSeconds % 3600) / 60;
  unsigned long seconds = totalSeconds % 60;
  
  char buf[20];
  snprintf(buf, sizeof(buf), "%02lu:%02lu:%02lu", hours, minutes, seconds);
  return String(buf);
}