/*
  Bu kod Yildiz Roket Takimi tarafindan Model Roketciligin gelismesine katki saglamak amaciyla
  acik kaynakli olarak paylasilmistir.

  Bu kod, 2020 yilinda Teknofest 2020 Roket Yarismasinda ucus yapip 2.'lik elde eden 
  Semruk - 3 Roketine ait ana ucus bilgisayarinin kodudur.

  Kod hakkinda sorulariniz icin aviyonik birimi uyelerimiz ile iletisime gecebilirsiniz.
  (Uyelerimizin iletisim bilgileri icin: https://www.yildizroket.org/takim-uyeleri)
    
  Detayli Bilgi Icin: https://www.yildizroket.org/blog-semruk3-aviyonikvekurtarma
*/

#include <SPI.h>
#include <SD.h>
#include <LoRaLib.h>
#include <EEPROM.h>
#include <SFE_BMP180.h>
#include <Wire.h>
#include <TinyGPS++.h>
#include <SimpleKalmanFilter.h> 

// Ana-Yedek bilgisayar haberlesmeleri:
#define BMPcalisiyor 36 
#define GucVar 38 

#define Buzzer 32 // buzzer baglantisi

// Kullanilan modullerin kutuphanelerini kullanmak icin olusturulan objeler:
SX1278 lora = new LoRa; 
File dosya; 
TinyGPSPlus gps; 
SFE_BMP180 BMPSensor;
SimpleKalmanFilter kalmanFiltresi(1, 1, 0.1);

// Kullanilan pinler:
const byte csBaglantisi = 49; // SD Kart Chip Select Pin
const byte RFMBaglantisi = 53; // RFM98W NSS Pin
const byte DragP = 44, MainP = 46; // Suruklenme ve Ana parasut aktivasyon pinleri

// GPS modulunun fonksiyonlari icin tanimlar:
static const uint32_t GPSBaud = 9600; 
const long SERIAL_REFRESH_TIME = 10;  
long refresh_time, lastTransmission = 0;

// Kodda kullanilan degiskenler:
float ilkBasinc = 0;
float maksIrtifa = -2000, anlikIrtifa = 0,filtreliIrtifa=0;
unsigned int DragPrDiff = 15;
unsigned int MainPrDiff = 600; 
float veritipi;
bool roketDususte = false;

// BMP180 basinc olcum fonksiyonu:
double basincOku() {
  double sicaklik, basinc;
  delay(BMPSensor.startTemperature());
  BMPSensor.getTemperature(sicaklik);
  delay(BMPSensor.startPressure(3));
  BMPSensor.getPressure(basinc,sicaklik);
  return basinc;
}

void setup() 
{
  Serial.begin(9600); 
  Serial1.begin(GPSBaud); // GPS modulunun seri haberlesmesinin baslamasi.

  // Cikti pinlerin tanimlanmasi:
  pinMode(DragP, OUTPUT);
  pinMode(MainP, OUTPUT);
  pinMode(BMPcalisiyor, OUTPUT);
  pinMode(GucVar, OUTPUT);  
  pinMode(Buzzer, OUTPUT); 
  digitalWrite(DragP, LOW);
  digitalWrite(MainP, LOW);

  // BMP180 baslangic:
  if (BMPSensor.begin()){
    Serial.println("BMP180 init success");
    digitalWrite(Buzzer,HIGH);
    delay(500);
    digitalWrite(Buzzer,LOW);
    delay(2000);
  }
  else
  {
  	// BMP180 baglantisi basarisiz ise:
    Serial.println("BMP180 baglantisi basarisiz\n\n");
    digitalWrite(Buzzer,HIGH);
    delay(2000);
    digitalWrite(Buzzer,LOW);
    delay(1000);
    digitalWrite(Buzzer,HIGH);
    delay(2000);
    digitalWrite(Buzzer,LOW);
    while(1); // Programi sonsuza kadar durdur.
  }

  ilkBasinc = basincOku(); 

  // Eger EEPROM'a bir deger yazilmamis ise:
  if (EEPROM.get(0,veritipi) == 0){
    EEPROM.put(0,ilkBasinc); // ilk basinc degerini EEPROM'a yaz.
  }

  // Eger EEPROM'a bir deger yazilmis ise:
  else {
    ilkBasinc = EEPROM.get(0,veritipi); // Degeri EEPROM'dan al
  }
  
  // Slave Select olarak sd kart modulunu sec (SPI haberlesmesi):
  digitalWrite(csBaglantisi, LOW); digitalWrite(RFMBaglantisi, HIGH); 

  Serial.println("Sd kart baslatiliyor...");
  if (!SD.begin(csBaglantisi)) 
  {
    digitalWrite(Buzzer,HIGH);
    delay(2000);
    digitalWrite(Buzzer,LOW);
    delay(1000);
    digitalWrite(Buzzer,HIGH);
    delay(2000);
    digitalWrite(Buzzer,LOW);
    delay(2000);
    Serial.println("Sd kart baglantisi basarisiz!");
  }
  else {
  	// Mikro SD Kart baglantisi basarisiz ise:
    digitalWrite(Buzzer,HIGH);
    delay(500);
    digitalWrite(Buzzer,LOW);
    delay(2000);
  }

  // Slave Select olarak rfm98w modulunu sec (SPI haberlesmesi):
  digitalWrite(csBaglantisi, HIGH); digitalWrite(RFMBaglantisi, LOW);

  int state = lora.begin(); // LoRa'yi baslat ve begin methodunun dondurdugunu kaydet.

  if (state == ERR_NONE) // Eger durum "HATASIZ" ise: 
  {
    Serial.println(F("LoRa Baglantisi Basarili!")); 
    digitalWrite(Buzzer,HIGH);
    delay(500);
    digitalWrite(Buzzer,LOW);
    delay(2000);
  } 

  else // Degilse: 
  {
    Serial.print(F("LoRa Baglantisi Basarisiz! Hata Kodu: ")); 
    Serial.println(state); // hata kodu
    digitalWrite(Buzzer,HIGH);
    delay(2000);
    digitalWrite(Buzzer,LOW);
    delay(1000);
    digitalWrite(Buzzer,HIGH);
    delay(2000);
    digitalWrite(Buzzer,LOW);
    delay(2000);
    while (true); // sonsuza kadar durdur
  }

  lora.setFrequency(435.64); // LoRa (RFM98W) haberlesme frekansini 435.64 olarak ayarla.
}

