#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>
#include <DHT.h>

// Set these to run example. 
#define FIREBASE_HOST "helpyweather-b54ab-default-rtdb.firebaseio.com" 
#define FIREBASE_AUTH "LwDhQynOI2BrXFBzxkSW8GSkUF9zAEqVL1nCK6Ck" 
#define WIFI_SSID "LIVECOM_JOSE" 
#define WIFI_PASSWORD "9677tjsdias"

#define DHT_PIN 4
const int pinHigrometro = A0;

DHT dht(DHT_PIN, DHT11);

void setup() { 

  Serial.begin(9600); 
  // Inicializa o sensor DHT
  dht.begin();

  // connect to wifi. 
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD); 
  Serial.print("connecting"); 
  while (WiFi.status() != WL_CONNECTED) { 
    Serial.print("."); 
    delay(500); 
  } 
  Serial.println(); 
  Serial.print("connected: "); 
  Serial.println(WiFi.localIP()); 
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); 
} 

void loop() { 
  // Lê os valores do sensor DHT
  float temperatura = dht.readTemperature();
  float umidadeAr = dht.readHumidity();

  // Simula um valor para o sensor de solo (substitua com a lógica do seu sensor)
  int umidadeSolo = analogRead(pinHigrometro);

  // set value 
  Firebase.setFloat("sensores/umidadeSolo", umidadeSolo); 
  // handle error 
  if (Firebase.failed()) { 
      Serial.print("setting /number failed:"); 
      Serial.println(Firebase.error());   
      return; 
  }
  Firebase.setFloat("sensores/umidadeAr", umidadeAr); 
  // handle error 
  if (Firebase.failed()) { 
      Serial.print("setting /number failed:"); 
      Serial.println(Firebase.error());   
      return; 
  } 
  Firebase.setFloat("sensores/temperatura", temperatura); 
  // handle error 
  if (Firebase.failed()) { 
      Serial.print("setting /number failed:"); 
      Serial.println(Firebase.error());   
      return; 
  } 
  delay(1000); 
}
