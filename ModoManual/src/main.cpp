//************************
//****** Librerias *******
//************************

#include <SoftwareSerial.h>
#include <Arduino.h>
#include <Servo.h>

//************************
//******** Pines *********
//************************

//int R = 0; // pin del led rojo
//int G = 0; // pin del led verde
//int B = 0; // pin del led azul

int IR = A0; // pin Infrarrojo

int servoVert = D1; // pin del servo de control vertical
int servoHorz = D0; // pin del servo que actua como puerta entrada de pelotas
int servo3 = D4;

int RXBT = D3; // pin RX del Bluetooth 
int TXBT = D2; // pin TX del Bluetooth 

int Motor1 = D6;
int Motor2 = D8;

//************************
//****** Variables *******
//************************

int pos = 0;
int ValorBT = 0;
int incremento = 0;
int puerta = 0;
int est = 0;
int vel = 70;
int estado = 0;
int valorVer = 0;
int valorHor = 0;
int valorMot = 0;
bool dato = 1;

//*************************
//*** Config. Librerias ***
//*************************

Servo Vert; // servo de control vertical
Servo Horz; // servo de puerta entrada de pelotas

SoftwareSerial BT(RXBT, TXBT); // RX | TX Bluetooth

//*************************
//******** Void's *********
//*************************

void sweep();
void dance();

//************************
//******** Setup *********
//************************

