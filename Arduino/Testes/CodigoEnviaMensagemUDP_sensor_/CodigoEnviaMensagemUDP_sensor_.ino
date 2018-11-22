/*  Desenvolvido por Alessandro Kantousian
    Data 13/09/2018
    Referências: https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/udp-examples.html

*/
#include <QueueArray.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#define pinoLEDRecBro D0
#define pinoLEDEnvBro D2
#define pinoLEDSenInf D3
#define pinoLEDEnvRas D4
#define pinoLEDRecRas D5
#define pinoSensorInf D7

long previousMillis = 0; //Variável de controle do tempo
long intParaZerarVar = 6000; //Tempo em ms do intervalo a ser executado
unsigned long currentMillis;
unsigned int localUdpPorta = 4500;
unsigned int localUdpPortaRasp = 10000;
const char* ssid = "virus";
const char* senha = "12345678";
char mensagemEntradaMacBro[255];
char convMeuMAC[21];
String converteMAC, recebeMACConv, mensagemMAC;
String fim = "";
String nomeRede = "";


WiFiUDP Udp;
IPAddress broadcastIp(192, 168, 0, 255);
IPAddress broadcastIpRasp(192, 168, 0, 100);


void setup() {

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.mode(WIFI_AP);
  WiFi.softAP("A1P1");
  Serial.println();
  pinMode(pinoLEDRecBro, OUTPUT);
  pinMode(pinoLEDEnvBro, OUTPUT);
  pinMode(pinoLEDSenInf, OUTPUT);
  pinMode(pinoLEDEnvRas, OUTPUT);
  pinMode(pinoLEDRecRas, OUTPUT);
  pinMode(pinoSensorInf, INPUT);
  Udp.begin(localUdpPorta);
  Udp.begin(localUdpPortaRasp);
  conectarRede();
}


void sepVetorStr() {

  //  if(com == nomeRede){
  //    enviaMacParaRasp();
  //  }
  //  Serial.printf("Rede com menor distância foi: %s, Distância em dBs: (%d)\n", nomeRede.c_str() , menorRede);
  //  Serial.println(fim);
  //A função abaixo faz a mesma coisa que o método substring
  //  int i=0;
  //  String come = "";
  //  String fin= "";
  //  for(i; i<recebConteudo.length() && i<3; i++){
  //    come += recebConteudo.charAt(i);
  //  }
  //  for(i=3; i<recebConteudo.length(); i++){
  //    fin += recebConteudo.charAt(i);
  //  }
  //   Serial.println(come);
  //   Serial.println(fin);
}

void zeraVariavel() {

  if ((currentMillis - previousMillis) > intParaZerarVar) {
    previousMillis = currentMillis;
    recebeMACConv = "a";
  }
}

void scaneiaRede(int numeroSSID) { // Ainda não estou usando para nada essa função ainda

  int i = 0;
  //String nomeRede = "";
  int menorRede = -1000;
  while (i < numeroSSID) {
    if (WiFi.RSSI(i) >= menorRede) {
      menorRede = WiFi.RSSI(i);
      nomeRede = WiFi.SSID(i);
    }
    i = i + 1;
    Serial.println(WiFi.SSID(i).c_str());
  }
  Serial.printf("Rede com menor distância foi: %s, Distância em dBs: (%d)\n", nomeRede.c_str() , menorRede);
}

void sensorPresenca(int recebeValor) {

  if (recebeValor == 0) {
    digitalWrite(pinoLEDSenInf, HIGH);
    scaneiaRede(WiFi.scanNetworks(false, false, 3, NULL));
    enviaUDP();
  } digitalWrite(pinoLEDSenInf, LOW);
}

void conectarRede() {

  int i = 0;
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, senha);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
  } if (WiFi.status() == WL_CONNECTED) {
    //Serial.println("Conectado na Rede Pela Web: ");
    //Serial.println(WiFi.SSID());
    //Serial.println("IP Consebido a ESP: ");
    Serial.println(WiFi.localIP());
    //Serial.printf("Listar o ip %s, UDP porta %d\n", WiFi.localIP().toString().c_str(), localUdpPorta);
  }
}

