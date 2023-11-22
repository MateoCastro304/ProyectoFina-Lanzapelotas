#include <Arduino.h>
#include <SoftwareSerial.h>

#define PIN_SOFT_RX A0
#define PIN_SOFT_TX A1

SoftwareSerial Soft_BT(PIN_SOFT_RX, PIN_SOFT_TX); // RX | TX Bluetooth
String formatFloat(float value, uint8_t totalWidth, int decimalPlaces) {
    String formatted = "";
    if (value < 0) {
        formatted = String(value, decimalPlaces);
        formatted = formatted.substring(1);
        while (formatted.length() < totalWidth-1){
            formatted = '0' + formatted;
        }
        formatted = '-' + formatted;
    }else{
        formatted = String(value, decimalPlaces);
        while(formatted.length() < totalWidth) {
            formatted = '0' + formatted;
        }
    }
  return formatted;
}

void sendDataBT(float angX, float angY){

    String strAngX = String(angX,2);//Formatear floats para que tengan un largo de 7 (0000.00 o -000.00)
    String strAngY = String(angY,2);
    //Serial.println(strAngX.c_str());
    //Serial.println(strAngY.c_str());
    static bool dat = false;
    dat =! dat;
    char data[19];
    sprintf(data,"%s,%s,%d",strAngY.c_str(),strAngX.c_str(),dat);
    Serial.println(data);
    //Serial.println(strAngX);
    //Serial.println(strAngY);
    Soft_BT.print(data);
}


void setup() {
    Serial.begin(9600);
    Soft_BT.begin(9600);
    Soft_BT.read();
    Serial.println("");
    
}

void loop() {
    while (!Serial.available());
    float angX = Serial.parseFloat();
    Serial.println(angX);
    Serial.read();

    while (!Serial.available());
    float angY = Serial.parseFloat();
    Serial.println(angY);
    Serial.read();

    sendDataBT(angX,angY);

    if (Soft_BT.available())
    {
        String btdata = Soft_BT.readStringUntil('.');
        Serial.println(btdata);
        
    }
}

