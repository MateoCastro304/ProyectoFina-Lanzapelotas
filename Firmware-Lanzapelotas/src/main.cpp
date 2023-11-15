#include <Arduino.h>
#include <SoftwareSerial.h>
#include <espnow.h>
#include <ESP8266WiFi.h>
// put function declarations here:
#include "movementes.h"
#include "comNodeToWemos.h"

#define PIN_SOFT_RX D3
#define PIN_SOFT_TX D2

#define MODE 0
#define POS_VERT 1
#define POS_HORZ 2
#define POT 3
#define DO_LAUNCH 4

SoftwareSerial Soft_BT(PIN_SOFT_RX, PIN_SOFT_TX); // RX | TX Bluetooth
dataWemos datosPaleta;

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

void sendDataBT(bool launched,ang angulosRotacion){

    String strAngX = formatFloat(angulosRotacion.x, 7, 2); //Formatear floats para que tengan un largo de 7 (0000.00 o -000.00)
    String strAngY = formatFloat(angulosRotacion.y, 7, 2);
    //Serial.println(strAngX.c_str());
    //Serial.println(strAngY.c_str());
    char data[19];
    sprintf(data,"%d,%s,%s",launched,strAngY.c_str(),strAngX.c_str());
    //Serial.println(data);
    //Serial.println(strAngX);
    //Serial.println(strAngY);
}
void receiveDataBT(String str, int *saveDat, int datalenght){
    if (str.charAt(0) == '{' && str.charAt(str.length() - 1) == '}') {
        str = str.substring(str.indexOf('{')+1, str.indexOf('}')); // Elimina las llaves iniciales y finales

        for (int i = 0; i < datalenght; i++)
        {
            int index = str.indexOf(',');
            saveDat[i] = str.substring(0, index).toInt();
            str = str.substring(index + 1);
        }
    }
}

void setup() {
    Serial.begin(9600);
    Soft_BT.begin(9600);
    Soft_BT.read();
    Serial.println("");

    setPeripherals(); 
    //ESPMOW
    set_espnow();
}

void loop() {
    const int dataLenght = 5;
    static int data[dataLenght];
    static bool data_receive=false;
    if (datosNuevos())
    {
        datosPaleta = getDatosRecibidos();
        sendDataBT(getLanzamientoCompletado(),datosPaleta.posicion_golpe);
    }

    if (Soft_BT.available()){
        String str = Soft_BT.readStringUntil('.');
        receiveDataBT(str, data, dataLenght);
       // String _buff = "Mode:" + String(data[MODE]) + " Vert:" + String(data[POS_VERT]) + " Horz:" + String(data[POS_HORZ]) + " Pot:" + String(data[POT]) + " Launch:" + String(data[DO_LAUNCH]);
       // Serial.println(_buff);
        Soft_BT.read();
        data_receive=true;
    }

    if (data_receive)
    {
        if (data[DO_LAUNCH])
        {
            Launch(data[POT],data[POS_VERT],data[POS_HORZ]);
        }
        else{
            writeServos(data[POS_VERT],data[POS_HORZ]);
        }
        data_receive = false;
    }
}