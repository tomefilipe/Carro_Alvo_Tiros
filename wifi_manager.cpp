#include "wifi_manager.hpp"

void WiFiManager_::begin(){
    if (!MDNS.begin(SERVERNAME)) {
        Serial.println("Error setting up MDNS responder!");
        ESP.restart();
    }
    Serial.println("mDNS responder started");
    staticConnect();
    //  connect();
}

bool WiFiManager_::connect(){
    WiFi.begin(SSID, PASSWORD);
    WiFi.setSleep(false);
    int cont = 240;
    while ( WiFi.status() != WL_CONNECTED && cont--){
        delay(500);
    }
    if(cont <= 0){
        Serial.println("Erro ao conectar ao wifi, reiniciando o esp32");
        ESP.restart();
        return false;
    }
    Serial.println(WiFi.localIP());
    Serial.println(WiFi.gatewayIP());
    return true;
}

bool WiFiManager_::staticConnect(){
    WiFi.config(_ip, _gwip, _subnet);
    return connect();
}

uint32_t getGatewayIp(){
    return 0;
}

WiFiManager_ WiFiManager;
