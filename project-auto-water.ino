#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int soilSensorPin = A0;
const int relayPin = D5;
const int dryThreshold = 880;
// --- 1. ตั้งค่า Wi-Fi (รองรับเฉพาะ 2.4GHz) ---
const char* ssid = "iPhone (2)";         // ชื่อ Wi-Fi
const char* password = "POON7777"; // รหัสผ่าน Wi-Fi

// --- 2. ตั้งค่า ThingSpeak ---
WiFiClient client;
unsigned long myChannelNumber = 3540876;           // Channel ID (ใส่ตัวเลขล้วน ห้ามใส่ "")
const char * myWriteAPIKey = "WQ6Z81SL9HG3164T"; // Write API Key (ข้อความใน "")

// ตั้งเวลาส่งข้อมูลทุก 15 วินาที (เกณฑ์ขั้นต่ำของ ThingSpeak บัญชีฟรี)
unsigned long lastTime = 0;
unsigned long timerDelay = 15000;

void setup() {
  Serial.begin(9600);

 // เริ่มต้นเชื่อมต่อ Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nWiFi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // เริ่มต้นใช้งาน ThingSpeak
  ThingSpeak.begin(client);

  // ตั้งขา D5 เป็น INPUT เพื่อตัดสัญญาณปิดรีเลย์ตั้งแต่เริ่มต้น
  pinMode(relayPin, INPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED Allocation Failed"));
    for (;;);
  }

  display.clearDisplay();
  display.setTextColor(WHITE);
}

void loop() {
  int soilValue = analogRead(soilSensorPin);
  bool isPumpOn = false;

  if (soilValue > dryThreshold) {
    // ดินแห้ง -> สั่งเปิดปั๊ม (เปลี่ยนเป็น OUTPUT และสั่ง LOW)
    pinMode(relayPin, OUTPUT);
    digitalWrite(relayPin, LOW); 
    isPumpOn = true;
  } else {
    // ดินชื้น -> สั่งปิดปั๊มแบบเด็ดขาด (เปลี่ยนเป็น INPUT ตัดสัญญาณ)
    pinMode(relayPin, INPUT);
    isPumpOn = false;
  }

  // --- แสดงผลบนหน้าจอ OLED ---
  display.clearDisplay();
  
  display.setTextSize(1);
  display.setCursor(10, 0);
  display.println("AUTO WATERING");
  display.drawLine(0, 10, 128, 10, WHITE);

  display.setCursor(0, 20);
  display.print("Soil Value: ");
  display.print(soilValue);

  display.setTextSize(2);
  display.setCursor(0, 44);
  if (isPumpOn) {
    display.println("PUMP: ON");
  } else {
    display.println("PUMP: OFF");
  }

  // ตรวจสอบรอบเวลาทุกๆ 15 วินาที
  if ((millis() - lastTime) > timerDelay) {
    
    if (WiFi.status() == WL_CONNECTED) {
      
      // ตัวอย่างการอ่านค่าเซนเซอร์อนาลอกจากขา A0
      int sensorValue = analogRead(A0);

      // กำหนดค่าลง Field 1 (หากมีหลาย Field ให้ใช้ ThingSpeak.setField(2, value) เพิ่มเติม)
      ThingSpeak.setField(1, sensorValue);

      // สั่งส่งข้อมูลขึ้น ThingSpeak Cloud
      int httpCode = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

      // ตรวจสอบสถานะการส่งข้อมูลบน Serial Monitor
      if (httpCode == 200) {
        Serial.println("ThingSpeak update successful! (HTTP 200)");
      } else {
        Serial.print("ThingSpeak error. HTTP Code: ");
        Serial.println(httpCode);
      }

    } else {
      Serial.println("WiFi Disconnected!");
    }

    lastTime = millis(); // บันทึกเวลาล่าสุด
  }

  display.display();

  delay(1000);
}
