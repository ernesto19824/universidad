#include "sensor.h"

Sensor::Sensor(int SS, int MOSI, int MISO, int SCK){
    Sensor::max = new Adafruit_MAX31865(SS, MOSI, MISO, SCK);
    Sensor::pt100 = new pt100rtd();
}

void Sensor::Iniciar() volatile{
    Sensor::max->begin(MAX31865_2WIRE);
}

void Sensor::Leer() volatile{
    uint16_t rtd, ohmsx100;
    uint32_t dummy;
    float ohms, Tlut;
    float Tcvd, Tcube, Tpoly, Trpoly;

    rtd = Sensor::max->readRTD();
    dummy = ((uint32_t)(rtd << 1)) * 100 * ((uint32_t) floor(Sensor::rref));
    dummy >>= 16;
    ohmsx100 = (uint16_t) (dummy & 0xFFFF);

    Sensor::checkFault();
    Sensor::Temperatura = Sensor::pt100->celsius(ohmsx100);
}

void Sensor::checkFault() volatile{
    uint8_t fault = Sensor::max->readFault();
    if(fault){
        Serial.print("Fault 0x"); Serial.println(fault, HEX);
        if (fault & MAX31865_FAULT_HIGHTHRESH)
        {
            Serial.println("RTD High Threshold"); 
        }
        if (fault & MAX31865_FAULT_LOWTHRESH)
        {
            Serial.println("RTD Low Threshold"); 
        }
        if (fault & MAX31865_FAULT_REFINLOW)
        {
            Serial.println("REFIN- > 0.85 x Bias"); 
        }
        if (fault & MAX31865_FAULT_REFINHIGH)
        {
            Serial.println("REFIN- < 0.85 x Bias - FORCE- open"); 
        }
        if (fault & MAX31865_FAULT_RTDINLOW)
        {
            Serial.println("RTDIN- < 0.85 x Bias - FORCE- open"); 
        }
        if (fault & MAX31865_FAULT_OVUV)
        {
            Serial.println("Under/Over voltage"); 
        }
        Sensor::max->clearFault();

    }
}
