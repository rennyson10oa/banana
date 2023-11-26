#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <DHT.h>

// Substitua essas informações com as suas credenciais Firebase
#define FIREBASE_HOST "seu-projeto.firebaseio.com"
#define FIREBASE_AUTH "sua-chave-de-autenticacao"

// Substitua essas informações com as suas credenciais de Wi-Fi
#define WIFI_SSID "seu-SSID"
#define WIFI_PASSWORD "sua-senha"

#define DHT_PIN 2 // Pino de dados do sensor DHT

DHT dht(DHT_PIN, DHT11);

void setup() {
  Serial.begin(115200);

  // Conecta-se ao Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }
  Serial.println("Conectado ao Wi-Fi!");

  // Inicializa o sensor DHT
  dht.begin();

  // Inicializa a comunicação com o Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  // Lê os valores do sensor DHT
  float temperatura = dht.readTemperature();
  float umidade = dht.readHumidity();

  // Simula um valor para o sensor de solo (substitua com a lógica do seu sensor)
  int umidadeSolo = analogRead(A0);

  // Cria um objeto JSON para enviar ao Firebase
  String jsonData = "{\"temperatura\": " + String(temperatura) +
                    ", \"umidade\": " + String(umidade) +
                    ", \"umidade_solo\": " + String(umidadeSolo) + "}";

  // Envia os dados para a coleção "dados" no Firebase
  Firebase.pushString("/dados", jsonData);

  // Aguarda por um intervalo antes de enviar novamente (por exemplo, a cada 5 segundos)
  delay(5000);
}
