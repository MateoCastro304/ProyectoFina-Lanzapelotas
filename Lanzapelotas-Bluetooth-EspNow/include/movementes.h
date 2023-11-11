#ifndef MOVEMENTES_H
#define MOVEMENTES_H
#include <Arduino.h>
#include <Servo.h>

#define PIN_SERVO_GATE D4
#define PIN_SERVO_HORZ D0
#define PIN_SERVO_VERT D1

#define MOTOR1 D6
#define MOTOR2 D7

#define PIN_IR A0 // pin Infrarrojo

#define OPEN_GATE 80
#define CLOSE_GATE 110
#define TRESHOLD_BALL 100

void setPeripherals();

void writeServos(int vert_value, int horz_value);

void Launch(int pot, int vert_value, int horz_value);

void watiForBall();

#endif