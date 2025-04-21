#include <Arduino.h>
#include <LoRa.h>
#include <SPI.h>
#include <Wire.h>
#include "ADS1X15.h"
#include "SHT21.h"
#include "base64.hpp"

#define ss 0
#define rst 15
#define dio0 7

ADS1115 ADS(0x48);
SHT21 sht;

unsigned char normal_text[128];
unsigned char base64_text[192];  // Base64-encoded data may be longer than original

float temp = 0.0;
float hum = 0.0;
float readings[4] = {0.0};

String data;

void setup()
{
  Serial.begin(115200);

  LoRa.setPins(ss, rst, dio0);
  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa initialization failed");
    while (1);
  }

  Wire.begin();
  randomSeed(analogRead(A0));

  if (!ADS.begin()) {
    Serial.println("Failed to initialize ADS1115");
    while (1);
  }

  Serial.println("System initialized");
}

void loop()
{
  for (int i = 0; i < 4; i++) {
    int16_t val = ADS.readADC(i);
    readings[i] = val * ADS.toVoltage(1) * 1000;
  }

  temp = sht.getTemperature();
  hum = sht.getHumidity();

  data = "PlantaPiloto/AS3_Pablo#" + String(temp, 2) + "," + String(hum, 2);
  for (int i = 0; i < 4; i++) {
    data += "," + String(readings[i], 3);
  }

  memset(normal_text, 0, sizeof(normal_text));
  memset(base64_text, 0, sizeof(base64_text));
  data.toCharArray((char*)normal_text, sizeof(normal_text) - 1);

  int base64_length = encode_base64(normal_text, strlen((char*)normal_text), base64_text);

  Serial.print("Base64: ");
  Serial.println((char*) base64_text);
  Serial.print("Base64 Length: ");
  Serial.println(base64_length);

  LoRa.beginPacket();
  LoRa.write(base64_text, base64_length);
  LoRa.endPacket();

  delay(random(500, 5000));
}
