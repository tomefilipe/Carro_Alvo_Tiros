#ifdef CORE_DEBUG_LEVEL
#undef CORE_DEBUG_LEVEL
#endif

#define CORE_DEBUG_LEVEL 5
#define LOG_LOCAL_LEVEL ESP_LOG_DEBUG


#pragma once
#ifndef __ACTUATOR__
#define __ACTUATOR__

#include <Arduino.h>
#include <map>

enum actuator_state_t {
    ACTUATOR_NORMAL,
    ACTUATOR_SAFE,
    ACTUATOR_FINDING_HOME
};

class Actuator{
public:
    Actuator(uint8_t enablePin, uint8_t directionPin, uint8_t velocityPin, uint8_t sensorPin);
    
    /// inicia timer @param period periodo em us de meia onda
    void begin(int period);

    void setDirection();
    void setDesiredPos(String commando);
    void homeManager(bool reinit = false);
    bool slowDown(bool reinit = false);

    volatile int _desired, _current, _slowPeriod;
    volatile bool _direction, _enabled;
    uint8_t _enablePin, _directionPin, _velocityPin, _sensorPin;
    volatile uint8_t _state;

protected:
    hw_timer_t * timer = NULL;
    static std::map <String,int> incrementalPos;
    static std::map <String,int> absolutePos;
    static String homeCommand;
    static Actuator * motor;
    
    //static const int FIRST_GEAR = 2.5;
    //static const int SLOW_STEPS = 50;
    //static const int FIRST_GEAR = 6;
    //static const int SLOW_STEPS = 100;
    
    static const int FIRST_GEAR = 2;  //2 para motor fraco   1.7 para motor forte
    static const int SLOW_STEPS = 300;
    static const int SLOW_PERIOD = 2 * FIRST_GEAR * SLOW_STEPS;

    

    IRAM_ATTR static void actuate();
    IRAM_ATTR static void atHome(void *args);

};

#endif
