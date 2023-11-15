
#include "movementes.h"

Servo ServoVert; // servo de control vertical
Servo ServoHorz; // servo de puerta entrada de pelotas
Servo ServoGate;// servo de puerta entrada de pelotas
bool lanzamientoCompletado = false;
bool getLanzamientoCompletado(){
    bool _lanzamientoCompletado = lanzamientoCompletado;
    lanzamientoCompletado = false;

    return _lanzamientoCompletado;
}
void setPeripherals(){
    ServoVert.attach(PIN_SERVO_VERT, 544, 2400);
    ServoHorz.attach(PIN_SERVO_HORZ, 544, 2400); 
    ServoGate.attach(PIN_SERVO_GATE,544,2400);
    
    ServoGate.write(CLOSE_GATE);
    ServoVert.write(90);
    ServoHorz.write(90); // Todos los servos en posicion inicial

    pinMode(PIN_IR, INPUT); // Infrarrojo

    pinMode(MOTOR1, OUTPUT);
    pinMode(MOTOR2, OUTPUT);

    Serial.println("Perifericos seteados");
}

void writeServos(int vert_value, int horz_value){
  ServoHorz.write(horz_value);
  ServoVert.write(vert_value);
}
void watiForBall(){
    while (analogRead(PIN_IR) > TRESHOLD_BALL){
        delay(10);
    }
}
void Launch(int pot, int vert_value, int horz_value){
    lanzamientoCompletado = false;
    int vel = map(pot,0,100,0,255);

    writeServos(vert_value, horz_value);

    analogWrite(MOTOR1,250);
    analogWrite(MOTOR2,250);

    delay(600);
        
    analogWrite(MOTOR1,vel);
    analogWrite(MOTOR2,vel);

    delay(500);

    ServoGate.write(OPEN_GATE);
    watiForBall();

    ServoGate.write(CLOSE_GATE);
    lanzamientoCompletado = true;
    
    delay(600);
    analogWrite(MOTOR1,0);
    analogWrite(MOTOR2,0);
}