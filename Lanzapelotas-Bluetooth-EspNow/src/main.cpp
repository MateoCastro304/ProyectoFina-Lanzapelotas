#include <Arduino.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#include <espnow.h>
#include <ESP8266WiFi.h>
// put function declarations here:


//******** MACROS *********
#define PIN_SERVO_GATE D4
#define PIN_SERVO_HORZ D0
#define PIN_SERVO_VERT D1

#define PIN_SOFT_RX D3
#define PIN_SOFT_TX D2

#define MOTOR1 D6
#define MOTOR2 D7

#define PIN_IR A0 // pin Infrarrojo

#define OPEN_GATE 0
#define CLOSE_GATE 90
#define TRESHOLD_BALL 100

#define MODE 0
#define POS_VERT 1
#define POS_HORZ 2
#define POT 3
#define DO_LAUNCH 4

//ESP01 MAC: 84:F3:EB:4C:A0:DF
//WEMOS MAC: 3C:61:05:D1:C2:49
//NODEMCU MAC: A4:CF:12:C9:91:48
uint8_t macESP01[] = {0x84, 0xF3, 0xEB, 0x4C, 0xA0, 0xDF};
uint8_t macWemos[] = {0x3C, 0x61, 0x05, 0xD1, 0xC2, 0x49};
uint8_t macNodeMCU[] = {0xA4, 0xCF, 0x12, 0xC9, 0x91, 0x48};
typedef struct ang{
    float x;
    float y;
}ang;
typedef struct dataWemos{
    bool bat_ok;
    bool hay_golpe;
    int cant_golpes;
    ang posicion_golpe;
}dataWemos;

dataWemos dataRecv;

Servo ServoVert; // servo de control vertical
Servo ServoHorz; // servo de puerta entrada de pelotas
Servo ServoGate;// servo de puerta entrada de pelotas
SoftwareSerial Soft_BT(PIN_SOFT_RX, PIN_SOFT_TX); // RX | TX Bluetooth



void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
    Serial.print("Sending... Delivery: ");
    Serial.println(!sendStatus ? "Success":"Fail");
}
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
    Serial.println("DATA RECEIVE");
    memcpy(&dataRecv, incomingData, sizeof(dataRecv));

    Serial.println((dataRecv.hay_golpe) ? "Golpe Efectuado":"");
    String _buff = "Bytes received: "+ String(len) + "\nGolpes: "+ String(dataRecv.cant_golpes);
    Serial.println(_buff);
    delay(5);
    Serial.print("Battery OK: ");
    Serial.println((dataRecv.bat_ok) ? "True":"False");
    
    _buff = "Ang Golpe:     X:  "+ String(dataRecv.posicion_golpe.x) + "     Y: "+ String(dataRecv.posicion_golpe.y);
    Serial.println(_buff);
    delay(5);
}
void setServos(int vert_value, int horz_value){
  ServoHorz.write(horz_value);
  ServoVert.write(vert_value);
}
void Launch(int pot, int vert_value, int horz_value){
  int vel = map(pot,0,100,0,255);

  ServoHorz.write(horz_value);
  ServoVert.write(vert_value);

  analogWrite(MOTOR1,250);
  analogWrite(MOTOR2,250);

  delay(1000);

  analogWrite(MOTOR1,vel);
  analogWrite(MOTOR2,vel);
  /*
  abrir compuerta
  esperar a que la pelota caiga
  cerrar la puerta
  esperar un tiempo determinado que será lo que aproximadamente tarda en lanzarse
  */
  delay(5000);

  analogWrite(MOTOR1,0);
  analogWrite(MOTOR2,0);
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
  
  ServoVert.attach(PIN_SERVO_VERT, 544, 2400);
  ServoHorz.attach(PIN_SERVO_HORZ, 544, 2400); 
  ServoGate.attach(PIN_SERVO_GATE,544,2400);
  
  ServoGate.write(0);
  ServoVert.write(90);
  ServoHorz.write(90); // Todos los servos en posicion inicial

  pinMode(PIN_IR, INPUT); // Infrarrojo

  pinMode(MOTOR1, OUTPUT);
  pinMode(MOTOR2, OUTPUT);

  Soft_BT.read();

  //ESPMOW
  WiFi.mode(WIFI_STA);
  
  Serial.println(!esp_now_init() ? "ESPNOW Iniciado correctamente":"Error al iniciar ESPNOW");

  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);
    //ESPNOW PEER CONFIG
  Serial.println((!esp_now_add_peer(macWemos, ESP_NOW_ROLE_COMBO, 1, NULL, 0)) ? "Peer añadido":"Peer no añadido");
  Serial.println((esp_now_is_peer_exist(macWemos) > 0) ? "Wemos Encontrada":"No se encontró Wemos");
}

void loop() {
  const int dataLenght = 5;
  static int data[dataLenght];
  static bool data_receive=false;

  if (Soft_BT.available()){
    String str = Soft_BT.readStringUntil('.');
    receiveDataBT(str, data, dataLenght);

    String _buff = "Mode:" + String(data[MODE]) + " Vert:" + String(data[POS_VERT]) + " Horz:" + String(data[POS_HORZ]) + " Pot:" + String(data[POT]) + " Launch:" + String(data[DO_LAUNCH]);
    Serial.println(_buff);
    Soft_BT.read();

    data_receive=true;
  }

  if (data_receive)
  {
    if (data[4]){
      Launch(data[POT],data[POS_VERT],data[POS_HORZ]);
    }
    else{
      setServos(data[POS_VERT],data[POS_HORZ]);
    }
    
    data_receive = false;
  }
}

