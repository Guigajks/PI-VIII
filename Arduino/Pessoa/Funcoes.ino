/********Metodos para o calculo da posicao**********/

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
      retorno += WiFi.SSID(i) + ":" + String(WiFi.RSSI(i), DEC) + ";";
    }
  }

  if (retorno != "")
    retorno = retorno.substring(0, retorno.length() - 1);

  return retorno;
}


/********Metodos de comunicacao**********/

void enviaUDP() {
  //Envia Solicitação de MAC
  Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
  Udp.write(WiFi.macAddress().c_str());
  Udp.endPacket();
  // if (Udp.endPacket() == true) {
  //   int i = 0;
  //   while (i < 30000) {
  //     digitalWrite(pinoLEDEnv, HIGH);
  //     i = i + 1;
  //   }
  // } digitalWrite(pinoLEDEnv, LOW);
}

void recebeUDP() {
  // int tamanhoPacote = Udp.parsePacket();
  if (Udp.parsePacket()) {
    //Recebe pacotes de entrada
    //Serial.printf("Recebe %d bytes de %s, porta %d \n", tamanhoPacote, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int leitura = Udp.read(mensagemEntrada, 255);
    if (leitura > 0) {
      mensagemEntrada[leitura] = 0;
      int i = 0;
      while (i < 30000) {
        digitalWrite(pinoLEDRec, HIGH);
        i = i + 1;
      }
    }
    //Serial.printf("O pacote UDP contem: %s\n", mensagemEntrada);
    digitalWrite(pinoLEDRec, LOW);
    enviaUDP();
  }
}