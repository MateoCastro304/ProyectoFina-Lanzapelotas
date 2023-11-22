#ifndef COMUNICACION_NODEMCU_H
#define COMUNICACION_NODEMCU_H
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>

typedef struct ang{
    float x;
    float y;
}ang;

typedef struct dataWemos{
    bool bat_ok;
    bool hay_golpe;
    bool error;
    int cant_golpes;
    float posicion_golpe_x;
    float posicion_golpe_y;
}dataWemos;

void set_espnow();
void sendMacWemos(uint8_t* structurePointer, int len);
dataWemos getDatosRecibidos();
bool datosNuevos();

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus);
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len);

#endif