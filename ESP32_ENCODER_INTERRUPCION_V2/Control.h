#ifndef Control_h
#define Control_h

#include <Arduino.h>

class ControlPID{
public:
    ControlPID();
    float Setpoint;
    float KP;
    float KI;
    float KD;
};

#endif