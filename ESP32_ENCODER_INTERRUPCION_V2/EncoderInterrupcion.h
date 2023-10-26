#ifndef EncoderInterrupcion_h
#define EncoderInterrupcion_h

#include <Arduino.h> // Asegúrate de incluir la biblioteca Arduino si estás utilizando tipos de datos de Arduino


class Encoder{
public:
    Encoder(int clkPin, int dtPin, int swPin, unsigned long debounceDelayValue);
    static void Iniciar();
    static void Leer();
    static void LeerValor();
    static void Reiniciar();
    static bool IsPressed;
    static int Value;
private:
    static int clk;
    static int dt;
    static int sw;
    static int lastValue;
    static uint8_t state;
    static uint8_t lastState;
    static unsigned long debounceDelay;
    static unsigned long lastMillis;
    static bool realIsPressed;
    static int realValue;

    static void interrupcionRotacion();
    static void interrupcionPresionar();
};

#endif
