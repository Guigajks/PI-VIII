#include "ESP8266WiFi.h"
#define pinoLED D7
#define pinoSensor D1
 
void setup()
{
  Serial.begin(115200);  
  WiFi.mode(WIFI_STA);
  Serial.println();
  pinMode(pinoLED, OUTPUT);
  pinMode(pinoSensor, INPUT);  
  WiFi.mode(WIFI_AP);
  WiFi.softAP("P1");
 
}

void scaneiaRede(int numeroSSID){
  int i=0;
  for(i; i<numeroSSID; i++){
    Serial.printf("Rede: %s, Distância em dBs: (%d)\n", WiFi.SSID(i).c_str(),WiFi.RSSI(i));
  }
}

void sensorPresenca(int recebeValor){
  digitalWrite(pinoLED, LOW);
  Serial.println(recebeValor);
  if(recebeValor == 1){
    digitalWrite(pinoLED, HIGH);
  }
}
 
void loop()
{
  sensorPresenca(digitalRead(pinoSensor));
  //scaneiaRede(WiFi.scanNetworks());        
  delay(500);                                
}
