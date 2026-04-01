#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h> 
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "HX711.h"

WiFiMulti wifiMulti;

const char* server_url = "https://...../api/infusion/update"; 

const char* root_ca = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRnXxCGmz...\n" \
"-----END CERTIFICATE-----\n";

const int LASER_PIN = 4;
const int HX711_DT = 16;
const int HX711_SCK = 17;
const int PIN_RED = 8;   
const int PIN_GREEN = 9; 
const int PIN_BLUE = 11;  


const int BATTERY_PIN = 34; 

HX711 scale;
volatile uint16_t drop_count = 0;
volatile unsigned long last_drop_time = 0;
const unsigned long DEBOUNCE_TIME = 50;

struct IVData {
  int session_id;     
  uint32_t timestamp;
  float current_rate;  
  float remaining_vol; 
  char status[15];      
  bool low_battery;    
};

QueueHandle_t dataQueue;
const char* device_id = "ESP32_Giuong01";

void updateLEDs(int red, int green, int blue) {
  analogWrite(PIN_RED, red);
  analogWrite(PIN_GREEN, green); 
  analogWrite(PIN_BLUE, blue); 
}

void IRAM_ATTR dropSensorISR() {
  unsigned long current_time = millis();
  if (current_time - last_drop_time > DEBOUNCE_TIME) {
    drop_count++;
    last_drop_time = current_time;
  }
}

void TaskSendToServer(void *pvParameters) {
  IVData dataToSend;
  char jsonBuffer[150];
  
  while (1) {
    if (wifiMulti.run() == WL_CONNECTED) {
      if (xQueueReceive(dataQueue, &dataToSend, (TickType_t)10) == pdPASS) {
        
        snprintf(jsonBuffer, sizeof(jsonBuffer), 
         "{\"session_id\":%d, \"device_id\":\"%s\", \"rate\":%.1f, \"vol\":%.1f, \"status\":\"%s\", \"low_bat\":%s}", 
         dataToSend.session_id, device_id, dataToSend.current_rate, 
         dataToSend.remaining_vol, dataToSend.status,
         dataToSend.low_battery ? "true" : "false");

        WiFiClientSecure client;
        client.setCACert(root_ca);

        HTTPClient http;
        
        Serial.print("[HTTPS] Đang gửi dữ liệu... ");
        if (http.begin(client, server_url)) {
          http.addHeader("Content-Type", "application/json");
          
          int httpCode = http.POST(jsonBuffer);

          if (httpCode > 0) {
            Serial.printf("Mã phản hồi: %d\n", httpCode);
            if (httpCode != HTTP_CODE_OK && httpCode != HTTP_CODE_CREATED) {
                xQueueSendToFront(dataQueue, &dataToSend, (TickType_t)0);
            }
          } else {
            Serial.printf("Lỗi kết nối: %s\n", http.errorToString(httpCode).c_str());
            xQueueSendToFront(dataQueue, &dataToSend, (TickType_t)0);
          }
          http.end();
        }
      }
    }
    vTaskDelay(pdMS_TO_TICKS(100)); 
  }
}

void setup() {
Serial.begin(115200);
  
  pinMode(PIN_RED, OUTPUT);    
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT); 
  pinMode(LASER_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(LASER_PIN), dropSensorISR, FALLING);
  
  scale.begin(HX711_DT, HX711_SCK);
  scale.set_scale(420); 
  scale.tare();

  dataQueue = xQueueCreate(100, sizeof(IVData)); 

  wifiMulti.addAP("Tang1_Network", "pass_tang_1");
  wifiMulti.addAP("Tang2_Network", "pass_tang_2");

  xTaskCreatePinnedToCore(TaskSendToServer, "HTTPS_Task", 8192, NULL, 1, NULL, 0);
}

unsigned long last_process_time = 0;
uint16_t last_sent_drops = 0; 
int current_session_id = 1; // 
uint16_t prev_drops = 0;
unsigned long last_rate_calc_time = 0;

void loop() {
  unsigned long current_time = millis();
  static unsigned long last_process_time = 0;
  static uint16_t last_sent_drops = 0;

  if (current_time - last_process_time >= 1000) {
    last_process_time = current_time;

    noInterrupts();
    uint16_t current_drops = drop_count;
    unsigned long time_since_last_drop = current_time - last_drop_time;
    interrupts();

    float current_weight = 0.0;
    if (scale.is_ready()) {
      current_weight = scale.get_units(3);
      if (current_weight < 0) current_weight = 0.0;
    }

    int battery_val = analogRead(BATTERY_PIN);
    bool is_low_battery = (battery_val < 1800); 

    bool is_fluid_empty = (current_weight < 20.0);
    bool is_blocked = (time_since_last_drop > 10000 && current_weight >= 20.0);
    bool is_alert = is_fluid_empty || is_blocked;

    if (is_alert) updateLEDs(255, 0, 0);
    else if (is_low_battery) updateLEDs(255, 255, 0);
    else updateLEDs(0, 255, 0);

    bool should_send_data = false;
    if (current_drops - last_sent_drops >= 7) {
      should_send_data = true;
    } 
    else if ((is_alert || is_low_battery) && (current_time % 5000 < 1000)) {
      should_send_data = true;
    }

    if (should_send_data) {
      IVData currentData = {current_time, current_drops, current_weight, is_alert, is_low_battery};
      xQueueSend(dataQueue, &currentData, (TickType_t)0);
      last_sent_drops = current_drops;
    }
  }
}
