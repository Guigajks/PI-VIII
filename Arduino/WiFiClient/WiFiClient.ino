#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char* ssid     = "";
const char* password = "";

const char* host = "";
const int httpPort = 10000;

WiFiClient client;

String scaneiaRede(int numeroSSID) {
  String retorno = "";
  
  for (int i = 0; i < numeroSSID; i++) {
    if (
      WiFi.SSID(i).length() == 4
      && WiFi.SSID(i).charAt(0) == 'A'
      && WiFi.SSID(i).charAt(2) == 'P'
      && isDigit(WiFi.SSID(i).charAt(1))
      && isDigit(WiFi.SSID(i).charAt(3))
      || WiFi.SSID(i) == "virus"
    ) {
      retorno += WiFi.SSID(i) + String(WiFi.RSSI(i), DEC) + ";";
    }
    else {
      continue;
    }
    //    if (WiFi.SSID(i) == "A1P2") {
    //      return String(WiFi.RSSI(i), DEC);
    //    }
  }
    if (retorno != "")
      retorno = retorno.substring(0, retorno.length() - 1);
  
  return retorno;
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

}

void loop() {

  Udp.beginPacket(host, httpPort);
  Udp.write(scaneiaRede(WiFi.scanNetworks(false, false, 3, NULL)), 2);
  Udp.endPacket();  
  delay(1000);

}
