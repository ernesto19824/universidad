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
    
    Menu::sensorAmbiente.Iniciar();
    Serial.println("SensorAmbiente Iniciado!");
    
    //Menu::sensorResistencia.Iniciar();
    Serial.println("SensorResistencia Removido!");

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
    static float ultimaTemperatura = 0;
    Menu::sensorAmbiente.Leer();

    if(ultimaTemperatura == Menu::sensorAmbiente.Temperatura){
      return;
    }
    ultimaTemperatura = Menu::sensorAmbiente.Temperatura;

    Menu::display->clearDisplay();
    Menu::display->setTextSize(1);
    Menu::display->setTextColor(SH110X_WHITE);
    Menu::display->setCursor(0,0);
    Menu::display->print("PID Temp Control");
    Menu::display->setCursor(0,10);
    Menu::display->print("Setpoint:");
    Menu::display->setCursor(60,10);
    Menu::display->print(Menu::controlPID->Setpoint,1);
    Menu::display->setCursor(0,20);
    Menu::display->print("Real:");
    Menu::display->setCursor(60,20);
    Menu::display->print(Menu::sensorAmbiente.Temperatura,1);
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


