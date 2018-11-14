/*  Desenvolvido por Alessandro Kantousian
    Data 13/09/2018
    Referências: https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/udp-examples.html

*/
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#define pinoLEDRecBro D6
#define pinoLEDEnvBro D7

WiFiUDP Udp;
unsigned int localUdpPorta = 4500;
char mensagemEntrada[255];
char convMeuMAC[21];
const char* ssid = "familia sarkis";
const char* senha = "47-999-935-1";

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.softAP("P2.");
  Serial.println();
  pinMode(pinoLEDEnvBro, OUTPUT);
  pinMode(pinoLEDRecBro, OUTPUT);
  conectarRede(WiFi.scanNetworks());
  Udp.begin(localUdpPorta);
}

void conectarRede(int numeroSSID) {
  int i = 0;
  String meuMAC = "";
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, senha);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
  } if (WiFi.status() == WL_CONNECTED) {
    //Serial.println("Conectado na Rede Pela Web: ");
    //Serial.println(WiFi.SSID());
    //Serial.println("IP Consebido a ESP: ");
    Serial.printf("Meu IP: %s\n", WiFi.localIP().toString().c_str());
    Serial.printf("Meu MAC Address: %s\n", WiFi.macAddress().c_str());
    meuMAC = "P2." + WiFi.macAddress();
    meuMAC.toCharArray(convMeuMAC, 21);
  }
}

void enviaUDP() {
  //Envia Solicitação de MAC
  Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
  Udp.write(convMeuMAC);
  Udp.endPacket();
  if (Udp.endPacket() == true) {
    int i = 0;
    while (i < 30000) {
      digitalWrite(pinoLEDEnvBro, HIGH);
      i = i + 1;
    }
  } digitalWrite(pinoLEDEnvBro, LOW);
}

void recebeUDP() {
  int tamanhoPacote = Udp.parsePacket();
  if (tamanhoPacote) {
    //Recebe pacotes de entrada
    //Serial.printf("Recebe %d bytes de %s, porta %d \n", tamanhoPacote, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int leitura = Udp.read(mensagemEntrada, 255);
    if (leitura > 0) {
      mensagemEntrada[leitura] = 0;
      int i = 0;
      while (i < 30000) {
        digitalWrite(pinoLEDRecBro, HIGH);
        i = i + 1;
      }
    }
    //Serial.printf("O pacote UDP contem: %s\n", mensagemEntrada);
    digitalWrite(pinoLEDRecBro, LOW);
    enviaUDP();
  }
}

void loop() {
  WiFi.mode(WIFI_STA);
  WiFi.softAP("P2.");
  recebeUDP();
}
