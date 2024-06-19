#define BLYNK_PRINT Serial


#define BLYNK_TEMPLATE_ID "xxxxxxxxxxxx"
#define BLYNK_TEMPLATE_NAME "xxxxxxxx"
#define BLYNK_AUTH_TOKEN "xxxxxxxxxxx"


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266_SSL.h>
#include <DHT.h>
float hum =0.0 , temp_c = 0.0 , temp_f = 0.0; 
const byte DHT_PIN =D1;
const byte DHT_TYPE =22;
DHT dht(DHT_PIN, DHT_TYPE);
char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "xxxx";
char pass[] = "xxxxxxxx";

BLYNK_WRITE(V1) {
  int value = param.asInt();
  digitalWrite(D7, value);
}
void dht() {
  hum = dht.readHumidity ();

temp_c =dht.readTemperature ();


 
if (isnan(hum)||isnan(temp_c) ){

  Serial.println("Failed To Read Fro DHT Sensor");
  return;

}
Serial.print("Humidity:");
Serial.print(hum);
Serial.print("%\t");
Serial.print("Temperature:");
Serial.print(temp_c);
Serial.print("c\t");
Serial.println();
Blynk.virtualWrite(V2, temp_c);
Blynk.virtualWrite(V3, hum);
}
void setup() {
  // Debug console
  Serial.begin(9600);
  pinMode(D7, OUTPUT);
  dht.begin();

  Blynk.begin(auth, ssid, pass);
}

void loop() {
  Blynk.run();
}
