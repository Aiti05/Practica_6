## **Practica 6: Buses de comunicación II (SPI)**

El objetivo de esta práctica es entender el funcionamiento del bus de comunicación SPI (Serial Peripheral Interface), 
ampliamente utilizado en sistemas embebidos por su rapidez y sencillez. El bus SPI permite la comunicación Full Duplex entre 
un dispositivo maestro y uno o varios dispositivos esclavos, utilizando un conjunto de líneas: MOSI, MISO, SCK y SS. Esta práctica, 
enfocada en el microcontrolador ESP32-S3, consiste en realizar dos ejercicios prácticos: la lectura/escritura de una memoria SD 
y la lectura de una etiqueta RFID mediante un módulo MFRC522. Ambos dispositivos utilizan el bus SPI para su comunicación con el 
microcontrolador, lo que permite explorar de forma práctica cómo iniciar, enviar y recibir datos a través de esta interfaz. El conocimiento 
adquirido en esta práctica es esencial para el diseño de sistemas que requieren interconectar múltiples periféricos de alta velocidad.

## **Ejercicio Practico 1 LECTURA/ESCRITURA DE MEMORIA SD:**
**Codigo main.cpp:**
```
#include <SPI.h>
#include <SD.h>


File myFile;


void setup() {
    Serial.begin(115200);
    Serial.print("Iniciando SD...");


    if (!SD.begin(10)) {  // CS en GPIO 10
        Serial.println("No se pudo inicializar");
        return;
    }
    Serial.println("Inicialización exitosa");


    myFile = SD.open("/archivo.txt", FILE_WRITE);
    if (myFile) {
        myFile.println("Hola desde ESP32-S3");
        myFile.close();
        Serial.println("Escritura exitosa");
    } else {
        Serial.println("Error al abrir el archivo");
    }


    myFile = SD.open("/archivo.txt");
    if (myFile) {
        Serial.println("Contenido del archivo:");
        while (myFile.available()) {
            Serial.write(myFile.read());
        }
        myFile.close();
    } else {
        Serial.println("Error al leer el archivo");
    }
}


void loop() {
}

```
En este ejercicio se trabaja con una tarjeta SD utilizando el bus SPI para realizar operaciones básicas de lectura y escritura. El programa 
inicializa la comunicación serie y la tarjeta SD a través del pin GPIO 10 como línea Chip Select (CS). Si la tarjeta se inicializa 
correctamente, se crea o abre un archivo llamado archivo.txt para escribir en él la frase "Hola desde ESP32-S3". Posteriormente, se cierra 
el archivo para asegurar que los datos se guarden correctamente. Luego, se vuelve a abrir el mismo archivo, esta vez en modo lectura, para 
leer su contenido y mostrarlo a través del monitor serie. Todo el proceso ocurre en la función setup(), ya que el bucle loop() no realiza 
ninguna operación en este caso. Este ejercicio introduce de manera sencilla el manejo de archivos en una memoria SD utilizando el bus SPI.



## **Ejercicio Practico 2 LECTURA DE ETIQUETA RFID:**
**Codigo main.cpp:**
```
#include <SPI.h>
#include <MFRC522.h>


#define RST_PIN 8    // Pin de reset del RC522
#define SS_PIN  9    // Pin SS (SDA) del RC522


MFRC522 mfrc522(SS_PIN, RST_PIN);  // Crear objeto RFID


void setup() {
    Serial.begin(115200);  // Iniciar la comunicación serial
    SPI.begin();           // Iniciar el bus SPI
    mfrc522.PCD_Init();    // Iniciar el módulo RFID
    Serial.println("Escaneando tarjetas RFID...");
}


void loop() {
    // Verificar si hay una nueva tarjeta presente
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
        Serial.print("UID de tarjeta: ");


        // Leer el UID de la tarjeta y mostrarlo en hexadecimal
        for (byte i = 0; i < mfrc522.uid.size; i++) {
            Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
            Serial.print(mfrc522.uid.uidByte[i], HEX);
        }
        Serial.println();


        mfrc522.PICC_HaltA();  // Detener la lectura de la tarjeta
    }
}



```

Este ejercicio consiste en leer el identificador único (UID) de una tarjeta RFID utilizando el módulo MFRC522 conectado mediante el bus 
SPI. Se configuran los pines de control, se inicializa la comunicación serie y se prepara el módulo para escanear tarjetas. En el bucle 
principal, el programa detecta si una nueva tarjeta RFID se acerca al lector, y si es así, obtiene y muestra su UID en formato hexadecimal 
a través del monitor serie. Tras la lectura, se envía una orden de parada al módulo para poder realizar nuevas detecciones. De esta manera, 
se pone en práctica cómo interactuar con un dispositivo RFID a través de SPI de manera sencilla y efectiva.



