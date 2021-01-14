/*
  Bu kod Yildiz Roket Takimi tarafindan Model Roketciligin gelismesine katki saglamak amaciyla
  acik kaynakli olarak paylasilmistir.
  
  Bu kod Mikro SD Kart Modulunun Arduino Mega ile kullanimi icin yazilmis ornek bir koddur. 
  
  Detayli Bilgi Icin: https://www.yildizroket.org/blog-semruk3-aviyonikvekurtarma
*/

#include <SD.h>

File dosya;

const byte sdCS = 49;

void setup() 
{
  Serial.begin(9600);

  pinMode(sdCS, OUTPUT);

  if (!SD.begin(sdCS)) 
    Serial.println("Mikro SD kart modulu basirili bir sekilde baslatildi!");

  else
  {
    Serial.println("Mikro SD kart modulu baslatilirken bir problem olustu!");
    while(1);
  }
}

void loop() 
{
  dosya = SD.open("sdTest.txt", FILE_WRITE);
  dosya.println("Zaman: " + String(millis() / 1000.0));

  dosya.close();
  delay(750);
}
