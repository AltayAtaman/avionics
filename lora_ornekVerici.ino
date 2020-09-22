/*
   Bu kod Yildiz Roket Takimi tarafindan Turkiye Cumhuriyetinde Model Roketciligin gelismesinde katki saglamak amaciyla
  acik kaynakli olarak paylasilmistir.

  Bu kod Hope RFM98W LoRa modulunun kullanimi icin yazilmis ornek bir koddur. 
*/

#include <LoRaLib.h>

SX1278 lora = new LoRa;
const unsigned int frekans = 373.2812; 
unsigned int sayac = 0;

void setup() 
{
  Serial.begin(9600);

  int state = lora.begin(); // LoRa'yi baslat ve begin methodunun dondurdugunu kaydet.
  if (state == ERR_NONE) // EGer state "HATASIZ" ise: 
  {
    lora.setFrequency(frekans);
    Serial.println("RFM98W LoRa Modulu Basarili Bir Sekilde Baslatildi!");
  } 

  else // Degilse: 
    Serial.println("RFM98W LoRa Modulu Baslatilamadi!!! Hata Kodu: " + String(state));
}

void loop() 
{
  lora.transmit("Test Paketi " + String(sayac));
  sayac++;
  delay(750);
}