void enviaMacParaRasp() {

  zeraVariavel();
  //Envia o MAC para o servidor, que foi respondido pelo Broadcast ativado pelo sensor
  fim.toCharArray(convMeuMAC, 20);
  converteMAC = String(mensagemEntradaMacBro);
  if (converteMAC != recebeMACConv) {
    recebeMACConv = converteMAC;
    //  Serial.println(recebeMACConv);
    Udp.beginPacket(broadcastIpRasp, localUdpPortaRasp);//Luiz add aqui o IP e a Porta da Rasp
    Udp.write(convMeuMAC);
    Udp.endPacket();
    if (Udp.endPacket() == true) {
      Serial.println("Enviado!");
      Serial.printf("Rede com menor distância foi: %s\n", nomeRede.c_str());
      Serial.println(convMeuMAC);
      fim="a";
      int i = 0;
      while (i < 30000) {
        digitalWrite(pinoLEDEnvRas, HIGH);
        i = i + 1;
      }
    }
    digitalWrite(pinoLEDEnvRas, LOW);
  }
  recMacDaRasp();
}

void recMacDaRasp() {

  //Recebe a o MAC do servidor
  char respostaEntradaMacRas[255];
  int tamanhoPacote = Udp.parsePacket();
  if (tamanhoPacote) {
    //Recebe pacotes de entrada
    //Serial.printf("Recebe %d bytes de %s, porta %d \n", tamanhoPacote, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int leitura = Udp.read(respostaEntradaMacRas, 255);
    if (leitura > 0) {
      respostaEntradaMacRas[leitura] = 0;
      int i = 0;
      while (i < 30000) {
        digitalWrite(pinoLEDRecRas, HIGH);
        i = i + 1;
      }
      Serial.println("Esse é o MAC respondido pelo Servidor: ");
      Serial.println(respostaEntradaMacRas);
      digitalWrite(pinoLEDRecRas, LOW);
      //if(mensagemEntrada==mensagemEntradaMac){
      //  Serial.println("Você é funcionário");
      //}else{
      //  Serial.println("Você não é funcionário");

      //  Serial.println(mensagemEntrada);
      //}
    }
  }
}

void enviaUDP() {

  //Envia Solicitação de MAC para quem passou pelo sensor
  char mensagemDeEnvioBro[] = "Quem é?";
  Udp.beginPacket(broadcastIp, localUdpPorta);
  Udp.write(mensagemDeEnvioBro);
  Udp.endPacket();
  if (Udp.endPacket() == true) {
    int i = 0;
    while (i < 30000) {
      digitalWrite(pinoLEDEnvBro, HIGH);
      i = i + 1;
    }
  }
  digitalWrite(pinoLEDEnvBro, LOW);
}

void recebeUDP() {

  int tamanhoPacote = Udp.parsePacket();
  String com = "";
  if (tamanhoPacote) {
    //Recebe pacotes de entrada para quem responder o Broadcast
    //Serial.printf("Recebe %d bytes de %s, porta %d \n", tamanhoPacote, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int leitura = Udp.read(mensagemEntradaMacBro, 255);
    if (leitura > 0) {
      mensagemEntradaMacBro[leitura] = 0;
      Serial.println("Este é o mac recebido");
      Serial.println(mensagemEntradaMacBro);
      int i = 0;
      while (i < 30000) {
        digitalWrite(pinoLEDRecBro, HIGH);
        i = i + 1;
      }
    }
    digitalWrite(pinoLEDRecBro, LOW);
    mensagemMAC = String(mensagemEntradaMacBro);
    com = mensagemMAC.substring(0, 2);
    fim = mensagemMAC.substring(2, 20);
    if (com == nomeRede) {
      enviaMacParaRasp();
    }
  }
}

void loop() {

  currentMillis = millis(); //Tempo atual em ms
  WiFi.mode(WIFI_STA);
  WiFi.softAP("A1P1");
  sensorPresenca(digitalRead(pinoSensorInf));
  recebeUDP();
}
