#include "Control.h"

ControlPID::ControlPID(){
    ControlPID::Setpoint = 0;
    ControlPID::KP = 0;
    ControlPID::KI = 0;
    ControlPID::KD = 0;

    ControlPID::salidaPwm = 0;
}

float ControlPID::CalcularValor(float temperatura){

}
