#include <Arduino.h>

/*
    This sketch establishes a TCP connection to a "quote of the day" service.
    It sends a "hello" message, and then prints received data.
*/

#include <ESP8266WiFi.h>
#include <espnow.h>
#include <ESP8266WiFi.h>
//ESP01 MAC: 84:F3:EB:4C:A0:DF
//WEMOS MAC: 3C:61:05:D1:C2:49
//NODEMCU MAC: A4:CF:12:C9:91:48
uint8_t macESP01[] = {0x84, 0xF3, 0xEB, 0x4C, 0xA0, 0xDF};
uint8_t macWemos[] = {0x3C, 0x61, 0x05, 0xD1, 0xC2, 0x49};
uint8_t macNodeMCU[] = {0xA4, 0xCF, 0x12, 0xC9, 0x91, 0x48};
//const char* host = "djxmmx.net";
//const uint16_t port = 17;
typedef struct ang{
  float x;
  float y;
}ang;
typedef struct data_paddel {
    ang anggolpe;
    float pressure;
    int golpes;
    float altitude;
    int valueBattery;
    uint8_t error;
} data_paddel;

data_paddel myData;

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
    Serial.print("Sending... Delivery: ");
    Serial.println(!sendStatus ? "Success":"Fail");
}
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
    Serial.println("DATA RECEIVE");
    memcpy(&myData, incomingData, sizeof(myData));

    String _buff = "Bytes received: "+ String(len) + "\nGolpes: "+ String(myData.golpes);
    Serial.println(_buff);
    delay(5);
    _buff = "Ang Golpe:     X:  "+ String(myData.anggolpe.x) + "     Y: "+ String(myData.anggolpe.y);
    Serial.println(_buff);
    delay(5);
    _buff = "Battery ADC: "+ String(myData.valueBattery) + "\nAltitude: "+ String(myData.altitude);
    Serial.println(_buff);
    delay(5);
    _buff = "Pressure: "+ String(myData.pressure) + "\nError: "+ String(myData.error);
    Serial.println(_buff);
    delay(5);
    char ask[] = "NodeMCU: Structure Receive";
    esp_now_send(macWemos, (uint8_t *) &ask, sizeof(ask));
  }

#ifndef STASSID
    #define STASSID "Electronica_ALUMNOS"
    #define STAPSK  "alumnosElec2022"
#endif

void setup() {
    Serial.begin(9600);

    Serial.println();
    Serial.println();
  
    // Set device as a Wi-Fi Station
    WiFi.mode(WIFI_STA);
    Serial.println(WiFi.macAddress());
    
    //WiFi.mode(WIFI_STA);
    //WiFi.begin(STASSID, STAPSK);
    //Serial.print("\nConnecting to ");
    //Serial.println(STASSID);

    /*while (WiFi.status() != WL_CONNECTED) {
      Serial.print('.');
      delay(500);
    }*/
    /*Serial.println();
    Serial.print("connected, address=");
    Serial.println(WiFi.localIP());*/

    //Init ESP-NOW
    /*if (esp_now_init() != 0) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }
    else{
      Serial.println("ESP-NOW Initialized");
    }*/
  
    //Once ESPNow is successfully Init, we will register for recv CB to
    //get recv packer info
    /*esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
    esp_now_register_recv_cb(OnDataRecv);

    //Add peer
    esp_now_add_peer(macWemos, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
    esp_now_register_send_cb(OnDataSent);*/
    
    Serial.println(!esp_now_init() ? "ESPNOW Iniciado correctamente":"Error al iniciar ESPNOW");

    esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
    esp_now_register_send_cb(OnDataSent);
    esp_now_register_recv_cb(OnDataRecv);

    //ESPNOW PEER CONFIG
    Serial.println((!esp_now_add_peer(macWemos, ESP_NOW_ROLE_COMBO, 1, NULL, 0)) ? "Peer añadido":"Peer no añadido");
    Serial.println((esp_now_is_peer_exist(macWemos) > 0) ? "Wemos Encontrada":"No se encontró Wemos");
    // esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
    // esp_now_register_recv_cb(OnDataRecv);
    
}

void loop() {
    unsigned char send = 'a';
    esp_now_send(macWemos,&send,sizeof(char));
    delay(3000);
}


