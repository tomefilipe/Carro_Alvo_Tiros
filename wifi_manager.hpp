#pragma once
#include <WiFi.h>
#include <ESPmDNS.h>

class WiFiManager_{
public:
    void begin();
    bool connect();
    bool staticConnect();

protected:
   
    //const char *SSID = "KHALIL";
    //const char *PASSWORD = "SiK!33950";

    //const char *SSID = "FT";
    //const char *PASSWORD = "f135717f";

    
    
    const char *SSID = "Tac Pro Shooting Range";
    const char *PASSWORD = "tacpro357@";
    
    const char *SERVERNAME = "carro_14";
    const IPAddress _ip = IPAddress(192, 168, 1, 64);
    const IPAddress _gwip = IPAddress(192, 168, 1, 254);
    const IPAddress _subnet = IPAddress(255, 255, 255, 0);

    uint32_t _getGatewayIp();
};

extern WiFiManager_ WiFiManager;
