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

//--------------------------------------------------------------------------

SoftwareSerial Tarjeta(9,10); //Crear puerto serie para el OpenLog | Creation of the serial port for the OpenLog (Rx, Tx)

//--------------------------------------------------------------------------

float segundo; //Añadir un valor para el tiempo | Add a float for the millis()
float i; //Añadir un identificador de dato | Add a data identificator

//--------------------------------------------------------------------------

float bmp_Pa; //Float para almacenar los valores de presión del BMP | Float for storing the pressure values from the BMP
float bme_Pa; //Float para almacenar los valores de presión del BME | Float for storing the pressure values from the BME
float co2;  //Float para almacenar los valores de CO2 | Float for storing CO2 values
float cov; //Float para almacenar los valores de COV | Float for storing COV values
float UVsensorVoltage; //Float para almacenar los valores de Voltaje del sensor UV | Float for storing the voltage values of the UV sensor
float UVsensorValue; //Float para almacenar la lectura analógica del sensor UV | Float for storing the analog read of the UV sensor
float bmp_T; //Float para almacenar los valores de temperatura del BMP | Float for storing the temperature values from the BMP
float bme_T; //Float para almacenar los valores de temperatura del BME | Float for storing the temperature values from the BME
float bme_H; //Float para almacenar los valores de humedad del BME | Float for storing the humidity values from the BME

//--------------------------------------------------------------------------

Adafruit_CCS811 ccs; // Crear objeto CCS811 | Create CSS811 object
BME280_I2C bme (0x76);  // Crear objeto BME280 con dirección 0x76 | Create BME280 object with 0x76 address
Adafruit_BMP280 bmp;  // Crear objeto BMP280 con dirección 0x77 | Create BMP280 object with 0x77 address

//--------------------------------------------------------------------------

void setup() {
 
  Serial.begin(9600); //Iniciar el puerto serie por defecto | Start the default serial port
  Tarjeta.begin(9600);  //Iniciar el puerto serie para el OpenLog | Start the OpenLog serial port

//--------------------------------------------------------------------------

  pinMode(4, OUTPUT); //Configurar el puerto 4 como salida (led) | Configure the port 4 like an output (led)

//--------------------------------------------------------------------------

  Serial.println("Iniciando..."); //Imprimir estado en el puerto serie por defecto (APC) | Print status on the default serial port (APC)
  Tarjeta.println("Iniciando..."); //Imprimir estado en el puerto serie del OpenLog (SD) | Print status on the OpenLog serial port (SD)

//--------------------------------------------------------------------------

  if (!bme.begin()) {
      
    Serial.println("No se ha encontrado el sensor BME280, comprobar conexiones"); //Avisar de que no se ha encontrado el BME280 (APC) | Warn about BME280 failure (APC)
    Tarjeta.println("No se ha encontrado el sensor BME280, comprobar conexiones"); //Avisar de que no se ha encontrado el BME280 (SD) | Warn about BME280 failure (SD)
    delay(5000);

  }

//--------------------------------------------------------------------------

  if (!bmp.begin()) {
    
    Serial.println(F("No se ha encontrado el sensor BMP280, comprobar conexiones"));  //Avisar de que no se ha encontrado el BMP280 (APC) | Warn about BMP280 failure (APC)
    Tarjeta.println(F("No se ha encontrado el sensor BMP280, comprobar conexiones")); //Avisar de que no se ha encontrado el BMP280 (SD) | Warn about BMP280 failure (SD)
    delay(5000);

  }

//--------------------------------------------------------------------------
  
  if(!ccs.begin()){
      
    Serial.println("No se ha encontrado el sensor CCS811, comprobar conexiones"); //Avisar de que no se ha encontrado el CCS811 (APC) | Warn about CCS811 failure (APC)
    Tarjeta.println("No se ha encontrado el sensor CCS811, comprobar conexiones"); //Avisar de que no se ha encontrado el CCS811 (SD) | Warn about CCS811 failure (SD)
    delay(5000);

  }

//--------------------------------------------------------------------------

  Serial.println("ident, Seg, CO2, COV, PresBME, PresBMP, Humed, TempBME, TempBMP, UV");
  Tarjeta.println("ident, Seg, CO2, COV, PresBME, PresBMP, Humed, TempBME, TempBMP, UV");
  
}

//--------------------------------------------------------------------------

void loop() {

  String data; //Crear una cadena para el envio de datos | Create a String for the data

//--------------------------------------------------------------------------

  digitalWrite(4, HIGH); // Encender el led | Power on the led

//--------------------------------------------------------------------------

  segundo = millis(); //Guardar el tiempo actual en la float "segundo" | Store the actual time on the "segundo" float.
  i = i + 1; //Aumentar el identificador de dato | Increase the data identifier

//--------------------------------------------------------------------------

  if(ccs.available()){
    if(!ccs.readData()){
      
      co2 = ccs.geteCO2(); //Almacenar el valor de CO2 | Store the CO2 value
      cov = ccs.getTVOC(); //Almacenar el valor de COV | Store the COV value

    }
  }

//--------------------------------------------------------------------------

  bme.readSensor();
  bmp_Pa = bmp.readPressure() / 100; //Almacenar la presion del BMP | Store the pressure value from the BMP
  bme_Pa = bme.getPressure_MB() * 100; //Almacenar la presion del BME | Store the pressure value from the BME
  bme_H = bme.getHumidity(); //Almacenar el porcentaje de humedad ambiente | Store the humidity value
  bme_T = bme.getTemperature_C(); //Almacenar la temperatura del BME (celsius) | Store the celsius temperature value of the BME
  bmp_T = bmp.readTemperature(); //Almacenar la temperatura del BMP (celsius) | Store the celsius temperature value of the BMP

//--------------------------------------------------------------------------
 
  UVsensorValue = analogRead(A0); //Leer el valor analógico del sensor UV | Read the analog value of the UV sensor
  UVsensorVoltage = UVsensorValue/1024*5; //Convertir el valor analogico en voltaje | Convert the analog value on voltage

//--------------------------------------------------------------------------
//
// Añadimos todos los datos al String | Add all data to the String
//

  data += String(i, 2);
  data += F(",");
  data += String(segundo, 2);
  data += F(",");
  data += String(co2, 2);
  data += F(",");
  data += String(cov, 2);
  data += F(",");
  data += String(bme_Pa, 2);
  data += F(",");
  data += String(bmp_Pa, 2);
  data += F(",");
  data += String(bme_H, 2);
  data += F(",");
  data += String(bme_T, 2);
  data += F(",");
  data += String(bmp_T, 2);
  data += F(",");
  data += String(UVsensorVoltage, 2);

//--------------------------------------------------------------------------

  Tarjeta.println(data); //Guardar datos en la SD | Send data to the SD Card
  Serial.println(data); //Enviar datos por el APC | Send data through the APC
 
  delay(100); //Esperar por 100ms | Wait for 100ms

}
