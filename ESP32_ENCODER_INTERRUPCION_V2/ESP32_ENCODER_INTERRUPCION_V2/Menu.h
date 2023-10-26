#ifndef Menu_h
#define Menu_h

#include <Arduino.h> // Asegúrate de incluir la biblioteca Arduino si estás utilizando tipos de datos de Arduino
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#include "EncoderInterrupcion.h"
#include "Control.h"
#include "sensor.h"


class Menu{
public:
    Menu(Adafruit_SH1106G * displayPointer, ControlPID * controlPIDPointer, Encoder * encoderPointer, volatile Sensor& sensorAmbientePointer, volatile Sensor& sensorResistenciaPointer);
    void Iniciar();
    void AvanzarPagina();
    int ObtenerPaginaActual();
    void MostrarPaginaActual();

    void MostrarTemperatura();
    void SetearTemperatura();
    void SetearKp();
    void SetearKi();
    void SetearKd();
private:
    int paginaActual;
    Adafruit_SH1106G * display;
    ControlPID * controlPID;
    Encoder * encoder;
    volatile Sensor& sensorAmbiente;
    volatile Sensor& sensorResistencia;
};

#endif
