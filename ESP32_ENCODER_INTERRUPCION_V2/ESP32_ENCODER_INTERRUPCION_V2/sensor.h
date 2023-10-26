#ifndef sensor_h
#define sensor_h

#include <Arduino.h>
#include <Adafruit_MAX31865.h>
#include <pt100rtd.h>

class Sensor{
public:
    Sensor(int SSvalue, int MOSIvalue, int MISOvalue, int SCKvalue);
    void Iniciar() volatile;
    float Temperatura;
    void Leer() volatile;
    void Desabilitar() volatile;
    void Habilitar() volatile;

private:
    const float rref = 430.0;
    Adafruit_MAX31865* max;
    pt100rtd* pt100;
    void checkFault() volatile;
    int chipSelect;
};

#endif
