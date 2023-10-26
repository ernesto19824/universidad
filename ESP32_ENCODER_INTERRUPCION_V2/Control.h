#ifndef Control_h
#define Control_h

#include <Arduino.h>

class ControlPID{
public:
    ControlPID(int salidaPwmValue);
    void Controlar(float temperatura);
    float Setpoint;
    float KP;
    float KI;
    float KD;
private:
    int salidaPwm;

    float pid_error;
    float pid_previous_error;
    int pid_p;
    int pid_i;
    int pid_d;
    unsigned long timePrev;
    unsigned long elapsedTime;
    unsigned long time;

    /*
    PID_error = set_temperature - temperature_read + 3;
    PID_p = 0.01 * kp * PID_error;
    PID_i = 0.01 * PID_i + (ki * PID_error);

    timePrev = Time;
    Time = millis();
    elapsedTime = (Time - timePrev) / 1000;

    PID_d = 0.01 * kd * ((PID_error - previous_error) / elapsedTime);
    PID_value = PID_p + PID_i + PID_d;

    if(PID_value < 0){
      PID_value = 0;
    }
    if(PID_value > 255){
      PID_value = 255;
    }
    dutyCycle = PID_value;

    ledcWrite(pwmChannel, 255-dutyCycle);
    previous_error = PID_error;
    
    
    */
};

#endif