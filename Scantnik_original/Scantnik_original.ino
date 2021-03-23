/*
 *
 * 
 * THIS IS NOT THE ACTUAL CODE
 * THIS IS NOT THE ACTUAL CODE
 * THIS IS NOT THE ACTUAL CODE
 * THIS IS NOT THE ACTUAL CODE
 * THIS IS NOT THE ACTUAL CODE
 * 
 * 
 */


#include "Adafruit_CCS811.h"
#include <Wire.h>
#include <SPI.h>
#include "cactus_io_BME280_I2C.h"
#include <Adafruit_BMP280.h> // BMP280 importante conexión 3v3
#include <SoftwareSerial.h>

SoftwareSerial Tarjeta(9,10); //Pines de conexion del OpenLog // (Rx, Tx)

// Definir las conexiones del BMP280
#define BMP_SCK  (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS   (10)

  int segundo; //Añadir un valor para el tiempo 

  float bmp_Pa; //Convertir los Pa en atm
  float bme_Pa; //Convertir los mb en atm

  Adafruit_CCS811 ccs; // Crear objeto CCS811

  BME280_I2C bme (0x76);// Crear objeto BME280 con dirección 0x76

  Adafruit_BMP280 bmp;// Crear objeto BMP280 con dirección 0x77

void setup() {
 
  Serial.begin(9600);
  Tarjeta.begin(9600);

  Serial.println("CCS81 (CO2+compuesto organico volatil), GUVA_S12SD (radiación UV) y BME20/BMP280 (Temperatura, presión y humedad) ");
  Tarjeta.println("CCS81 (CO2+compuesto organico volatil), GUVA_S12SD (radiación UV) y BME20/BMP280 (Temperatura, presión y humedad) ");

    if (!bme.begin()) {
      
    Serial.println("No se ha encontrado el sensor BME280, comprobar conexiones");
    Tarjeta.println("No se ha encontrado el sensor BME280, comprobar conexiones");
   
    while (1);
}

    if (!bmp.begin()) {
    
    Serial.println(F("No se ha encontrado el sensor BMP280, comprobar conexiones"));
    Tarjeta.println(F("No se ha encontrado el sensor BMP280, comprobar conexiones"));
    
    while (1);
  }
   bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500);
  
    if(!ccs.begin()){
      
    Serial.println("No se ha encontrado el sensor CCS811, comprobar conexiones");
    Tarjeta.println("No se ha encontrado el sensor CCS811, comprobar conexiones");
    
    while(1);
  }

    Serial.println("Seg, Seg_De, CO2, COV, PresBME, PresBMP, Humed, TempBME, TempBMP, UV");
    Tarjeta.println("Seg, Seg_De, CO2, COV, PresBME, PresBMP, Humed, TempBME, TempBMP, UV");

  // Esperar hasta que el sensor esté listo
  while(!ccs.available());
}

void loop() {

  digitalWrite(4, HIGH); // Encender el Led
  
  digitalWrite(7, HIGH); // Encender el Buzzer

  segundo = segundo+1; // Cuantificar el tiempo

  Serial.print(segundo);
  Tarjeta.print(segundo);
  Serial.print(", ");
  Tarjeta.print(", ");
  
  if(ccs.available()){
    if(!ccs.readData()){

      Serial.print(ccs.geteCO2());
      Tarjeta.print(ccs.geteCO2());
      Serial.print(", ");
      Tarjeta.print(", ");
      
      Serial.print(ccs.getTVOC());
      Tarjeta.print(ccs.getTVOC());
      Serial.print(", ");
      Tarjeta.print(", ");
    }
   
    else{
      Serial.println("ERROR!");
      Tarjeta.println("ERROR!");
      while(1);
    }
  }
  bme.readSensor();

  bmp_Pa = bmp.readPressure()/101325;
  bme_Pa = bme.getPressure_MB()*100/101325;

  Serial.print(bme_Pa); Serial.print(", "); // Presión del BME280
  Tarjeta.print(bme_Pa); Tarjeta.print(", ");

  Serial.print(bmp_Pa); Serial.print(", "); // Presión del BMP280
  Tarjeta.print(bmp_Pa); Tarjeta.print(", ");
  
  Serial.print(bme.getHumidity()); Serial.print(", "); // Porcentaje de humedad ambiente
  Tarjeta.print(bme.getHumidity()); Tarjeta.print(", ");

  Serial.print(bme.getTemperature_C()); Serial.print(", "); //Temperatura en grados centígrados
  Tarjeta.print(bme.getTemperature_C()); Tarjeta.print(", ");
  
  Serial.print(bmp.readTemperature()); Serial.print(", "); // Temperatura del sensor BMP280
  Tarjeta.print(bmp.readTemperature()); Tarjeta.print(", ");


  float sensorVoltage; 
  float sensorValue;
 
  sensorValue = analogRead(A0);
  sensorVoltage = sensorValue/1024*5;
 
  Serial.print(sensorVoltage);
  Tarjeta.print(sensorVoltage);
  Serial.println("");
  Tarjeta.println("");
  
  delay(1000);

}
