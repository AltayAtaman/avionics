/*
  Bu kod Yildiz Roket Takimi tarafindan Model Roketciligin gelismesine katki saglamak amaciyla
  acik kaynakli olarak paylasilmistir.

  Bu kod GY-NEO6MV2 modulunun kullanimi icin yazilmis ornek bir koddur.
  Bu ornek kod Arduino Mega ve Arduino Mega Pro icin yazilmistir. 

  GY-NEO6MV2 GPS Baglantilari:
    Vin: 3.3V
    Gnd: Gnd
    RX: 18
    TX: 19
    
  Detayli Bilgi Icin: https://www.yildizroket.org/blog-semruk3-aviyonikvekurtarma
*/

#include <TinyGPS++.h>

TinyGPSPlus gps; 
const int gpsBaud = 9600;

void setup() 
{
  Serial1.begin(gpsBaud);
}

void loop() 
{
  String loraVeri = String(gps.location.lat(), 6) + "," + String(gps.location.lng(), 6) + ";"; 

  Serial.println("Uydu Sayisi: " + String(gps.satellites.value()) + " Enlem: " + 
    String(gps.location.lat(), 6) + " Boylam: " + String(gps.location.lng(), 6));

  smartDelay(150);
}

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (Serial1.available())
      gps.encode(Serial1.read());

  } while (millis() - start < ms);
}
