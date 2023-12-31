#include "Menu.h"

Menu::Menu(Adafruit_SH1106G * displayPointer, ControlPID * controlPIDPointer, Encoder * encoderPointer, volatile Sensor& sensorAmbientePointer, volatile Sensor& sensorResistenciaPointer) : sensorAmbiente(sensorAmbientePointer), sensorResistencia(sensorResistenciaPointer){
    Menu::display = displayPointer;
    Menu::controlPID = controlPIDPointer;
    Menu::encoder = encoderPointer;

    Menu::paginaActual = 1;
}

void Menu::Iniciar(){
    Menu::display->begin(0x3C, true);
    Menu::display->clearDisplay();
    Menu::display->setCursor(0,0);
    Menu::display->setTextSize(1);
    Menu::display->setTextColor(SH110X_WHITE);
    Menu::display->print("Iniciando!!");
    Menu::display->display();

    vTaskDelay(1000 / portTICK_PERIOD_MS);
    Serial.println("Display Iniciado!");

    Menu::encoder->Iniciar();
    Serial.println("Encoder Iniciado!");
    
    vTaskDelay(3000 / portTICK_PERIOD_MS);
    Menu::MostrarPaginaActual();
}

void Menu::AvanzarPagina(){
    Menu::encoder->Value = 0;

    if(Menu::paginaActual < 5){
        Menu::paginaActual++;
    }
    else{
        Menu::paginaActual = 1;
    }  
}

int Menu::ObtenerPaginaActual(){
    return Menu::paginaActual;
}

void Menu::MostrarPaginaActual(){
    if(Menu::paginaActual == 1){
        Menu::MostrarTemperatura();
    }
    else if(Menu::paginaActual == 2){
        Menu::SetearTemperatura();
    }
    else if(Menu::paginaActual == 3){
        Menu::SetearKp();
    }
    else if(Menu::paginaActual == 4){
        Menu::SetearKi();
    }
    else if(Menu::paginaActual == 5){
        Menu::SetearKd();
    }
}

void Menu::MostrarTemperatura(){
    static float leerSensor = 0;
    
    static float ultimaTemperaturaAmbiente = 0;
    static float ultimaTemperaturaResistencia = 0;

    if(leerSensor == 0){
      Menu::sensorAmbiente.Habilitar();
      Menu::sensorResistencia.Desabilitar();
      Menu::sensorResistencia.Leer();
      leerSensor = 1;
    }
    else{
      Menu::sensorAmbiente.Desabilitar();
      Menu::sensorResistencia.Habilitar();
      Menu::sensorAmbiente.Leer();
      leerSensor = 0;
    }
    

    if(ultimaTemperaturaAmbiente == Menu::sensorAmbiente.Temperatura){
      if(ultimaTemperaturaResistencia == Menu::sensorResistencia.Temperatura){
        return;
      }
    }


    
    ultimaTemperaturaAmbiente = Menu::sensorAmbiente.Temperatura;
    ultimaTemperaturaResistencia = Menu::sensorResistencia.Temperatura;

    Menu::display->clearDisplay();
    Menu::display->setTextSize(1);
    Menu::display->setTextColor(SH110X_WHITE);
    
    Menu::display->setCursor(0,0);
    Menu::display->print("PID Temp Control");
    
    Menu::display->setCursor(0,10);
    Menu::display->print("Setpoint:");
    Menu::display->setCursor(80,10);
    Menu::display->print(Menu::controlPID->Setpoint,1);
    
    Menu::display->setCursor(0,20);
    Menu::display->print("Real:");
    Menu::display->setCursor(80,20);
    Menu::display->print(Menu::sensorAmbiente.Temperatura,1);

    Menu::display->setCursor(0,30);
    Menu::display->print("Resistance:");
    Menu::display->setCursor(80,30);
    Menu::display->print(Menu::sensorResistencia.Temperatura,1);
    
    Menu::display->display();
}

void Menu::SetearTemperatura(){
    Menu::controlPID->Setpoint += Menu::encoder->Value;


    Serial.println("renderizando setpoint!");

    Menu::display->clearDisplay();
    Menu::display->setTextSize(1);
    Menu::display->setTextColor(SH110X_WHITE);
    Menu::display->setCursor(0,0);
    Menu::display->print("Set temperature");
    Menu::display->setCursor(0,10);
    Menu::display->print(Menu::controlPID->Setpoint);
    Menu::display->display();
}

void Menu::SetearKp(){
    Menu::controlPID->KP += Menu::encoder->Value;
  
    Serial.println("renderizando kp!");

    Menu::display->clearDisplay();
    Menu::display->setTextSize(1);
    Menu::display->setTextColor(SH110X_WHITE);
    Menu::display->setCursor(0,0);
    Menu::display->print("Set P value ");
    Menu::display->setCursor(0,10);
    Menu::display->print(Menu::controlPID->KP);
    Menu::display->display();
}

void Menu::SetearKi(){
    Menu::controlPID->KI += Menu::encoder->Value;

    Serial.println("renderizando ki!");

    Menu::display->clearDisplay();
    Menu::display->setTextSize(1);
    Menu::display->setTextColor(SH110X_WHITE);
    Menu::display->setCursor(0,0);
    Menu::display->print("Set I value ");
    Menu::display->setCursor(0,10);
    Menu::display->print(Menu::controlPID->KI);
    Menu::display->display();
}

void Menu::SetearKd(){
    Menu::controlPID->KD += Menu::encoder->Value;

    Serial.println("renderizando kd!");

    Menu::display->clearDisplay();
    Menu::display->setTextSize(1);
    Menu::display->setTextColor(SH110X_WHITE);
    Menu::display->setCursor(0,0);
    Menu::display->print("Set D value ");
    Menu::display->setCursor(0,10);
    Menu::display->print(Menu::controlPID->KD);
    Menu::display->display();
}