void loop() 
{
  digitalWrite(GucVar,HIGH);

  if(BMPSensor.getError() != 0){
    digitalWrite(BMPcalisiyor, LOW);
  }
  else {
    digitalWrite(BMPcalisiyor, HIGH);
  }
    
  float basinc = basincOku();
  float irtifa = BMPSensor.altitude(basinc,ilkBasinc);
  float filtreliIrtifa = kalmanFiltresi.updateEstimate(irtifa); // okunan irtifayi Kalman Filtresinden gecir

  if (filtreliIrtifa > maksIrtifa) // Eger okunan irtifa maks irtifadan buyuk ise:
  {
    maksIrtifa = filtreliIrtifa; // Okunan irtifayi maks irtifa yap.
  }

  // Eger okunan irtifa ile maks irtifa arasi fark 15'den fazla ise:
  if (maksIrtifa - filtreliIrtifa > DragPrDiff && !roketDususte) 
  {
      Serial.println("Suruklenme Parasutleri Acildi!"); roketDususte = true; 
      digitalWrite(DragP,HIGH); // Suruklenme Parasutunu aktif et.
  }

  // Eger suruklenme parasutu acilmis ve okunan irtifa 600'un altinda ise:
  if (roketDususte == true && filtreliIrtifa < MainPrDiff)
  {
      Serial.println("Ana Parasut Acildi!");
      digitalWrite(MainP,HIGH); // Ana parasutu aktif et.
  }  
    
  // Slave Select olarak sd kart modulunu sec (SPI haberlesmesi):
  digitalWrite(csBaglantisi, LOW); digitalWrite(RFMBaglantisi, HIGH);
  
  dosya = SD.open("Veri_ana.txt", FILE_WRITE); // Sd kart dosyasini ac.

  if (dosya) // Eger dosya duzgun acildiysa:
  {
    // Verileri dosyaya yazdir:
    dosya.print("Irtifa: "); dosya.print(filtreliIrtifa);
    dosya.print("|| Uydu Sayisi: "); dosya.print(gps.satellites.value(), 7); 
    dosya.print(" Irtifa(GPS): "); dosya.print(gps.altitude.meters(), 7);
    dosya.print(" Enlem: "); dosya.print(gps.location.lat(), 7);
    dosya.print(" Boylam: "); dosya.print(gps.location.lng(), 7);
    dosya.println(""); 

    dosya.close(); // dosyayi kapat.
  } 

  else 
  {
    Serial.println("Dosyayi acarken bir problem olustu!!!");
    dosya.close(); // dosyayi kapat.
  }

  // Slave Select olarak rfm98w modulunu sec (SPI haberlesmesi):
  digitalWrite(csBaglantisi, HIGH); digitalWrite(RFMBaglantisi, LOW);

  // Veri olarak gonderilecek string'in olusturulmasi:
  String bmpVeri= String(filtreliIrtifa), gpsEn = String(gps.location.lat(),6),gpsBoy = String(gps.location.lng(),6), spd = String(gps.speed.kmph(),6);
  String toplamVeri ="YRT,R," + bmpVeri + "," + spd + "," + gpsEn + "," + gpsBoy + ";";

  lora.transmit(toplamVeri); // olusturulan veriyi gonder.
  
  smartDelay(50); // 50ms'lik delay
}


//*** GPS ve Delay için fonksiyonlar...***

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (Serial1.available())
      gps.encode(Serial1.read());

  } while (millis() - start < ms);
}
