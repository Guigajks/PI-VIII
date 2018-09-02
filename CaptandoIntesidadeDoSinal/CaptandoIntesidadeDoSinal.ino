#include "ESP8266WiFi.h"

 
void setup()
{
  Serial.begin(115200);  
  WiFi.mode(WIFI_STA);
  Serial.println();
 
}

void scaneiaRede(int numeroSSID){
  int i=0;
  for(i; i<numeroSSID; i++){
    Serial.printf("Rede: %s, Distância em dBs: (%d)\n", WiFi.SSID(i).c_str(),WiFi.RSSI(i));
  }
}
 
void loop()
{
  scaneiaRede(WiFi.scanNetworks());        
  delay(60000);                                
}
