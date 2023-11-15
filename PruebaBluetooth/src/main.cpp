

#include <SoftwareSerial.h>
#include <Arduino.h>


#define SOFT_RX D3
#define SOFT_TX D2
SoftwareSerial Soft_BT(SOFT_RX,SOFT_TX); // TX | RX Bluetooth

unsigned long currentMillis = 0;
const int interval = 10000;

void setup(){
  Serial.begin(9600);
  Soft_BT.begin(9600);
  
  Soft_BT.read();
}
void loop(){
  char data[12];
  float angX,angY;
  char strAngX[5],strAngY[5];
  dtostrf(angX,5,2,strAngX);
  dtostrf(angY,5,2,strAngY);
  if ((millis()-currentMillis) > interval)
  {
    currentMillis = millis();
    sprintf(data,"%s,%s",strAngX,strAngY);
    Soft_BT.println(data);

  }
  String ola;
  ola.
  


  if (Soft_BT.available())
  {
    String DatReceived = Soft_BT.readStringUntil('.');
    Serial.print("Dato recibido: ");
    Serial.println(DatReceived);
  }


}