void setup(){
  Serial.begin(9600);
  BT.begin(9600);
  
  Vert.attach (servoVert, 544, 2400);
  Horz.attach (servoHorz, 544, 2400); 

  Vert.write(0);
  Horz.write(0); // Todos los servos en posicion inicial

//  pinMode(B, OUTPUT); // led azul
//  pinMode(G, OUTPUT); // led verde
//  pinMode(R, OUTPUT); // led rojo

  pinMode(IR, INPUT); // Infrarrojo

  pinMode(Motor1, OUTPUT);
  pinMode(Motor2, OUTPUT);
  Horz.write(90);
  Vert.write(90);
  
  digitalWrite(Motor1, HIGH);
  digitalWrite(Motor1, LOW);
  delay(300);
  digitalWrite(Motor2, HIGH);
  digitalWrite(Motor2, LOW);
  BT.read();
}
void loop(){
  Serial.println(estado);
  switch (estado){
    case 0:
        BT.println("Vertical: ");
        while(!BT.available());
        
        ValorBT = BT.parseInt();
        BT.read();
        if(ValorBT >= 1 && ValorBT <= 91){
          Vert.write(ValorBT);
          valorVer = ValorBT;
          estado++;
        }
        break;

    case 1:
      BT.println("Horizontal: ");
      while(!BT.available());
      
      ValorBT = BT.parseInt();
      BT.read();
      if(ValorBT >= 1 && ValorBT <= 91){
        Horz.write(ValorBT);
        valorHor = ValorBT;
        estado++;
      }
      break;
    
    case 2:
      BT.println("Velocidad: ");
      while(!BT.available());
      
      ValorBT = BT.parseInt();
      BT.read();
      if(ValorBT >= 1 && ValorBT <= 256){
        valorMot = ValorBT;
        BT.println("Preparando lanzamiento...");
        analogWrite(Motor1, 255);
        analogWrite(Motor2, 255);
        delay(500);
        estado++;
      }
      break;

    case 3:
      analogWrite(Motor1, valorMot);
      analogWrite(Motor2, valorMot);
      delay(1000);
      BT.println("---------------------");
      BT.println("Preparado para lanzar!");
      BT.print("Servo Vertical: ");
      BT.print(valorVer);
      BT.println("°");
      BT.print("Servo Horizontal: ");
      BT.print(valorHor);
      BT.println("°");
      int velocidad = map(valorMot, 0, 255, 0, 100);
      BT.print("Motor: ");
      BT.print(velocidad);
      BT.print("%");
      BT.print("  (");
      BT.print(valorMot);
      BT.println(")");
      delay(1000);
      BT.print("5 ");
      delay(1000);
      BT.print("4 ");
      delay(1000);
      BT.print("3 ");
      delay(1000);
      BT.print("2 ");
      delay(1000);
      BT.println("1 ");
      BT.println("---------------------");
      digitalWrite(Motor1, LOW);
      digitalWrite(Motor2, LOW);
      Vert.write(90);
      Horz.write(90);
      estado = 0;
      break;

  }
}
    /*switch (ValorBT){
      case 0:
      break;

      case 1:
        Vert.write(pos);
        BT.println("Servo posicionado a: 0 grados");
        break;
  
      case 2:
        Vert.write(10);
        BT.println("Servo posicionado a: 10 grados");
        break;
    
      case 3:
        Vert.write(20);
        BT.println("Servo posicionado a: 20 grados");
        break;

      case 4:
        Vert.write(30);
        BT.println("Servo posicionado a: 30 grados");
        break;

      case 5:
        Vert.write(40);
        BT.println("Servo posicionado a: 40 grados");
        break;

      case 6:
        Vert.write(50);
        BT.println("Servo posicionado a: 50 grados");
        break;

      case 7:
        Vert.write(60);
        BT.println("Servo posicionado a: 60 grados");
        break;
      
      case 8:
        Vert.write(70);
        BT.println("Servo posicionado a: 70 grados");
        break;
      
      case 9:
        Vert.write(80);
        BT.println("Servo posicionado a: 80 grados");
        break;

      case 10:
        Vert.write(90);
        BT.println("Servo posicionado a: 90 grados");
        break;

      case 11:
        Vert.write(100);
        BT.println("Servo posicionado a: 100 grados");
        break;

      case 12:
        Vert.write(110);
        BT.println("Servo posicionado a: 110 grados");
        break;

      case 13:
        Vert.write(120);
        BT.println("Servo posicionado a: 120 grados");
        break;

      case 14:
        Vert.write(130);
        BT.println("Servo posicionado a: 130 grados");
        break; 

      case 15:
        Vert.write(140);
        BT.println("Servo posicionado a: 140 grados");
        break; 

      case 16:
        Vert.write(150);
        BT.println("Servo posicionado a: 150 grados");
        break;  

      case 17:
        Vert.write(160);
        BT.println("Servo posicionado a: 160 grados");
        break;

      case 18:
        Vert.write(170);
        BT.println("Servo posicionado a: 170 grados");
        break;
          
      case 19:
        Vert.write(180);
        BT.println("Servo posicionado a: 180 grados");
        break;

      case 20: // sweep
        BT.print("Escaneando, espere un momento");
        sweep();
        BT.print(" -ESCANEO COMPLETADO-");
        break;

      case 21: // dance
        BT.print("Quiere que le baile? ");
        dance();
        BT.print("ok. suficiente.");
        break;
      
      case 22:
        Horz.write(180);
        puerta = 1;
        BT.print("puerta abierta");
        break;

      case 23:
        Horz.write(0);
        puerta = 0;
        BT.print("puerta cerrada");
        break;

      case 24:
        if(puerta == 0){
          Horz.write(180);
          BT.print("puerta abierta");
          puerta = 1;
        }
        else{
          Horz.write(0);
          BT.print("puerta cerrada");
          puerta = 0;
        }
        break;

      case 25:
        Horz.write(100);
        analogWrite(Motor1, 255);
        analogWrite(Motor2, 255);
        delay(300);
        analogWrite(Motor1, vel);
        analogWrite(Motor2, vel);
        BT.println("lanzamiento medio");
        delay(5000);
        digitalWrite(Motor1, LOW);
        digitalWrite(Motor2, LOW);
        break;
        Horz.write(125);
        delay(100);
        analogWrite(EN1, vel);
        digitalWrite(EN2, LOW);
        analogWrite(EN3, vel);
        digitalWrite(EN4, LOW);
        BT.print("BOOM");
        delay(1000);
        digitalWrite(EN1, LOW);
        digitalWrite(EN2, LOW);
        digitalWrite(EN3, LOW);
        digitalWrite(EN4, LOW);
        Horz.write(90);
        delay(100);
        break;

      case 26:
        Horz.write(140);
        analogWrite(Motor1, 255);
        analogWrite(Motor2, 255);
        delay(300);
        analogWrite(Motor1, vel);
        analogWrite(Motor2, vel);
        BT.println("lanzamiento medio");
        delay(5000);
        digitalWrite(Motor1, LOW);
        digitalWrite(Motor2, LOW);
        Horz.write(140);
        delay(100);
        analogWrite(EN1, vel);
        digitalWrite(EN2, LOW);
        analogWrite(EN3, vel);
        digitalWrite(EN4, LOW);
        BT.print("BOOM");
        delay(1000);
        digitalWrite(EN1, LOW);
        digitalWrite(EN2, LOW);
        digitalWrite(EN3, LOW);
        digitalWrite(EN4, LOW);
        Horz.write(90);
        delay(100);
        break;

      case 30:
        analogWrite(Motor1, vel);
        analogWrite(Motor2, vel);
        BT.println("ON");
        analogWrite(EN1, vel);
        analogWrite(EN2, 0);
        analogWrite(EN3, vel);
        analogWrite(EN4, 0);
        break;

      case 31:
        digitalWrite(Motor1, LOW);
        digitalWrite(Motor2, LOW);
        BT.println("OFF");
        digitalWrite(EN1, LOW);
        digitalWrite(EN2, LOW);
        digitalWrite(EN3, LOW);
        digitalWrite(EN4, LOW);
        break;

      default:
        BT.println("Inserte un valor valido...");
        break;
    }*/
//}
  
/* -----> INFRARROJO
  int interruptIR = digitalRead(IR);
//  digitalWrite(R, interruptIR);

  if(est == 0){
    if (interruptIR == 1){
      BT.print("Pelota detectada");
      est = 1;
    }
  }
  if(est == 1){
    if(interruptIR == 0){
      BT.print("Pelota lanzada");
      est = 0;
    }
  }
}
/* -----> VOID DANCE / SWEEP

//************************
//******** Sweep *********
//************************

void sweep(){
  for (pos = 0; pos <= 180; pos += 1) {
    Vert.write(pos);
    BT.print(".");
    delay(15); // Tiempo de espera entre incrementos
  }
  
  for (pos = 180; pos >= 0; pos -= 1) {
    Vert.write(pos);
    BT.print(".");
    delay(15); // Tiempo de espera entre incrementos
  }
}

//************************
//******** Dance *********
//************************

void dance(){
  for (int i = 0; i < 5; i++) {
    for (pos = 30; pos <= 150; pos += 30) {
      Vert.write(pos);
      delay(200);
    }
    for (pos = 150; pos >= 30; pos -= 30) {
      Vert.write(pos);
      delay(200);
    }
    BT.print("7u7 ");
  }
}

*/