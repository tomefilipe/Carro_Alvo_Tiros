#pragma once
#include <Arduino.h>
#include <map>

class Led{
public:
    void begin(uint8_t pin, uint8_t channel, uint8_t resolution = 8, double freq = 5000);
    void control(String command);
protected:
    uint8_t _pin, _channel;
    static std::map<String, int> intensity;
};