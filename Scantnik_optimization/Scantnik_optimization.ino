/* Versión optimizada del código original del equipo Scantnik2021
 * Optimized version from the original Scantnik2021 team code
 * ----------------------------------------------------------------
 *  
 *  **Funcionando y probado: | Working and tested:**
 * 
 *  Nada de momento | Nothing yet :)
 * 
 * 
 * ----------------------------------------------------------------
 * 
 *  **No probado: | Not tested:**
 *  
 *  Envío APC220 | APC220 sending
 *  Lectura sensores | Sensor reading
 *  Guardar en SD | Saving on SD
 *  
 * 
 * ----------------------------------------------------------------
 * 
 *  **Cadena de envío/recepción: | String:**
 * 
 *  ident, Seg, CO2, COV, PresBME, PresBMP, Humed, TempBME, TempBMP, UV
 * 
 */


#include "Adafruit_CCS811.h"  // Libreria necesaria para el CCS811 | Library for the CCS811
#include <Wire.h> // Libreria necesaria para la comunicación por I2C | Library for I2C communication
#include <SPI.h>  // Libreria necesaria para la comunicación por SPI | Library for SPI communication
#include "cactus_io_BME280_I2C.h" // Libreria necesaria para el BME280 | Library for the BME280
#include <Adafruit_BMP280.h>  // Libreria necesaria para el BMP280 | Library for the BMP280
#include <SoftwareSerial.h> // Libreria necesaria para crear puertos serial en pines digitales (para la tarjeta SD) | Library for creating serial ports on digital arduino ports (for the SD card)

SoftwareSerial Tarjeta(9,10); //Crear puerto serie para el OpenLog | Creation of the serial port for the OpenLog (Rx, Tx)

// // Definir las conexiones del BMP280
// #define BMP_SCK  (13)
// #define BMP_MISO (12)
// #define BMP_MOSI (11)
// #define BMP_CS   (10)

  float segundo; //Añadir un valor para el tiempo | Add a float for the millis()

  int i = 0; //Añadir un identificador de dato | Add a data identificator

  float bmp_Pa; //Float para almacenar los valores de presión del BMP
  float bme_Pa; //Float para almacenar los valores de presión del BMP

  Adafruit_CCS811 ccs; // Crear objeto CCS811 | Create CSS811 object

  BME280_I2C bme (0x76);  // Crear objeto BME280 con dirección 0x76 | Create BME280 object with 0x76 address

  Adafruit_BMP280 bmp;  // Crear objeto BMP280 con dirección 0x77 | Create BMP280 object with 0x77 address

void setup() {
 
  Serial.begin(9600); //Iniciar el puerto serie por defecto | Start the default serial port
  Tarjeta.begin(9600);  //Iniciar el puerto serie para el OpenLog | Start the OpenLog serial port

  pinMode(4, OUTPUT); //Configurar el puerto 4 como salida (led) | Configure the port 4 like an output (led)

  Serial.println("Iniciando..."); //Imprimir estado en el puerto serie por defecto (APC) | Print status on the default serial port (APC)
  Tarjeta.println("Iniciando..."); //Imprimir estado en el puerto serie por defecto (SD) | Print status on the default serial port (SD)

  if (!bme.begin()) {
      
    Serial.println("No se ha encontrado el sensor BME280, comprobar conexiones"); //Avisar de que no se ha encontrado el BME280 (APC) | Warn about BME280 failure (APC)
    Tarjeta.println("No se ha encontrado el sensor BME280, comprobar conexiones"); //Avisar de que no se ha encontrado el BME280 (SD) | Warn about BME280 failure (SD)
    delay(5000);

  }

  if (!bmp.begin()) {
    
    Serial.println(F("No se ha encontrado el sensor BMP280, comprobar conexiones"));  //Avisar de que no se ha encontrado el BMP280 (APC) | Warn about BMP280 failure (APC)
    Tarjeta.println(F("No se ha encontrado el sensor BMP280, comprobar conexiones")); //Avisar de que no se ha encontrado el BMP280 (SD) | Warn about BMP280 failure (SD)
    delay(5000);

  }
  
  // bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
  //                 Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
  //                 Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
  //                 Adafruit_BMP280::FILTER_X16,      /* Filtering. */
  //                 Adafruit_BMP280::STANDBY_MS_500);
  
  if(!ccs.begin()){
      
    Serial.println("No se ha encontrado el sensor CCS811, comprobar conexiones"); //Avisar de que no se ha encontrado el CCS811 (APC) | Warn about CCS811 failure (APC)
    Tarjeta.println("No se ha encontrado el sensor CCS811, comprobar conexiones"); //Avisar de que no se ha encontrado el CCS811 (SD) | Warn about CCS811 failure (SD)
    delay(5000);

  }
}

void loop() {

  String data; //Crear una cadena para el envio de datos | Create a String for the data

  digitalWrite(4, HIGH); // Encender el led | Power on the led

  segundo = millis(); //Guardar el tiempo actual en la float "segundo" | Store the actual time on the "segundo" float.

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
