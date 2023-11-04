

#include <SoftwareSerial.h>
#include <Arduino.h>


#define SOFT_RX D3
#define SOFT_TX D2
SoftwareSerial Soft_BT(SOFT_RX,SOFT_TX); // TX | RX Bluetooth


void setup(){
  Serial.begin(9600);
  Soft_BT.begin(9600);
  
  Soft_BT.read();
}
void loop(){

  if (Soft_BT.available())
  {
    String DatReceived = Soft_BT.readStringUntil('.');
    Serial.print("Dato recibido: ");
    Serial.println(Soft_BT.read());  
  }

}
