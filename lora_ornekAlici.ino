/*
   Bu kod Yildiz Roket Takimi tarafindan Model Roketciligin gelismesine katki saglamak amaciyla
  acik kaynakli olarak paylasilmistir.

  Bu kod Hope RFM98W LoRa modulunun kullanimi icin yazilmis ornek bir koddur. 
  
  Detayli Bilgi Icin: https://www.yildizroket.org/blog-semruk3-aviyonikvekurtarma
*/

#include <LoRaLib.h>

SX1278 lora = new LoRa;
const unsigned int frekans = 373.2812; 

void setup() 
{
  Serial.begin(9600);

  int state = lora.begin(); // LoRa'yi baslat ve begin methodunun dondurdugunu kaydet

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
  String str;
  int state = lora.receive(str);

  if (state == ERR_NONE)
  {
    Serial.print(str);
  } 
    
  else if (state == ERR_RX_TIMEOUT) 
  {
    Serial.print(F("Zaman Asimi!\n"));
  } 

  else if (state == ERR_CRC_MISMATCH) 
    Serial.print(F("CRC hatasi!\n"));

  delay(150);
}
