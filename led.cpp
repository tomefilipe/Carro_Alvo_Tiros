#include "led.hpp"

std::map<String, int> Led::intensity = {{"W35", 20}, {"W70", 40}, {"W105", 60}, {"W140", 80}, {"W175", 120}, {"W0", 0},  {"w", 0}};

void Led::begin(uint8_t pin, uint8_t channel, uint8_t resolution, double freq){
    _pin = pin;
    _channel = channel;
    pinMode(_pin, OUTPUT);
    ledcSetup(_channel, freq, resolution);
    ledcAttachPin(_pin, _channel);
}

void Led::control(String command){
    if (intensity.find(command) != intensity.end()){
        ledcWrite(_channel, intensity[command]);
    }
}