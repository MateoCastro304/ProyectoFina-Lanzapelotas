#include <Arduino.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#include <espnow.h>
#include <ESP8266WiFi.h>
// put function declarations here:
//************************
//******** Pines *********
//************************
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
//************************
//****** Variables *******
//************************
int pos = 0;
int ValorBT = 0;
int incremento = 0;
int puerta = 0;
int est = 0;
int vel = 70;

int valorVer = 0;
int valorHor = 0;
int valorMot = 0;
bool dato = 1;
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

void setup() {
  Serial.begin(9600);
  Soft_BT.begin(9600);
  
  ServoVert.attach(PIN_SERVO_VERT, 544, 2400);
  ServoHorz.attach(PIN_SERVO_HORZ, 544, 2400); 
  ServoGate.attach(PIN_SERVO_GATE,544,2400);
  
  ServoGate.write(0);
  ServoVert.write(0);
  ServoHorz.write(0); // Todos los servos en posicion inicial

  pinMode(PIN_IR, INPUT); // Infrarrojo

  pinMode(MOTOR1, OUTPUT);
  pinMode(MOTOR2, OUTPUT);

  ServoHorz.write(90);
  ServoVert.write(90);
  
  digitalWrite(MOTOR1, HIGH);
  digitalWrite(MOTOR1, LOW);
  delay(300);
  digitalWrite(MOTOR2, HIGH);
  digitalWrite(MOTOR2, LOW);
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
  Soft_BT.println("Elija la opcion: ");
  Soft_BT.println("(0) Manual\n(1)Automático");
  while(!Soft_BT.available());
  int opc = Soft_BT.parseInt();
  Soft_BT.read();

  if (opc == 1)
  {
    delay(4000);
    Soft_BT.read();
    while(!Soft_BT.available()){
      int numAleatorio = random(65, 116); //65, 116 PING PONG
      // Controlar el servo ServoHorz con el valor aleatorio
      ServoVert.write(60);
      // Mantener ServoVert en 60
      ServoHorz.write(numAleatorio);
      // Establecer la velocidad de los motores PWM
      // Enviar los datos a través del puerto serie del Bluetooth
      Soft_BT.print("ServoHorz: ");
      Soft_BT.print(numAleatorio);
      Soft_BT.print(", ServoVert: 60, Velocidad Motores: 50");
      Soft_BT.println();  // Agregar un salto de línea
      analogWrite(MOTOR1, 255);
      analogWrite(MOTOR2, 255);
      delay(800);

      analogWrite(MOTOR1, 60);
      analogWrite(MOTOR2, 60);

      ServoGate.write(OPEN_GATE);
      
      int adc_ir = analogRead(PIN_IR);
      while (adc_ir < TRESHOLD_BALL){

        Soft_BT.println("Valor Infrarrojo ADC: ");
        Soft_BT.println(adc_ir);

        adc_ir = analogRead(PIN_IR);
      }

      ServoGate.write(CLOSE_GATE);

      delay(3000);
      analogWrite(MOTOR1, 0);
      analogWrite(MOTOR2, 0);

      delay(5000);
    }
  }
  else if (opc == 0){
    Soft_BT.read();
    Soft_BT.flush();
    Soft_BT.println("Vertical");
    do{
      Soft_BT.println("Ingrese un valor entre 1 y 181: ");
      while(!Soft_BT.available());
      ValorBT = Soft_BT.parseInt();
      Soft_BT.read();
    }while (ValorBT < 1 || ValorBT > 181);
    valorVer = ValorBT;

    Soft_BT.read();
    Soft_BT.flush();
    Soft_BT.println("Horizontal");
    do{
      Soft_BT.println("Ingrese un valor entre 1 y 181: ");
      while(!Soft_BT.available());
      ValorBT = Soft_BT.parseInt();
      Soft_BT.read();
    }while (ValorBT < 1 || ValorBT > 181);
    valorHor = ValorBT;

    Soft_BT.read();
    Soft_BT.flush();
    Soft_BT.println("Velocidad");
    do{
      Soft_BT.println("Ingrese un valor entre 1 y 255: ");
      while(!Soft_BT.available());
      ValorBT = Soft_BT.parseInt();
      Soft_BT.read();
    }while (ValorBT < 1 || ValorBT > 255);
    valorMot = ValorBT;

    Soft_BT.println("Preparando lanzamiento...");

    ServoHorz.write(valorHor);
    ServoVert.write(valorVer);
    delay(1000);
    analogWrite(MOTOR1, 255);
    analogWrite(MOTOR2, 255);
    delay(500);
    analogWrite(MOTOR1, valorMot);
    analogWrite(MOTOR2, valorMot);
    delay(1000);

    Soft_BT.println("---------------------");
    Soft_BT.println("Preparado para lanzar!");

    String dataServoVert = "Servo Vertical: " + String(valorVer) + "°";
    String dataServoHorz = "Servo Horizontal: " + String(valorHor) + "°";
    String dataMotor = "Motor: " + String(map(valorMot, 0, 255, 0, 100)) + "% (" + String(valorMot) + ")";

    Soft_BT.println(dataServoVert);
    Soft_BT.println(dataServoHorz);
    Soft_BT.println(dataMotor);

    for (int i = 5; i > 0; i--)
    {
      delay(1000);
      Soft_BT.print(i);
      Soft_BT.print(" ");
    }

    Soft_BT.println("");
    Soft_BT.println("---------------------");
    
  }
  else{
    Soft_BT.println("Se ingreso una opcion incorrecta");
  }
    digitalWrite(MOTOR1, LOW);
    digitalWrite(MOTOR2, LOW);
    ServoVert.write(90);
    ServoHorz.write(90);

    Soft_BT.read();
    Soft_BT.flush();
}

