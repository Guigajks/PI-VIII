#include <ESP8266WiFi.h>
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

void scaneiaRede(int numeroSSID) {
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
      retorno += WiFi.SSID(i) + "-" + String(WiFi.RSSI(i), DEC) + ";";
    }
  }

  if (retorno != "")
    retorno = retorno.substring(0, retorno.length() - 1);

  return retorno;
}

void sensorPresenca(int recebeValor) {
  digitalWrite(pinoLED, LOW);
  Serial.println(recebeValor);
  if (recebeValor == 1) {
    digitalWrite(pinoLED, HIGH);
  }
}

void loop()
{
  sensorPresenca(digitalRead(pinoSensor));
  //scaneiaRede(WiFi.scanNetworks());
  delay(500);
}
