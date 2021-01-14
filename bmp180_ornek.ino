/*
  Bu kod Yildiz Roket Takimi tarafindan Model Roketciligin gelismesinde katki saglamak amaciyla
  acik kaynakli olarak paylasilmistir.

  Bu kod BMP180 modulunun kullanimi icin yazilmis ornek bir koddur. 

  BMP180 Baglantilari:
    Vin: 3.3V
    Gnd: Gnd
    SCL: A5 (Uno/Nano), 21 (Mega/Mega Pro)
    SDA: A4 (Uno/Nano), 20 (Mega/Mega Pro)
*/

#include <SFE_BMP180.h>
#include <Wire.h>
 
SFE_BMP180 sensor; 
double tabanBasinci = 0, irtifa = 0, maxIrtifa = 0; 

void setup() 
{   
  Serial.begin(9600);
   
  if (!sensor.begin())  
  {
    Serial.println("BMP180 Pressure Sensor Error!"); 
    while(1);  
  }
  tabanBasinci = getPressure(); 
} 
   
void loop() 
{   
  irtifa = irtifaHesapla();

  if (irtifa > maxIrtifa) 
    maxIrtifa = irtifa; 
    
  Serial.println("Anlik Irtifa: " + String(irtifa) + ", Olculen Maksimum Irtifa: " + String(maxIrtifa));

  delay(50); 
}
  
double getPressure() 
{
  char status= ' '; 
  double sicaklik = 0, basinc = 0;
    
  status = sensor.startTemperature(); 
  if (status != 0)
  {
    delay(status); 
    status = sensor.getTemperature(sicaklik);  
    status = sensor.startPressure(3); 

    if (status != 0) 
    {
      delay(status);
      status = sensor.getPressure(basinc, sicaklik);
  
      if (status != 0)
            return basinc;
    }  
  }
}

double irtifaHesapla()
{
  return sensor.altitude(getPressure(), tabanBasinci); 
}
