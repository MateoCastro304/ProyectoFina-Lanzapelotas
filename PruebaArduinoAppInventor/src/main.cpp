#include <Arduino.h>
#include <SoftwareSerial.h>

#define PIN_SOFT_RX A0
#define PIN_SOFT_TX A1

SoftwareSerial Soft_BT(PIN_SOFT_RX, PIN_SOFT_TX); // RX | TX Bluetooth

void setup() {
    Serial.begin(9600);
    Soft_BT.begin(9600);
    Soft_BT.read();
}

void loop() {
    if (Serial.available())     
    {
        String serialdata = Serial.readStringUntil('\n');
        Soft_BT.print(serialdata);
        Serial.read(); 
    }
    if (Soft_BT.available())
    {
        String btdata = Soft_BT.readStringUntil('.');
        Serial.println(btdata);
        
    }
}

