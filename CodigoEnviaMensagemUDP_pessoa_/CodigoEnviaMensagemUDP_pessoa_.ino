/*  Desenvolvido por Alessandro Kantousian
 *  Data 13/09/2018
 *  Referências: https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/udp-examples.html
 */
#include "ESP8266WiFi.h"
#include "WiFiUdp.h"
#define pinoLED D7
#define pinoSensor D1

WiFiUDP Udp;
unsigned int localUdpPorta = 4310;
char mensagemEntrada[255];
char mensagemDeEnvio[] = "Enviei meu MAC";
const char* ssid = "SpeedRun WiFi";
const char* senha = "47-999-935-1";
 
void setup(){
  Serial.begin(115200);  
  WiFi.mode(WIFI_STA);    
  WiFi.mode(WIFI_AP);
  WiFi.softAP("P1");
  Serial.println();
  pinMode(pinoLED, OUTPUT);
  pinMode(pinoSensor, INPUT);
  Udp.begin(localUdpPorta);
}

void scaneiaRede(int numeroSSID){  
  int i=0;
  String nomeRede="";
  int menorRede=-1000;
  for(i; i<numeroSSID; i++){    
    if(WiFi.RSSI(i) >= menorRede){
      menorRede=WiFi.RSSI(i);    
      nomeRede=WiFi.SSID(i);
    }
  }Serial.printf("Rede com menor distância foi: %s, Distância em dBs: (%d)\n", nomeRede.c_str() ,menorRede);
}

void sensorPresenca(int recebeValor){
  if(recebeValor == HIGH){
    digitalWrite(pinoLED, HIGH);    
    scaneiaRede(WiFi.scanNetworks());
  }else{
    digitalWrite(pinoLED, LOW);
  }
}

void conectarRede(int numeroSSID){
  int i=0;
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
    Serial.printf("Listar o ip %s, UDP porta %d\n", WiFi.localIP().toString().c_str(), localUdpPorta); 
  }
}

void enviaUDP(){
    //Envia Solicitação de MAC
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(mensagemDeEnvio);
    Udp.endPacket();
}

void recebeUDP(){
  int tamanhoPacote=Udp.parsePacket();
  if(tamanhoPacote){
    //Recebe pacotes de entrada
    Serial.printf("Recebe %d bytes de %s, porta $d \n", tamanhoPacote, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int leitura = Udp.read(mensagemEntrada, 255);
    if(leitura>0){
      mensagemEntrada[leitura]=0;     
    }
    Serial.printf("O pacote UDP contem: %s\n", mensagemEntrada);
    enviaUDP();
  }
}

void loop(){      
  conectarRede(WiFi.scanNetworks());
  //sensorPresenca(digitalRead(pinoSensor));       
  //WiFi.mode(WIFI_STA);  
  //WiFi.softAP("P1");
  recebeUDP();
  delay(500);                         
}
