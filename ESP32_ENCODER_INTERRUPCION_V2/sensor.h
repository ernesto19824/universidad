#ifndef sensor_h
#define sensor_h

#include <Arduino.h>
#include <Adafruit_MAX31865.h>
#include <pt100rtd.h>

class Sensor{
public:
    Sensor(int SS, int MOSI, int MISO, int SCK);
    void Iniciar() volatile;
    float Temperatura;
    void Leer() volatile;

private:
    const float rref = 430.0;
    Adafruit_MAX31865* max;
    pt100rtd* pt100;
    void checkFault() volatile;
};

#endif
