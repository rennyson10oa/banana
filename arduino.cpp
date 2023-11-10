#include <DHT.h>

const int dhtPin = 4;  // GPIO4 para DHT11
const int higroPin = A0;
int higroValue = 0; // valor inicial

DHT dht(dhtPin, DHT11);

void setup() {
  Serial.begin(115200);
  dht.begin();
}

void loop() {
  // Read the temperature and humidity data from the sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  higroValue = analogRead(higroPin);

  // Print the data to the serial monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");
  Serial.print("sensor = ");
  Serial.print(higroValue);
  

  // Wait for 2 seconds before taking the next reading
  delay(2000);
}