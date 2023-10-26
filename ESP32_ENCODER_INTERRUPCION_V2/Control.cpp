#include "Control.h"

ControlPID::ControlPID(int salidaPwmValue){
    ControlPID::Setpoint = 0;
    ControlPID::KP = 0;
    ControlPID::KI = 0;
    ControlPID::KD = 0;

    ControlPID::salidaPwm = salidaPwmValue;

    ControlPID::pid_error = 0;
    ControlPID::pid_previous_error = 0;
    ControlPID::pid_p = 0;
    ControlPID::pid_i = 0;
    ControlPID::pid_d = 0;
    ControlPID::timePrev = 0;
    ControlPID::elapsedTime = 0;
    ControlPID::time = 0;
    
}

void ControlPID::Controlar(float temperatura){
    ControlPID::pid_error = ControlPID::Setpoint - temperatura + 3;
    ControlPID::pid_p = 0.01 * ControlPID::KP * ControlPID::pid_error;
    ControlPID::pid_i = 0.01 * ControlPID::pid_i + (ControlPID::KI * ControlPID::pid_error);

    ControlPID::timePrev = ControlPID::time;
    ControlPID::time = millis();
    ControlPID::elapsedTime = (ControlPID::time - ControlPID::timePrev) / 1000;

    ControlPID::pid_d = 0.01 * ControlPID::KD * ((ControlPID::pid_error - ControlPID::pid_previous_error) / ControlPID::elapsedTime );

    float pid_value = ControlPID::pid_p + ControlPID::pid_i + ControlPID::pid_d;
    float dutyCycle = constrain(pid_value, 0, 255);

    ControlPID::pid_previous_error = ControlPID::pid_error;
    ledcWrite(ControlPID::salidaPwm, 255-dutyCycle);

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
}
