#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "HX711.h"

WiFiMulti wifiMulti;

const char* server_url = "http://...../api/infusion/update";


const char* device_id = ".......";
const char* record_id = ".......";
const char* nurse_id  = ".......";

const int LASER_PIN = 4;
const int HX711_DT = 16;
const int HX711_SCK = 17;
const int BATTERY_PIN = 34;

struct IVData {
  float current_rate;
  float remaining_vol;
  bool is_alert;
  bool low_battery;
};

QueueHandle_t dataQueue;
HX711 scale;
volatile uint16_t drop_count = 0;
volatile unsigned long last_drop_time = 0;
const unsigned long DEBOUNCE_TIME = 50;

void IRAM_ATTR dropSensorISR() {
  unsigned long current_time = millis();
  if (current_time - last_drop_time > DEBOUNCE_TIME) {
    drop_count++;
    last_drop_time = current_time;
  }
}

void TaskSendToServer(void *pvParameters) {
  IVData data;
  char jsonBuffer[256];

  while (1) {
    if (wifiMulti.run() == WL_CONNECTED) {
      if (xQueueReceive(dataQueue, &data, (TickType_t)10) == pdPASS) {

        snprintf(jsonBuffer, sizeof(jsonBuffer),
         "{\"record_id\":\"%s\", \"device_id\":\"%s\", \"nurse_id\":\"%s\", \"rate\":%.1f, \"vol\":%.1f, \"alert\":%s, \"low_bat\":%s}",
         record_id, device_id, nurse_id, data.current_rate,
         data.remaining_vol, data.is_alert ? "true" : "false",
         data.low_battery ? "true" : "false");

        HTTPClient http;
        http.begin(server_url);
        http.addHeader("Content-Type", "application/json");

        int httpCode = http.POST(jsonBuffer);

        if (httpCode > 0) {
          Serial.printf("[HTTP] POST thành công, mã: %d\n", httpCode);
        } else {
          Serial.printf("[HTTP] Lỗi: %s\n", http.errorToString(httpCode).c_str());
          xQueueSendToFront(dataQueue, &data, (TickType_t)0);
        }
        http.end();
      }
    }
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LASER_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(LASER_PIN), dropSensorISR, FALLING);

  scale.begin(HX711_DT, HX711_SCK);
  scale.set_scale(420);
  scale.tare();

  dataQueue = xQueueCreate(50, sizeof(IVData));

  wifiMulti.addAP("Tang1_Network", "pass_tang_1");

  xTaskCreatePinnedToCore(TaskSendToServer, "NetworkTask", 8192, NULL, 1, NULL, 0);
}

void loop() {
  unsigned long current_time = millis();
  static unsigned long time_of_7_drops_ago = 0;
  static uint16_t last_drop_count_processed = 0;

  if (drop_count - last_drop_count_processed >= 7) {

    unsigned long duration = current_time - time_of_7_drops_ago;

    float average_rate = (7.0 / (float)duration) * 60000.0;

    float weight = 0.0;
    if (scale.is_ready()) weight = scale.get_units(3);
    bool low_bat = (analogRead(BATTERY_PIN) < 1800);

    IVData currentData = {average_rate, weight, false, low_bat};
    xQueueSend(dataQueue, &currentData, (TickType_t)0);

    last_drop_count_processed = drop_count;
    time_of_7_drops_ago = current_time;
  }

  if (current_time - last_drop_time > 10000 && drop_count > 0) {
      IVData alertData = {0.0, 0.0, true, false};
      xQueueSend(dataQueue, &alertData, (TickType_t)0);
  }
}