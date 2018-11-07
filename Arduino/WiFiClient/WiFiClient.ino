#include <ESP8266WiFi.h>

const char* ssid     = "";
const char* password = "";

const char* host = "192.168.0.106";
const int httpPort = 10000;

bool pareado = false;

WiFiClient client;

String scaneiaRede(int numeroSSID) {
  String retorno = "";

  int i = 0;
  for (i; i < numeroSSID; i++) {
    if (
      WiFi.SSID(i).length() == 4
      && WiFi.SSID(i).charAt(0) == 'A'
      && WiFi.SSID(i).charAt(2) == 'P'
      && isDigit(WiFi.SSID(i).charAt(1))
      && isDigit(WiFi.SSID(i).charAt(3))
    ) {
      retorno += WiFi.SSID(i) + ":" + String(WiFi.RSSI(i), DEC) + ";";
    }
  }

  if (retorno != "")
    retorno = retorno.substring(0, retorno.length() - 1);

  return retorno;
}

bool conectar(char* host, int porta) {
  pareado = client.connect(host, porta); 
  return pareado;
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

  // Use WiFiClient class to create TCP connections

  if (!pareado) {
    pareado = conectar(host, httpPort);
  }
  else{
    client.print(WiFi.macAddress() + ">" + scaneiaRede(WiFi.scanNetworks()));
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }

  }
  // if (!client.connect(host, httpPort)) {
  //   Serial.println("connection failed");
  //   return;
  // }
  
  // This will send the request to the server
  
  // unsigned long timeout = millis();

  
  Serial.println();
  Serial.println("closing connection");
}

