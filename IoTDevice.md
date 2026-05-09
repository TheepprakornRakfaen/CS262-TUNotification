# INTRODUCING -- Set up IoT device
การเตรียม Hardware (Edge Device) ใช้บอร์ด ESP32-S3 เชื่อมต่อกับไมโครโฟน INMP441 ผ่านโปรโตคอล I2S โดนมีรูปแบบการต่อดังนี้ <br><br>
<img width="1000" height="572" alt="image" src="https://github.com/user-attachments/assets/488deb58-5903-495e-bd2c-58b37c4c7f29" />
<br><br>
## ขั้นตอนการเชื่อมต่อ IoT core 
  1. ดาวโหลด ArduinoIDE <br><br>
  2. ดาวโหลด Library เสริม **ArduinoJson** และ **PubSubClient** <br><br>
<img width="1000" height="500" alt="image" src="https://github.com/user-attachments/assets/f7de0692-3fef-4205-85c3-88512bfa1e97" /><br><br>
  3. เข้าไปยังหน้า AWS IoT core <br><br>
  4. กด All devices -> Things <br><br>
  5. กด Create Thing เลือก Single Things <br><br>
  6. ตั้งชื่อ Thing_name (นำไปใช้ในโค้ด) -> กด Next <br><br>
  7. เลือก Auto-generate a new certificate (recommended) -> กด Next <br><br>
  8. สร้าง Policy เพื่ออนุญาตให้บอร์ดสามารถเข้าถึงทรัพยากรทุอย่างได้(สะดวกต่อการพัฒนาใน demo) โดยตั้งชื่อ ESP32_Allow_All ใส่โค้ดรูปแบบ json ดังนี้ 
```json
{
  "Version": "2012-10-17",
  "Statement": [
    {
      "Effect": "Allow",
      "Action": "iot:*",
      "Resource": "*"
    }
  ]
}
```
จากนั้นกด **Create** <br><br>
9. กลับมาที่หน้า Attach policies to certificate กดติ๊ก **ESP32_Allow_All** -> กด Create thing <br><br>
10. ดาวโหลด Private key, AmazonRootCA1 และ deviceCertificate <br><br>
11. นำโค้ดต่อไปนี้เขียนลง ArduinoIDE โดยใส่ข้อมูลในช่องว่างให้ถูกต้อง <br>
```Python
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"
#include <driver/i2s.h>
#include <math.h>
#include <time.h>
#include <pgmspace.h>

// -----------------------------------------
// 1. ตั้งค่าเครือข่าย Wi-Fi
// -----------------------------------------
const char* ssid = "";     // <-- ใส่ชื่อ Wi-Fi
const char* password = ""; // <-- ใส่รหัส Wi-Fi

// -----------------------------------------
// 2. ตั้งค่า AWS IoT Core
// -----------------------------------------
const char* AWS_IOT_ENDPOINT = "";  // <-- เอา Endpoint URL มาใส่ตรงนี้ (เช่น xxxxxx-ats.iot.us-east-1.amazonaws.com)
const char* AWS_IOT_TOPIC = "esp32/sound_data"; // ชื่อหัวข้อที่ใช้ส่งข้อมูล


static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
// --- วางใบรับรอง Amazon Root CA 1 ---
-----END CERTIFICATE-----
)EOF";


static const char AWS_CERT_CRT[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
// --- วาง Device Certificate ---
-----END CERTIFICATE-----
)EOF";


static const char AWS_CERT_PRIVATE[] PROGMEM = R"EOF(
-----BEGIN RSA PRIVATE KEY-----
// --- วาง Private Key ---
-----END RSA PRIVATE KEY-----
)EOF";

// -----------------------------------------
// 3. ตั้งค่าเซนเซอร์ I2S (INMP441)
// -----------------------------------------
#define I2S_WS 4
#define I2S_SCK 5
#define I2S_SD 6
#define I2S_PORT I2S_NUM_0
const double CALIBRATION_OFFSET = -65.0;

WiFiClientSecure net = WiFiClientSecure();
PubSubClient client(net);

void connectAWS() {
  Serial.print("Connecting to AWS IOT");
  

  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  client.setServer(AWS_IOT_ENDPOINT, 8883); 

  while (!client.connect("")) {  // <-- client.connect ใส่ชื่อ Things ที่สร้างเอาไว้
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nAWS IoT Connected!");
}

void setup() {
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi Connected!");

  configTime(7 * 3600, 0, "pool.ntp.org");
  Serial.print("Syncing time");
  struct tm timeinfo;
  while (!getLocalTime(&timeinfo)) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nTime Synced!");

  connectAWS();

  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = 44100,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 8,
    .dma_buf_len = 64,
    .use_apll = false
  };
  i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
    .data_out_num = -1,  
    .data_in_num = I2S_SD
  };
  i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_PORT, &pin_config);
}

unsigned long lastPublish = 0;

void loop() {
  client.loop(); 
  
  if (!client.connected()) {
    connectAWS();
  }

  int32_t samples[64];
  size_t bytes_read;
  
  i2s_read(I2S_PORT, &samples, sizeof(samples), &bytes_read, portMAX_DELAY);
  int samples_read = bytes_read / sizeof(int32_t);
  
  if (samples_read > 0) {
    double sum = 0;
    for (int i = 0; i < samples_read; i++) {
      int32_t sample = samples[i] >> 8; 
      sum += (double)sample * sample;
    }
    double rms = sqrt(sum / samples_read);
    
    if (rms > 0 && millis() - lastPublish > 2000) {
      lastPublish = millis();
      double db_spl = 20.0 * log10(rms) + CALIBRATION_OFFSET; 
      
      struct tm timeinfo;
      getLocalTime(&timeinfo);
      char timeString[30];
      strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", &timeinfo);

      StaticJsonDocument<256> doc;
      doc["deviceId"] = ""; // <-- client.connect ใส่รหัสอุปกรณ์
      doc["timestamp"] = timeString;
      doc["decibel"] = db_spl;
      doc["latitude"] = 14.0724;   
      doc["longitude"] = 100.6063; 
      
      char jsonBuffer[256];
      serializeJson(doc, jsonBuffer);
      
      client.publish(AWS_IOT_TOPIC, jsonBuffer);
      
      Serial.print("Published to AWS: ");
      Serial.println(jsonBuffer);
    }
  }
}
```
<br><br>
10. กด **Upload** <br><br>
### ทดสอบการทำงานของ IoT
1. เลือกอุปกรณ์ที่ต้องการ <br><br>
<img width="1907" height="896" alt="image" src="https://github.com/user-attachments/assets/b7612bdb-76e4-4920-9c6c-c264735173ef" /><br>
2. เลือก **Activity** -> **MQTT test client** <br><br>
<img width="1917" height="852" alt="สกรีนช็อต 2026-05-09 232750" src="https://github.com/user-attachments/assets/4f40c775-6ae0-4d18-990b-d163247b32c4" /><br>
3. เลือก **Topic filters** -> พิมพ์ **esp32/sound_data** -> กด **Subscribe** <br><br>
<img width="1917" height="852" alt="สกรีนช็อต 2026-05-09 232750" src="https://github.com/user-attachments/assets/bc5e7b5a-a0dd-4081-a651-86a6e6b3c20d" /><br>


