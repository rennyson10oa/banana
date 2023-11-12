#include <ESP8266WiFi.h>

void setup() {
  Serial.begin(115200);

  // Inicie a conexão Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(1000);
  Serial.println("Escaneando redes Wi-Fi próximas:");
  int numRedes = WiFi.scanNetworks();

  if (numRedes == 0) {
    Serial.println("Nenhuma rede Wi-Fi encontrada.");
  } else {
    Serial.println("Redes Wi-Fi disponíveis:");
    for (int i = 0; i < numRedes; i++) {
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.println(WiFi.SSID(i));
    }

    Serial.print("Escolha o número da rede Wi-Fi: ");
    while (Serial.available() == 0) {
      // Aguarda a entrada do usuário
    }

    int escolhaRede = Serial.parseInt();
    if (escolhaRede > 0 && escolhaRede <= numRedes) {
      Serial.print("Digite a senha da rede escolhida: ");
      while (Serial.available() == 0) {
        // Aguarda a entrada do usuário
      }
      String senha = Serial.readString();
      senha.trim();

      Serial.print("Conectando à rede: ");
      Serial.println(WiFi.SSID(escolhaRede - 1));

      WiFi.begin(WiFi.SSID(escolhaRede - 1).c_str(), senha.c_str());

      while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Conectando ao WiFi...");
      }

      Serial.println("Conectado à rede WiFi");
    } else {
      Serial.println("Número de rede inválido.");
    }
  }
}

void loop() {
  // Seu código principal aqui
}
