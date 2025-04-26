## ** PRACTICA 6: Buses de comunicación II (SPI)**
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


## **Ejercicio de subida de nota ( muy valorado):**

### **Parte 1 - Realizar utilizando el sd y el lector rfid escribiendo en un fichero.log la hora y codigo de cada lectura ( describir como se resuelve el hardware para utilizar un spi para dos perifericos) .:**
```
#include <SPI.h>
#include <SD.h>
#include <MFRC522.h>
#include <Wire.h>
#include <RTClib.h>  // Librería para el reloj en tiempo real

#define RST_PIN 8
#define SS_PIN_RFID 9
#define SS_PIN_SD 10

MFRC522 mfrc522(SS_PIN_RFID, RST_PIN);
RTC_DS3231 rtc;  // Instancia para el RTC
File logFile;

void setup() {
    Serial.begin(115200);
    SPI.begin();

    // Inicializar SD
    if (!SD.begin(SS_PIN_SD)) {
        Serial.println("No se pudo inicializar la tarjeta SD");
        while (1);
    }
    Serial.println("SD inicializada");

    // Inicializar RFID
    mfrc522.PCD_Init();
    Serial.println("Lector RFID inicializado");

    // Inicializar RTC
    if (!rtc.begin()) {
        Serial.println("No se pudo encontrar RTC");
        while (1);
    }
    if (rtc.lostPower()) {
        Serial.println("RTC detenido, ajustar fecha y hora!");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
}

void loop() {
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
        DateTime now = rtc.now();
        String logEntry = "";

        // Construir la entrada de registro
        logEntry += String(now.timestamp(DateTime::TIMESTAMP_FULL));
        logEntry += " UID:";

        for (byte i = 0; i < mfrc522.uid.size; i++) {
            logEntry += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
            logEntry += String(mfrc522.uid.uidByte[i], HEX);
        }
        logEntry += "\n";

        // Escribir en archivo .log
        logFile = SD.open("/lecturas.log", FILE_APPEND);
        if (logFile) {
            logFile.print(logEntry);
            logFile.close();
            Serial.print("Guardado en log: ");
            Serial.print(logEntry);
        } else {
            Serial.println("Error abriendo lecturas.log");
        }

        mfrc522.PICC_HaltA(); // Detener lectura de tarjeta
    }
}


```
En este programa, se utiliza tanto el lector RFID como la tarjeta SD mediante el bus SPI. Para usar dos dispositivos SPI, se asigna un 
pin SS (Slave Select) distinto a cada uno: el pin 9 para el lector RFID y el pin 10 para la SD. Se inicializa el lector RFID, la tarjeta 
SD y un reloj en tiempo real (RTC) que proporciona la fecha y la hora actual.

Cuando se detecta una tarjeta, se obtiene su UID y la hora exacta, generando una cadena de texto que incluye ambos datos. Esta cadena 
se guarda en un archivo llamado lecturas.log en la SD, añadiendo cada nueva lectura al final del archivo. Esto permite tener un registro 
histórico de todas las tarjetas detectadas, junto con el momento preciso de cada lectura.


### **Parte 2 - Generar una pagina web donde se pueda ver la lectura del lector rfid:**
```
#include <SPI.h>
#include <SD.h>
#include <MFRC522.h>
#include <WiFi.h>
#include <WebServer.h>

#define RST_PIN 8
#define SS_PIN_RFID 9
#define SS_PIN_SD 10

const char* ssid = "TU_SSID";
const char* password = "TU_PASSWORD";

MFRC522 mfrc522(SS_PIN_RFID, RST_PIN);
WebServer server(80);

void setup() {
    Serial.begin(115200);
    SPI.begin();

    // Inicializar WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi conectado");
    Serial.println(WiFi.localIP());

    // Inicializar SD
    if (!SD.begin(SS_PIN_SD)) {
        Serial.println("No se pudo inicializar la SD");
        return;
    }

    // Inicializar RFID
    mfrc522.PCD_Init();

    // Configurar servidor web
    server.on("/", handleRoot);
    server.begin();
}

void loop() {
    server.handleClient();

    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
        String uid = "";
        for (byte i = 0; i < mfrc522.uid.size; i++) {
            uid += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
            uid += String(mfrc522.uid.uidByte[i], HEX);
        }
        uid += "\n";

        // Guardar UID en archivo
        File logFile = SD.open("/rfid_log.txt", FILE_APPEND);
        if (logFile) {
            logFile.print(uid);
            logFile.close();
            Serial.println("UID guardado");
        } else {
            Serial.println("Error al guardar UID");
        }

        mfrc522.PICC_HaltA();
    }
}

void handleRoot() {
    File file = SD.open("/rfid_log.txt");
    if (!file) {
        server.send(404, "text/plain", "Archivo no encontrado");
        return;
    }

    String page = "<html><body><h1>Lecturas RFID</h1><pre>";

    while (file.available()) {
        page += (char)file.read();
    }
    page += "</pre></body></html>";

    file.close();
    server.send(200, "text/html", page);
}


```
En este segundo proyecto, el ESP32 actúa como un servidor web. Se conecta a una red WiFi y escucha peticiones HTTP en el puerto 80. 
Cada vez que el lector RFID detecta una tarjeta, guarda el UID en un archivo llamado rfid_log.txt en la tarjeta SD.

Cuando un usuario accede a la dirección IP del ESP32 desde un navegador web, el servidor responde mostrando una página HTML que 
contiene todas las lecturas registradas en rfid_log.txt. De esta forma, es posible ver en tiempo real desde cualquier dispositivo 
conectado a la misma red todas las tarjetas RFID que han sido leídas.


