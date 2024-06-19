#include <Blynk.h>
#define BLYNK_PRINT Serial
#define BLYNK_AUTH_TOKEN "xxxxxxxxxxxxxxx"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266_SSL.h>
#include <DHT.h>
#define BLYNK_TEMPLATE_ID "xxxxxxxxxxxx"
#define BLYNK_TEMPLATE_NAME "xxxxxxxxx"
#define BLYNK_AUTH_TOKEN "xxxxxxxxxxxxx"
float hum = 0.0, temp_c = 0.0, temp_f = 0.0;
int gasValue = 0;
int flameValue = 0;
int thresholdValue = 900;  

const byte DHT_PIN = D1;
const byte DHT_TYPE = 22;
const byte GAS_SENSOR_PIN = A0;
const byte FLAME_SENSOR_PIN = D6;
const int buzzerPin1 = D0;
const int buzzerPin2 = D8;
DHT dht(DHT_PIN, DHT_TYPE);

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "xxxxxxx";
char pass[] = "xxxxxxxxxxx";

BLYNK_WRITE(V1) {
  int value = param.asInt();
  digitalWrite(D7, value);
}

void readDHT() {
  hum = dht.readHumidity();
  temp_c = dht.readTemperature();

  if (isnan(hum) || isnan(temp_c)) {
    Serial.println("Failed To Read From DHT Sensor");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(hum);
  Serial.print("%\t");
  Serial.print("Temperature: ");
  Serial.print(temp_c);
  Serial.print("°C\t");
  Serial.println();

  Blynk.virtualWrite(V2, temp_c);
  Blynk.virtualWrite(V3, hum);
}

void readGasSensor() {
  gasValue = analogRead(GAS_SENSOR_PIN);
  Serial.print("Gas Sensor Value: ");
  Serial.println(gasValue);

  Blynk.virtualWrite(V4, gasValue);
}

void readFlameSensor() {
  flameValue = digitalRead(FLAME_SENSOR_PIN);
  Serial.print("Flame Sensor Value: ");
  Serial.println(flameValue);

  if (flameValue == HIGH && gasValue > thresholdValue) {
    Serial.println("Fire Detected! Activating Alarm...");
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(D7, OUTPUT);
  pinMode(FLAME_SENSOR_PIN, INPUT);
  pinMode(buzzerPin1, OUTPUT);
  pinMode(buzzerPin2, OUTPUT);
  dht.begin();
  Blynk.begin(auth, ssid, pass);
}

void loop() {
  readGasSensor();     // Call the function to read gas sensor data
  readFlameSensor();   // Call the function to read flame sensor data and trigger alarm

  if (gasValue > 650) {
    Serial.println("Gas detected!");
    digitalWrite(buzzerPin1, HIGH); // Activate the buzzer on Pin D0
  } else {
    Serial.println("No gas detected");
    digitalWrite(buzzerPin1, LOW);  // Deactivate the buzzer on Pin D0
  }

  if (flameValue == HIGH) {
    Serial.println("Flame detected!");
    digitalWrite(buzzerPin2, HIGH); // Activate the buzzer on Pin D8
  } else {
    Serial.println("No flame detected");
    digitalWrite(buzzerPin2, LOW);  // Deactivate the buzzer on Pin D8
  }

  Blynk.run();
  readDHT();           // Call the function to read DHT sensor data

  // ... rest of the loop code ...
}
