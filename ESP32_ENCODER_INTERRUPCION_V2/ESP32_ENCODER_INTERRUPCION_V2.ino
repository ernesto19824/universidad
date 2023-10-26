#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#include "EncoderInterrupcion.h"
#include "Control.h"
#include "Menu.h"
#include "sensor.h"

volatile Sensor SensorAmbiente(15, MOSI, MISO, SCK);
volatile Sensor SensorResistencia(17, MOSI, MISO, SCK);

void Nucleo0(void *pvParameters) {
  (void) pvParameters;

  
  pinMode(15, OUTPUT);
  pinMode(17, OUTPUT);
  digitalWrite(15, HIGH);
  digitalWrite(17, LOW);

  SensorAmbiente.Iniciar();
  SensorResistencia.Iniciar();
  
  for (;;) {
    Serial.print("\n\nEjecutando en el Núcleo 0: ");
    Serial.println(xPortGetCoreID());

    Serial.print("Temperatura ambiente: ");
    Serial.println(SensorAmbiente.Temperatura);

    Serial.print("Temperatura resistencia: ");
    Serial.println(SensorResistencia.Temperatura);

    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

void Nucleo1(void *pvParameters) {
  (void) pvParameters;

  //width, height, wire, reset
  Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, &Wire, -1);
  Serial.println("display declarado");
  
  //clk, dt, sw
  Encoder encoder(2,4,3, 1);
  Serial.println("encoder declarado");

  
  ControlPID controlPID(34);
  Serial.println("Control pid declarado");
  
  Menu navegacion(&display, &controlPID, &encoder, SensorAmbiente, SensorResistencia);
  Serial.println("Menu declarado");
  
  navegacion.Iniciar();
  Serial.println("Menu navegacion Iniciado!");


  for (;;) {
    encoder.Leer();

    if(encoder.IsPressed){
      navegacion.AvanzarPagina();
      vTaskDelay(250 / portTICK_PERIOD_MS);
      navegacion.MostrarPaginaActual();
      encoder.Reiniciar();
      continue;
    }
    else{
      /*Serial.println("\n");
      Serial.print("\nValor encoder antes delay: ");
      Serial.println(encoder.Value);
      Serial.println("");*/
      if(navegacion.ObtenerPaginaActual() == 1){
        vTaskDelay(250 / portTICK_PERIOD_MS);
        navegacion.MostrarPaginaActual();
        continue;
      }
      
      encoder.Reiniciar();
      
      vTaskDelay(250 / portTICK_PERIOD_MS);
      encoder.LeerValor();
      if(encoder.Value == 0){
        Serial.println("No se ha girado el encoder!");
        continue;
      }

      Serial.println("");
      Serial.print("Valor encoder luego delay: ");
      Serial.println(encoder.Value);
      Serial.println("");

      navegacion.MostrarPaginaActual();
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("holaaaa! codigo con encoder, pantalla dos sensores declarados y uno andando, ademas de cambio de pin! Tambien tamanio pila 15k y log de declaraciones e inicios! La inicializacion del sensor resistencia ha sido comentada! Se muestra el estado del giro del encoder! Se aumento delay lectura encoder! Se agrego espacio serial print para ver correctamente!");
  Serial.println("CODIGO PRUEBA!!! Probando que pasa con la lectura interrumpida cuando se hace delay");
  Serial.println("Falla posiblemente solucionada! La solucion era volver a leer el valor luego del delay");
  Serial.println("Otro fallo posible. Se creo la funcion Reiniciar para el encoder. Es posible que falle la funcion renderizar menus");
  Serial.println("Reinicio del encoder aumentado!");
  Serial.println("Se eliminio un reiniciar que estaba al final");
  Serial.println("Cambio para actualizar temperatura en display automaticamente");
  Serial.println("Probando iniciar el sensor en otro nucleo! UPDATE: FUNCIONO!");
  Serial.println("Probando iniciar el otro sensor!!!!! Usando high y low! Pequenio cambio libreria!");
  Serial.println("IMPORTANTE! NO COLOCAR NADA EN CLK! Usando pin 17");
  Serial.println("Cambio para agregar chipselect al sensor!");
  Serial.println("Cambio para agregar funcion habilitar/deshabilitar al sensor! Se agrego identificador fault!");
  Serial.println("Probando porque no andan ambos sensores!");
  Serial.println("Agregado control PID!");

  

  Serial.println("\n\n");
  xTaskCreatePinnedToCore(Nucleo0, "Task0", 1000, NULL, 2, NULL, 0);

  vTaskDelay(2000 / portTICK_PERIOD_MS);
  
  xTaskCreatePinnedToCore(Nucleo1, "Task1", 15000, NULL, 2, NULL, 1);
}

void loop() {
  // put your main code here, to run repeatedly:
  vTaskDelay(portMAX_DELAY);
}
