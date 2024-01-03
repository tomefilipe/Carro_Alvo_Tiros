#include "actuator.hpp"

//"V" , 38583
std::map <String,int> Actuator::incrementalPos = {{"M" , 1930}, {"m" , -1930}};
std::map <String,int> Actuator::absolutePos = {{"C" , 9645}, {"D" , 19291}, {"Q" , 28937}, {"V" , 34000}, {"TESTE", 10}};
String Actuator::homeCommand = "Z";

Actuator *Actuator::motor = nullptr;

Actuator::Actuator(uint8_t enablePin, uint8_t directionPin, uint8_t velocityPin, uint8_t sensorPin):
          _desired(0), _current(0), _slowPeriod(0),_direction(0), _enabled(false),
           _enablePin(enablePin), _directionPin(directionPin), _velocityPin(velocityPin), _sensorPin(sensorPin), _state(ACTUATOR_NORMAL){}

void Actuator::begin(int period){

    timer = timerBegin(0, 80, true);                
    timerAttachInterrupt(timer, &actuate, true);
    motor = this;  
    
   // Sets an alarm to sound every second
    timerAlarmWrite(timer, period/2, true);          
    // timerAlarmEnable(timer);
    timerAlarmDisable(timer);

    pinMode(_enablePin, OUTPUT);
    pinMode(_directionPin, OUTPUT);
    pinMode(_velocityPin, OUTPUT);

    /// config sensor
    pinMode(_sensorPin, INPUT_PULLUP);
    gpio_isr_handler_add((gpio_num_t)_sensorPin, atHome, NULL);
    gpio_set_intr_type((gpio_num_t)_sensorPin, GPIO_INTR_NEGEDGE);
    

    /// Disable motor
    digitalWrite(_enablePin, HIGH);
    log_i("enable is HIGH");

}

void Actuator::setDirection(){
    _enabled = (_desired != _current);
    _direction = (_desired < _current);
    digitalWrite(_directionPin, (uint8_t) _direction);
    digitalWrite(_enablePin, (uint8_t) !_enabled);
    log_i("Desired pos: %d, Current Pos: %d\n", _desired, _current);
    log_i("Direction is %hhu", (uint8_t) _direction);
    log_i("Enable is %hhu", (uint8_t) !_enabled);
}

void Actuator::setDesiredPos(String command){
    if (command == homeCommand){
        homeManager(true);
        slowDown(true);
        setDirection();
        gpio_intr_enable((gpio_num_t)_sensorPin);
        timerAlarmEnable(timer);

    }
    else if( incrementalPos.find(command) != incrementalPos.end() ){
        _desired += incrementalPos[command];
        slowDown(true);
        setDirection();
        timerAlarmEnable(timer);
    }
    else if ( absolutePos.find(command) != absolutePos.end() ){
        _desired = absolutePos[command];
        slowDown(true);
        setDirection();
        timerAlarmEnable(timer);
    }
}

void Actuator::homeManager(bool reinit){
    if (reinit){
        log_i("going to home - safe");
        _desired = _current + SLOW_STEPS;
        _state = ACTUATOR_SAFE;
    }else if ( _state == ACTUATOR_SAFE ){
        if( _slowPeriod >= SLOW_PERIOD){
            log_i("going to home");
            _state = ACTUATOR_FINDING_HOME;
            _desired = - __INT_MAX__;
            slowDown(true);
            setDirection();
            timerAlarmEnable(timer);
        }
    }
}

bool Actuator::slowDown(bool reinit){
    static int steps;
    if(reinit){
        steps = 0;
        _slowPeriod = 0;
        log_v("First gear");
    }
    else if (motor->_slowPeriod++ < SLOW_PERIOD){
        steps = (steps + 1) % FIRST_GEAR;
        return steps == 0;
    }
    return true;
}

void Actuator::actuate(){
    static uint8_t state;
    if( motor->slowDown() ){
        if( motor->_current != motor->_desired || motor->_state == ACTUATOR_FINDING_HOME){
            digitalWrite(motor->_velocityPin, state^=1);
            if (state) motor->_current += (motor->_direction)? -1 : 1;
        }
        else if (motor->_enabled){
            digitalWrite(motor->_enablePin, HIGH);
            motor->_enabled = false;
            timerAlarmDisable(motor->timer);
        }
        motor->homeManager();
    }
}

void Actuator::atHome(void *args){
    motor->_current = 0;
    motor->_desired = motor->_current;
    motor->_state = ACTUATOR_NORMAL;
    gpio_intr_disable((gpio_num_t) motor->_sensorPin);
}
