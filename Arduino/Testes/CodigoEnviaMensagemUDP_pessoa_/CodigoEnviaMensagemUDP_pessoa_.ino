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

bool continuar = false;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP("P1");

  pinMode(pinoLEDEnvBro, OUTPUT);
  pinMode(pinoLEDRecBro, OUTPUT);

  WiFi.begin("virus", "12345678");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());
  Udp.begin(localUdpPorta);
}

void enviaUDP(String envio) {
  //Envia Solicitação de MAC
  Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
  Serial.println(envio);
  Serial.println(Udp.remoteIP());
  Serial.println(Udp.remotePort());
  Udp.write(envio.c_str(), envio.length());

  if (Udp.endPacket()) {
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
    char c[tamanhoPacote];
    int leitura = Udp.read(c, tamanhoPacote);
    if (leitura > 0) {
      c[leitura] = 0;
      int i = 0;
      while (i < 30000) {
        digitalWrite(pinoLEDRecBro, HIGH);
        i = i + 1;
      }
    }
    Serial.printf("O pacote UDP contem: %s\n", c);

    String envio;
    if (String(c) == "quem") {
      envio = "P1" + WiFi.macAddress() + WiFi.localIP().toString();
      enviaUDP(envio);
    }
    else if (String(c) == "ok") {
      envio = WiFi.macAddress();
      continuar = true;
      enviaUDP(envio);
    }
    else if (String(c) == "okn") {
      continuar = false;
    }
    else {
      Serial.println("Msg errada: " + String(c));
    }
    digitalWrite(pinoLEDRecBro, LOW);

  }
}

String scaneiaRede(int numeroSSID) {
  String retorno = "";
  for (int i = 0; i < numeroSSID; i++) {
    if (
      WiFi.SSID(i).length() == 4
      && WiFi.SSID(i).charAt(0) == 'A'
      && WiFi.SSID(i).charAt(2) == 'P'
      && isDigit(WiFi.SSID(i).charAt(1))
      && isDigit(WiFi.SSID(i).charAt(3))
    ) {
      Serial.println(WiFi.SSID(i));
      retorno += WiFi.SSID(i) + String(WiFi.RSSI(i), DEC) + ";";
    }

  }
  if (retorno != "")
    retorno = retorno.substring(0, retorno.length() - 1);

  return retorno;
}

void loop() {
  //  WiFi.mode(WIFI_AP_STA);
  //  WiFi.softAP("P1");
  recebeUDP();

  if (continuar) {
    unsigned long timeT = millis();
    while (millis() - timeT < 9000) {
      //      Serial.println(millis() - timeT);
      Udp.beginPacket("192.168.0.104", 10000);
      String envio = WiFi.macAddress() + ">" + scaneiaRede(WiFi.scanNetworks(false, false, 1, NULL));
      Serial.println(envio);
      Udp.write(envio.c_str(), envio.length());
      Udp.endPacket();
      Udp.flush();
    }
    continuar = false;
  }

}
