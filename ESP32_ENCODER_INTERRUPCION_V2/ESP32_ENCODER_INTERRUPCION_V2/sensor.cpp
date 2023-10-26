#include "sensor.h"

Sensor::Sensor(int SSvalue, int MOSIvalue, int MISOvalue, int SCKvalue){
    Sensor::max = new Adafruit_MAX31865(SSvalue, MOSIvalue, MISOvalue, SCKvalue);
    Sensor::pt100 = new pt100rtd();
    
    Sensor::Temperatura = -1;
    Sensor::chipSelect = SSvalue;
}

void Sensor::Iniciar() volatile{
    Sensor::max->begin(MAX31865_2WIRE);
}

void Sensor::Leer() volatile{
    if(digitalRead(chipSelect) == HIGH){
      return;
    }
    
  
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

void Sensor::Desabilitar() volatile{
  digitalWrite(Sensor::chipSelect, LOW); 
}

void Sensor::Habilitar() volatile{
  digitalWrite(Sensor::chipSelect, HIGH); 
}

void Sensor::checkFault() volatile{
    uint8_t fault = Sensor::max->readFault();
    if(fault){
        Serial.print("FAULT EN SENSOR SS: ");
        Serial.println(Sensor::chipSelect);
        
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
