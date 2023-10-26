#include "EncoderInterrupcion.h"

bool Encoder::IsPressed = false;
int Encoder::Value = 0;
int Encoder::clk = 0;
int Encoder::dt = 0;
int Encoder::sw = 0;

int Encoder::lastValue = 0;
uint8_t Encoder::state = 0;
uint8_t Encoder::lastState = 0;
unsigned long Encoder::debounceDelay = 0;
unsigned long Encoder::lastMillis = 0;
bool Encoder::realIsPressed = false;
int Encoder::realValue = 0;

Encoder::Encoder(int clkPin, int dtPin, int swPin, unsigned long debounceDelayValue){
    Encoder::clk = clkPin;
    Encoder::dt = dtPin;
    Encoder::sw = swPin;
    Encoder::debounceDelay = debounceDelayValue;

    Encoder::IsPressed = false;
    Encoder::Value = 0;

    Encoder::lastValue = 0;
    Encoder::state = 0;
    Encoder::lastState = 0;
    Encoder::lastMillis = 0;
    Encoder::realIsPressed = false;
    Encoder::realValue = 0;
}

void Encoder::Iniciar(){
    pinMode(Encoder::clk, INPUT_PULLUP);
    pinMode(Encoder::dt, INPUT_PULLUP);
    pinMode(Encoder::sw, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(Encoder::clk), Encoder::interrupcionRotacion, CHANGE);
    attachInterrupt(digitalPinToInterrupt(Encoder::sw), Encoder::interrupcionPresionar, FALLING);
}

void Encoder::Leer(){
    if(Encoder::realIsPressed){
        Encoder::IsPressed = true;
        Encoder::realIsPressed = false;
    }
    else{
        Encoder::IsPressed = false;
    }

    Encoder::Value = Encoder::realValue;
}

void Encoder::LeerValor(){
    Encoder::Value = Encoder::realValue;
} 

void Encoder::Reiniciar(){
    Encoder::IsPressed = false;
    Encoder::Value = 0;

    Encoder::lastValue = 0;
    Encoder::state = 0;
    Encoder::lastState = 0;
    Encoder::lastMillis = 0;
    Encoder::realIsPressed = false;
    Encoder::realValue = 0;
}

void Encoder::interrupcionRotacion(){
    unsigned long currentMillis = millis();
    
    if (currentMillis - Encoder::lastMillis < Encoder::debounceDelay) {
        return; // Sal de la ISR si no ha pasado suficiente tiempo
    }

    Encoder::lastState = Encoder::state;
    Encoder::state = (digitalRead(Encoder::dt) << 1) | digitalRead(Encoder::clk);

    if (Encoder::state != Encoder::lastState) {
        if (Encoder::lastState == 0b00) {
            if (Encoder::state == 0b01) Encoder::realValue--;
            if (Encoder::state == 0b10) Encoder::realValue++;
        } else if (Encoder::lastState == 0b01) {
            if (Encoder::state == 0b00) Encoder::realValue++;
            if (Encoder::state == 0b11) Encoder::realValue--;
        } else if (Encoder::lastState == 0b10) {
            if (Encoder::state == 0b00) Encoder::realValue--;
            if (Encoder::state == 0b11) Encoder::realValue++;
        } else { // oldState == 0b11
            if (Encoder::state == 0b01) Encoder::realValue++;
            if (Encoder::state == 0b10) Encoder::realValue--;
        }
    }

    Encoder::lastMillis = currentMillis; // Actualiza la última vez que se ejecutó la ISR
}

void Encoder::interrupcionPresionar(){
    Encoder::realIsPressed = true;
}
