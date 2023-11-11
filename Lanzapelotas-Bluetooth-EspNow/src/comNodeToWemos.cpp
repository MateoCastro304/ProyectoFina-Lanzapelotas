#include "comNodeToWemos.h"

//ESP01 MAC: 84:F3:EB:4C:A0:DF
//WEMOS MAC: 3C:61:05:D1:C2:49
//NODEMCU MAC: A4:CF:12:C9:91:48
uint8_t macESP01[] = {0x84, 0xF3, 0xEB, 0x4C, 0xA0, 0xDF};
uint8_t macWemos[] = {0x3C, 0x61, 0x05, 0xD1, 0xC2, 0x49};
uint8_t macNodeMCU[] = {0xA4, 0xCF, 0x12, 0xC9, 0x91, 0x48};

dataWemos dataRecv;

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
void set_espnow(){
    WiFi.mode(WIFI_STA);
    
    Serial.println(!esp_now_init() ? "ESPNOW Iniciado correctamente":"Error al iniciar ESPNOW");

    esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
    esp_now_register_send_cb(OnDataSent);
    esp_now_register_recv_cb(OnDataRecv);
        //ESPNOW PEER CONFIG
    Serial.println((!esp_now_add_peer(macWemos, ESP_NOW_ROLE_COMBO, 1, NULL, 0)) ? "Peer añadido":"Peer no añadido");
    Serial.println((esp_now_is_peer_exist(macWemos) > 0) ? "Wemos Encontrada":"No se encontró Wemos");
}
void sendMacWemos(uint8_t* structurePointer, int len){
    esp_now_send(macWemos,structurePointer, len);
}