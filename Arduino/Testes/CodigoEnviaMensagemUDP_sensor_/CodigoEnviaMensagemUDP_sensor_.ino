/*  Desenvolvido por Alessandro Kantousian
    Data 13/09/2018
    Referências: https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/udp-examples.html

*/
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#define pinoLEDRecBro D0
#define pinoLEDEnvBro D2
#define pinoLEDSenInf D3
#define pinoLEDEnvRas D4
#define pinoLEDRecRas D5
#define pinoSensorInf D7

long previousMillis=0; //Variável de controle do tempo
long intParaZerarVar=6000; //Tempo em ms do intervalo a ser executado
unsigned long currentMillis;
unsigned int localUdpPorta = 4500;
unsigned int localUdpPortaRasp = 10000;
const char* ssid = "SARKISTEL";
const char* senha = "479999351";
char mensagemEntradaMacBro[255];  
char mensagemIndentificaPessoa[8];
String converteMAC, recebeMACConv;


WiFiUDP Udp;
IPAddress broadcastIp(192, 168, 0, 255);
IPAddress broadcastIpRasp(192, 168, 43, 65);

typedef struct lista{
  String macAddr = "";
}Lista;

struct celula{
  Lista *conteudo;
  struct celula*ant;
  struct celula*prox;
};
typedef struct celula cel;


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
  conectarRede(WiFi.scanNetworks());
  Udp.begin(localUdpPorta);
  Udp.begin(localUdpPortaRasp);
}

//Insere no início da lista.

void insereInicio(cel**lst, Lista *p){
  cel *nova;
  nova=(cel*)malloc(sizeof(cel*));
  nova->conteudo=p;
  nova->prox=(*lst);
  nova->prox=(*lst)->prox;
  (*lst)->prox=nova;
  if(nova->prox != NULL){
    nova->prox->ant=nova;
  }
}

void deleta(cel **lst, Lista *r){ 
  
  cel *p;
  p=(*lst);
  if((*lst)->prox=NULL){
  }
  else{    
    while((p != NULL) && (p->conteudo != r)){
      p=p->prox;
    }
    if (p == NULL){
      return;//Lista vazia
    }
    if(p->prox != NULL){
      p->prox->ant = p->ant;
      p->ant->prox = p->prox;
      free(p);
      Serial.println("Deletei!");
    }
  }
}

void imprime(cel*lst){
  cel*p;
  p=lst->prox;
  while(p != NULL){
    Serial.printf("Print da lista: %s", p->conteudo->macAddr.c_str());
    Serial.println("\n");
    p=p->prox;
  }
}

void sepVetorStr(cel*lst){
  
  cel*p;
  p=lst->prox;
  while(p != NULL){
    p=p->prox;
//    deleta(&lst, p);
  }
    Serial.println((p->conteudo->macAddr).substring(0,3));
    Serial.println((p->conteudo->macAddr).substring(3,20));
//A função abaixo faz a mesma coisa que o método substring  
//  int i=0;
//  String come = "";
//  String fin= "";
//  for(i; i<recebeValor.length() && i<3; i++){
//    come += recebeValor.charAt(i);
//  }
//  for(i=3; i<recebeValor.length(); i++){
//    fin += recebeValor.charAt(i);
//  }  
//   Serial.println(come);
//   Serial.println(fin);
}

void zeraVariavel(){
  
  if((currentMillis-previousMillis) > intParaZerarVar){
    previousMillis=currentMillis;
    recebeMACConv="a";    
  }
}

void scaneiaRede(int numeroSSID) { // Ainda não estou usando para nada essa função ainda
  int i = 0;
  String nomeRede = "";
  int menorRede = -1000;
  while (i < numeroSSID) {
    if (WiFi.RSSI(i) >= menorRede) {
      menorRede = WiFi.RSSI(i);
      nomeRede = WiFi.SSID(i);
    }
    i = i + 1;
  } Serial.printf("Rede com menor distância foi: %s, Distância em dBs: (%d)\n", nomeRede.c_str() , menorRede);
}

void sensorPresenca(int recebeValor) {

  if (recebeValor == 0) {
    digitalWrite(pinoLEDSenInf, HIGH);
    //scaneiaRede(WiFi.scanNetworks());
    enviaUDP();
  } digitalWrite(pinoLEDSenInf, LOW);
}

void conectarRede(int numeroSSID) {

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
  converteMAC = String(mensagemEntradaMacBro);
  if (converteMAC != recebeMACConv) {
    recebeMACConv = converteMAC;
    //  Serial.println(recebeMACConv);
    Udp.beginPacket(broadcastIpRasp, localUdpPortaRasp);//Luiz add aqui o IP e a Porta da Rasp
    Udp.write(mensagemEntradaMacBro);
    Udp.endPacket();
    if (Udp.endPacket() == true) {
      int i = 0;
      while (i < 30000) {
        digitalWrite(pinoLEDEnvRas, HIGH);
        i = i + 1;
      }
    }
    digitalWrite(pinoLEDEnvRas, LOW);
  }
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
  if (tamanhoPacote) {
    //Recebe pacotes de entrada para quem responder o Broadcast
    //Serial.printf("Recebe %d bytes de %s, porta %d \n", tamanhoPacote, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int leitura = Udp.read(mensagemEntradaMacBro, 255);
    if (leitura > 0) {
      mensagemEntradaMacBro[leitura] = 0;
      //Serial.println("Este MAC é do Broadcast: ");
      //Serial.println(mensagemEntradaMacBro);
      int i = 0;
      while (i < 30000) {
        digitalWrite(pinoLEDRecBro, HIGH);
        i = i + 1;
      }
    }
    digitalWrite(pinoLEDRecBro, LOW);
    String mensagemMAC = String(mensagemEntradaMacBro);
    cel*lst;
    lst=(cel*)malloc(sizeof(cel*));
    lst->prox=NULL;
    lst->ant=NULL;
    lst->conteudo=0;

    Lista mac;
    mac.macAddr=mensagemMAC;
    insereInicio(&lst, &mac);
    //sepVetorStr(lst);
    imprime(lst);
    deleta(&lst, &mac);
    //enviaMacParaRasp();
  }
}

void loop() {
  
  currentMillis = millis(); //Tempo atual em ms
  WiFi.mode(WIFI_STA);
  WiFi.softAP("A1P1");
  sensorPresenca(digitalRead(pinoSensorInf));
  //scaneiaRede(WiFi.scanNetworks());
  //recMacDaRasp();
  recebeUDP();
}
