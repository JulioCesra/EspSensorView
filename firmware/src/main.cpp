#include <Arduino.h>
#include <math.h>
#include <WiFi.h>
#include <config.h> // Variáveis de ambiente
#include <ArduinoJson.h>
#include <time.h>
#include <HTTPClient.h>

#define pinNTC 32
#define pinInfravermelho 33
#define pinLDR 34

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -10800; 
const int   daylightOffset_sec = 0; 

double temperaturaObtidaPeloSensorNTC(){

  double leituraSensorNTC = analogRead(pinNTC);

  const double RTNOMINAL = 100000.0;   
  const double TEMPNOMINAL = 25.0;
  const double BCOEFFICIENT = 3950.0;  
  const double RESISTORFIXO = 10000.0; 

   if (leituraSensorNTC <= 10 || leituraSensorNTC >= 4085) {
    Serial.print("Leitura ADC fora dos limites: ");
    Serial.println(leituraSensorNTC);
    delay(1000);
    return 0.0;
  }

  double resistencia = RESISTORFIXO / ((4095.0 / leituraSensorNTC) - 1.0);

  // Equação de Steinhart-Hart para converter resistência em temperatura Celsius
  double temperatura;
  temperatura = resistencia / RTNOMINAL;     
  temperatura = log(temperatura);                  
  temperatura /= BCOEFFICIENT;                     
  temperatura += 1.0 / (TEMPNOMINAL + 273.15); 
  temperatura = 1.0 / temperatura;                
  temperatura -= 273.15;  

  return temperatura;
}

bool conexaoComAInternet(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_NOME, WIFI_SENHA);
  int tentativasDeConexao = 0;
  while (WiFi.status() != WL_CONNECTED && tentativasDeConexao < 6){
    Serial.print(".");
    tentativasDeConexao += 1;
    delay(1000);
  }
  if (WiFi.status() == WL_CONNECTED){
    Serial.println("\nESP32 conectado ao WIFI.");
    return true;
  }
  Serial.println("\nESP32 não conseguiu se conectar na internet.");
  return false;
}

String horarioLocal(){

  struct tm informacoesDoTempoLocal;
  if(!getLocalTime(&informacoesDoTempoLocal)){
    Serial.println("Falha ao obter dados do horário local.");
    return "";
  }
  char buffer[50];
  strftime(buffer, sizeof(buffer), "%H:%M:%S", &informacoesDoTempoLocal);
  String horarioFormatado = String(buffer);
  return horarioFormatado;
}

String jsonFormatadaParaEnvio(String nomeDoSensor, String tipoDaInformacao, double valor){
  JsonDocument doc;
  doc["sensor"] = nomeDoSensor;
  doc["tipo"] = tipoDaInformacao;
  doc["valor"] = valor;
  doc["referencia"] = horarioLocal();
  String jsonEstruturado;
  serializeJson(doc, jsonEstruturado);
  return jsonEstruturado;
}

void enviarDadosParaAPI(String json){
  HTTPClient http;
  http.begin(URL_API);
  http.addHeader("Content-Type", "application/json");
  int respostaRequisicaoHTTP = http.POST(json);
  if (respostaRequisicaoHTTP > 0) {
    String respostaDoServidor = http.getString();
    Serial.print("Código HTTP: ");
    Serial.println(respostaRequisicaoHTTP);
    Serial.print("Resposta: ");
    Serial.println(respostaDoServidor);
  } else {
    Serial.print("Erro no envio POST. Código: ");
    Serial.println(respostaRequisicaoHTTP);
  }
  http.end();
}

double dadosColetadosPeloSensorLDR(){
  return analogRead(pinLDR);
}

int dadosColetadosPeloSensorInfravermelho(){
  return digitalRead(pinInfravermelho);
}

bool foiConectadoAInternet = false;
void setup() {
  Serial.begin(115200);
  foiConectadoAInternet = conexaoComAInternet();
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  pinMode(33, INPUT);
}

String json;
void loop() {
  if (foiConectadoAInternet){
    double temperatura = temperaturaObtidaPeloSensorNTC();
    json = jsonFormatadaParaEnvio("NTC 10K", "temperatura", temperatura);
    enviarDadosParaAPI(json);

    int presenca = dadosColetadosPeloSensorInfravermelho();
    json = jsonFormatadaParaEnvio("TCRT5000", "presenca", presenca);
    enviarDadosParaAPI(json);

    double luminosidade = dadosColetadosPeloSensorLDR();
    String json = jsonFormatadaParaEnvio("LDR", "luminosidade", luminosidade);
    enviarDadosParaAPI(json);
  }

  delay(1000); // Atualiza a cada 1 segundo
}
