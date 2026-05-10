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
const char* ssid = "";         // <-- ใส่ชื่อ Wi-Fi
const char* password = ""; // <-- ใส่รหัส Wi-Fi

// -----------------------------------------
// 2. ตั้งค่า AWS IoT Core
// -----------------------------------------

const char* AWS_IOT_ENDPOINT = ""; // <-- เอา Endpoint URL มาใส่ตรงนี้ (เช่น xxxxxx-ats.iot.us-east-1.amazonaws.com)
const char* AWS_IOT_TOPIC = "esp32/sound_data"; // ชื่อหัวข้อที่ใช้ส่งข้อมูล


static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
// --- วางใบรับรอง Amazon Root CA 1 ---
-----END CERTIFICATE-----
)EOF";

// --- วาง Device Certificate ---
static const char AWS_CERT_CRT[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
// --- วาง Device Certificate ---
-----END CERTIFICATE-----
)EOF";

// --- วาง Private Key ---
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

  while (!client.connect("")) { //<--- ใส่ thing name ใน iot core
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
      doc["deviceId"] = "ESP32-S3_0X"; //<--- ใส่รหัสอุปกรณ์ (เพื่อเป็น partial key ในการแยกอุปกรณ์ใน DynamoDB)
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