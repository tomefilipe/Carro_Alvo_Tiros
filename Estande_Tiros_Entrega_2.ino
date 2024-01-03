#ifdef CORE_DEBUG_LEVEL
#undef CORE_DEBUG_LEVEL
#endif

#define CORE_DEBUG_LEVEL 5
#define LOG_LOCAL_LEVEL ESP_LOG_DEBUG


#include <Arduino.h>

#include <esp_log.h>

#include "OTA.h"
#include "streamer.hpp"
#include "camProvider.hpp"
#include "index.hpp"
#include "wifi_manager.hpp"
#include "led.hpp"
#include "actuator.hpp" 
#include "pins.h"

Streamer server;
CamProvider cam;
OTA ota;
Led led;
Actuator motor(ENABLE_PIN, DIRECTION_PIN, VELOCITY_PIN, SENSOR_PIN);


void indexR(){
    String response = Pages::indexCrop;
    server.send(200, "text/html", response);
}

void iterCrop(){
    String response = Pages::iteCrop;
    server.send(200, "text/html", response);
}

void control(){
    String response = "ok";
    Serial.println(server.argName(0));
    Serial.println(server.arg(0));
    if(server.arg(0) == "B") {
      Serial.print("Pedindo Nivel de Bateria: ");
      int voltage = (100 *analogRead(33))/3681;
      if (voltage > 100) voltage = 100;
      response = String(voltage);   
    }
    motor.setDesiredPos(server.arg(0));
    led.control(server.arg(0));

    server.send(200, "text/html", response);
}

void setup(){
    pinMode(33, INPUT);
    Serial.begin(115200);
    delay(5000);
    log_i("iniciou");

    WiFiManager.begin();

    server.on("/", &indexR);
    server.on("/crop", &iterCrop);
    server.on("/cmd", &control);
    ota.begin(server.server());
    server.begin(20, &cam);

    motor.begin(1000);
   // motor.begin(1300);
    led.begin(LED_PIN, LED_CHANNEL);
}

void loop(){
    vTaskDelay(1000);
}
