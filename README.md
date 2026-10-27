# 🚀 ระบบควบคุมปั้มน้ำอัตโนมัติ (Project Name)
เป็นการควบคุมการเปิดปิดปั้มน้ำอัตโนมัติด้วยรีเลย์และเซนเซอร์วัดความซื้นในดินโดยมีหน้าจอ OLED แสดงผล
## 📸 ตัวอย่างหน้าตาโปรเจกต์ (Screenshots / GIF)
<img width="1186" height="665" alt="Screenshot 2026-10-26 225218" src="https://github.com/user-attachments/assets/4d0ec5d1-ff6c-4dfa-9e50-0aad3c664828" />
## ✨ ฟีเจอร์หลัก (Key Features)

เวลาที่ค่าความชื้นในดินมีน้อยระบบจะเปิดปั้มน้ำอัตโนมัติ

โดยในภาพผมได้ทำการตั้งค่าไว้ที่ความชื้นต่ำกว่า 800 น้ำจะรดเองอัตโนมัติ

## 🛠 Tech Stack & Hardware

* **Microcontroller / Board:** ESP8266
* **Language:** C / C++ 
* **IDE / Tools:** Arduino IDE
* **Sensors & Components:** 
  * Soil Sensor (วัดความชื้นในดิน)
  * OLED Display (หน้าจอแสดงผล)
  * Relay Module (โมดูลรีเลย์ควบคุมอุปกรณ์ไฟฟ้า)
* **Cloud & Protocol:** ThingSpeak (สำหรับส่งข้อมูลขึ้นคลาวด์),Wi-Fi
