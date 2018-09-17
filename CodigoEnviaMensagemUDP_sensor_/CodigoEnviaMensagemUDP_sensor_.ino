/*  Desenvolvido por Alessandro Kantousian
 *  Data 13/09/2018
 *  Referências: https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/udp-examples.html
 */
#include "ESP8266WiFi.h"
#include "WiFiUdp.h"
#define pinoLEDRec D1
#define pinoLEDEnv D2
#define pinoLEDSen D3
#define pinoSensor D7

WiFiUDP Udp;
unsigned int localUdpPorta = 4310;
char mensagemEntrada[255];
char mensagemDeEnvio[] = "Passou na Porta";
const char* ssid = "SpeedRun WiFi";
const char* senha = "47-999-935-1";
IPAddress broadcastIp(192,168,43,255);

 
void setup(){
  Serial.begin(115200);  
  WiFi.mode(WIFI_STA);    
  WiFi.mode(WIFI_AP);
  WiFi.softAP("A1P1");
  Serial.println();
  pinMode(pinoLEDSen, OUTPUT);
  pinMode(pinoLEDEnv, OUTPUT);
  pinMode(pinoLEDRec, OUTPUT);
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
    digitalWrite(pinoLEDSen, HIGH);    
    //scaneiaRede(WiFi.scanNetworks());
    enviaUDP();    
  }digitalWrite(pinoLEDSen, LOW);  
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
    //Serial.println(WiFi.localIP());    
    //Serial.printf("Listar o ip %s, UDP porta %d\n", WiFi.localIP().toString().c_str(), localUdpPorta); 
  }
}

void enviaUDP(){
    //Envia Solicitação de MAC
    Udp.beginPacket(broadcastIp, localUdpPorta);
    Udp.write(mensagemDeEnvio);
    Udp.endPacket();
    if(Udp.endPacket() == true){ 
      int i=0;
      while(i<30000){     
      digitalWrite(pinoLEDEnv, HIGH);
      i=i+1;
      }
    }    
    //Serial.println("Qual é seu MAC?");      
      digitalWrite(pinoLEDEnv, LOW);
}

void recebeUDP(){
  int tamanhoPacote=Udp.parsePacket();
  if(tamanhoPacote){
    //Recebe pacotes de entrada
    //Serial.printf("Recebe %d bytes de %s, porta %d \n", tamanhoPacote, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int leitura = Udp.read(mensagemEntrada, 255);
    if(leitura>0){
      mensagemEntrada[leitura]=0;  
      int i=0;
      while(i<30000){          
      digitalWrite(pinoLEDRec, HIGH);     
      i=i+1;
      }   
    }
    Serial.printf("MAC Address Recebido: %s\n", mensagemEntrada);                
    digitalWrite(pinoLEDRec, LOW);
  }
}

void loop(){    
  WiFi.mode(WIFI_STA);  
  WiFi.softAP("A1P1");
  conectarRede(WiFi.scanNetworks());
  sensorPresenca(digitalRead(pinoSensor));
  recebeUDP();
//  delay(500);                         
}
