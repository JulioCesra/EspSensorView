#include <math.h>
#include <Arduino.h>
#include <config.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <NTPClient.h>

#define pinNTC 32

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.br", -10800, 60000);

bool conectarWifi(){
  WiFi.begin(WIFI_NOME, WIFI_SENHA);
  
  int tentativas = 0;
  while (WiFi.status() != WL_CONNECTED && tentativas < 20) {
    delay(500);
    Serial.print(".");
    tentativas++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConectado com sucesso!");
    return true;
  } else {
    Serial.println("\nFalha ao conectar. Continuando sem internet...");
    return false;
  }
}

double temperaturaCaptada(int pinoNTC){
  const double RTNOMINAL = 100000; 
  const double TEMPNOMINAL = 25;
  const double BCOEFFICIENT = 3950; 
  const double RESISTORFIXO = 10000; 

  double leitura = analogRead(pinoNTC);
  
  if (leitura <= 0 || leitura >= 4095) {
    Serial.println("Erro na leitura do sensor (Verifique o pino e conexões)");
    delay(1000);
    return 0;
  }

  double resistencia = RESISTORFIXO / ((4095.0 / leitura) - 1.0);
  
  double temperatura;
  temperatura = resistencia / RTNOMINAL;     
  temperatura = log(temperatura);                  
  temperatura /= BCOEFFICIENT;                     
  temperatura += 1.0 / (TEMPNOMINAL + 273.15); 
  temperatura = 1.0 / temperatura;                
  temperatura -= 273.15;  
  return temperatura;
}

void enviarDados(String json){
  const String URL = URL_API;
  HTTPClient http;
  
  http.begin(URL);
  http.addHeader("Content-Type", "application/json");
  
  int httpCodigoResposta = http.POST(json);
  
  if (httpCodigoResposta > 0) {
    Serial.print("Código de resposta HTTP: ");
    Serial.println(httpCodigoResposta);
    
    if (httpCodigoResposta == HTTP_CODE_OK || httpCodigoResposta == 201) {
      Serial.println("Dados enviados com sucesso!");
    } else {
      String payload = http.getString();
      Serial.println("Resposta do servidor: " + payload);
    }
  } else {
    Serial.print("Erro na requisição HTTP: ");
    Serial.println(http.errorToString(httpCodigoResposta).c_str());
  }
  
  http.end();
}

bool wifiFoiConectado = false;
void setup() {
  Serial.begin(115200);
  wifiFoiConectado = conectarWifi();
  timeClient.begin();
}

void loop() {
  if (wifiFoiConectado){
    timeClient.update();
    JsonDocument doc;
    const double temperatura = temperaturaCaptada(pinNTC);

    doc["temperatura"] = temperatura;
    doc["registro"] = timeClient.getFormattedTime();

    String saidaJson;
    serializeJson(doc, saidaJson);
    enviarDados(saidaJson);
  }
  
  delay(5000);
}
