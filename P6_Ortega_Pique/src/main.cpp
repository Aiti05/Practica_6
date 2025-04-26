// EJERCICIO 1

/*

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

*/



// EJERCICIO 2

/*

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

*/