#include <Wire.h> // Libreria necesaria para la comunicación por I2C | Library for I2C communication
#include <SPI.h>  // Libreria necesaria para la comunicación por SPI | Library for SPI communication
#include "cactus_io_BME280_I2C.h" // Libreria necesaria para el BME280 | Library for the BME280
#include <Adafruit_BMP280.h>  // Libreria necesaria para el BMP280 | Library for the BMP280

//--------------------------------------------------------------------------

float segundo; //Añadir un valor para el tiempo | Add a float for the millis()
float segundoold;
float i; //Añadir un identificador de dato | Add a data identificator

//--------------------------------------------------------------------------

float bmp_Pa; //Float para almacenar los valores de presión del BMP | Float for storing the pressure values from the BMP
float bme_Pa; //Float para almacenar los valores de presión del BME | Float for storing the pressure values from the BME

float bmp_T; //Float para almacenar los valores de temperatura del BMP | Float for storing the temperature values from the BMP
float bme_T; //Float para almacenar los valores de temperatura del BME | Float for storing the temperature values from the BME
float bme_H; //Float para almacenar los valores de humedad del BME | Float for storing the humidity values from the BME

//--------------------------------------------------------------------------

BME280_I2C bme (0x76);  // Crear objeto BME280 con dirección 0x76 | Create BME280 object with 0x76 address
Adafruit_BMP280 bmp;  // Crear objeto BMP280 con dirección 0x77 | Create BMP280 object with 0x77 address

//--------------------------------------------------------------------------

void setup() {
 
  Serial.begin(9600); //Iniciar el puerto serie por defecto | Start the default serial port

//--------------------------------------------------------------------------

  Serial.println("Iniciando el termómetro..."); //Imprimir estado en el puerto serie por defecto (APC) | Print status on the default serial port (APC)
  
//--------------------------------------------------------------------------

  if (!bme.begin()) {
      
    Serial.println("No se ha encontrado el sensor BME280, comprobar conexiones"); //Avisar de que no se ha encontrado el BME280 (APC) | Warn about BME280 failure (APC)
    delay(5000);

  }

//--------------------------------------------------------------------------

  if (!bmp.begin()) {
    
    Serial.println(F("No se ha encontrado el sensor BMP280, comprobar conexiones"));  //Avisar de que no se ha encontrado el BMP280 (APC) | Warn about BMP280 failure (APC)
    delay(5000);

  }

//--------------------------------------------------------------------------

  Serial.println("ident, Seg, PresBME, PresBMP, Humed, TempBME, TempBMP"); //Imprimir encabezado con las iniciales para identificar cada valor (APC) | Print header with the initials to identify each value (APC)
}

//--------------------------------------------------------------------------  


void loop() {

  String data; //Crear una cadena para el envio de datos | Create a String for the data
    
//--------------------------------------------------------------------------

  segundoold = segundo;
  segundo = millis(); //Guardar el tiempo actual en la float "segundo" | Store the actual time on the "segundo" float.
  i = i + 1; //Aumentar el identificador de dato | Increase the data identifier
  segundo = segundo/1000; //Convertir los milisegundos a segundo

//--------------------------------------------------------------------------

  bme.readSensor();
  bmp_Pa = bmp.readPressure(); //Almacenar la presion del BMP | Store the pressure value from the BMP
  bme_Pa = bme.getPressure_MB() * 100; //Almacenar la presion del BME | Store the pressure value from the BME
  bme_H = bme.getHumidity(); //Almacenar el porcentaje de humedad ambiente | Store the humidity value
  bme_T = bme.getTemperature_C(); //Almacenar la temperatura del BME (celsius) | Store the celsius temperature value of the BME
  bmp_T = bmp.readTemperature(); //Almacenar la temperatura del BMP (celsius) | Store the celsius temperature value of the BMP


//--------------------------------------------------------------------------
// Añadimos todos los datos al String | Add all data to the String

  data += String(i, 2);
  data += F(",");
  data += String(segundo, 2);
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

//--------------------------------------------------------------------------

  
  Serial.println(data); //Enviar datos por el APC | Send data through the APC
 
  delay(100); //Esperar por 100ms | Wait for 100ms

}
