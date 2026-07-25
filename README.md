# Semruk-3 Aviyonik ve Ana Uçuş Bilgisayarı

![TEKNOFEST 2020](https://img.shields.io/badge/TEKNOFEST-2020%20İkincisi-orange?style=for-the-badge)
![Platform](https://img.shields.io/badge/Platform-Arduino%20Mega%20%2F%20Uno-blue?style=for-the-badge)
![License](https://img.shields.io/badge/Lisans-Açık%20Kaynak-green?style=for-the-badge)

Bu depo, [Yıldız Roket Takımı](https://yildizroket.com/tr) tarafından geliştirilen ve TEKNOFEST 2020 Roket Yarışması'nda 2.'lik elde eden Semruk - 3 roketine ait ana uçuş bilgisayarı kodlarını ve aviyonik alt sistem örneklerini içermektedir.

Model roketçiliğin gelişmesine katkı sağlamak amacıyla açık kaynak olarak paylaşılmıştır.

---

## İçindekiler
- [Proje Hakkında](#proje-hakkında)
- [Donanım Özellikleri](#donanım-özellikleri)
- [Yazılım Mimarisi ve Modüller](#yazılım-mimarisi-ve-modüller)
- [Depo Dosya Yapısı](#depo-dosya-yapısı)
- [Pin Haritası ve Bağlantılar](#pin-haritası-ve-bağlantılar)
- [Gerekli Kütüphaneler](#gerekli-kütüphaneler)
- [Kurulum ve Kullanım](#kurulum-ve-kullanım)
- [Kaynaklar ve İletişim](#kaynaklar-ve-iletişim)

---

## Proje Hakkında

Semruk-3 Ana Uçuş Bilgisayarı; roketin fırlatılış anından itibaren irtifa, konum ve basınç verilerini işler, telemetri paketlerini yer istasyonuna LoRa üzerinden kablosuz aktarır ve iki aşamalı kurtarma sistemini (Sürüklenme ve Ana Paraşüt) güvenli bir şekilde tetikler.

Gürültülü sensör verilerini süzmek ve apogee (tepe noktası) tespitini hatalı tetiklemelerden korumak için 1D Kalman Filtresi entegre edilmiştir.

---

## Donanım Özellikleri

- Mikrodenetleyici: Arduino Mega 2560 (Ana Bilgisayar) / Arduino Uno (Test ve Yardımcı Modüller)
- Barometrik Basınç ve İrtifa Sensörü: Bosch BMP180 (I2C)
- Konum Belirleme (GPS): NEO-6M GPS Modülü (Hardware Serial - 9600 Baud)
- Kablosuz Telemetri (LoRa): HopeRF RFM98W / SX1278 (373.28 MHz SPI LoRa)
- Veri Depolama: SD Kart SPI Modülü (Uçuş Veri Kaydı)
- Kurtarma Sistemleri: 2 Kanal Pyrotechnic Ejection Trigger (Drogue & Main Parachute)
- Yedeklilik ve Sağlık Sinyalleri: Ana/Yedek bilgisayar durum kontrol pinleri

---

## Depo Dosya Yapısı

```
.
├── semruk3_anaBilgisayar.ino   # Semruk-3 ana uçuş bilgisayarı yazılımı
├── bmp180_ornek.ino            # BMP180 basınç/irtifa sensörü test kodu
├── gps_ornekMEGA.ino           # Arduino Mega için GPS okuma testi (Serial1)
├── gps_ornekUNO.ino           # Arduino Uno için GPS okuma testi (SoftwareSerial)
├── lora_ornekAlici.ino         # LoRa alıcı (Receiver) test kodu
├── lora_ornekVerici.ino        # LoRa verici (Transmitter) test kodu
├── sdKartOrnek_Mega.ino        # Arduino Mega SD kart okuma/yazma testi
└── sdKartOrnek_Uno.ino         # Arduino Uno SD kart okuma/yazma testi
```

---

## Pin Haritası ve Bağlantılar

### Ana Uçuş Bilgisayarı (Arduino Mega 2560)

| İşlev / Modül | Pin / Bağlantı | Açıklama |
| :--- | :--- | :--- |
| Drag Parachute (Sürüklenme) | Pin 44 | Tepe noktasında sürüklenme paraşütü tetikleme |
| Main Parachute (Ana Paraşüt) | Pin 46 | Belirlenen irtifada ana paraşüt tetikleme |
| SD Kart CS (Chip Select) | Pin 49 | SD Kart SPI Seçim Pini |
| LoRa NSS (CS) | Pin 53 | RFM98W / SX1278 SPI Seçim Pini |
| Buzzer | Pin 32 | Sistem durum ve sesli uyarı çıkışı |
| BMP Çalışıyor Sinyali | Pin 36 | Yedek bilgisayara BMP durum sinyali |
| Güç Durumu Sinyali | Pin 38 | Güç kontrol durum çıkışı |
| GPS (NEO-6M) | Serial1 (RX1/TX1) | 9600 Baud Hardware Serial |
| BMP180 (I2C) | SDA (Pin 20) / SCL (Pin 21) | Basınç ve Sıcaklık Sensörü |

---

## Gerekli Kütüphaneler

Kodun derlenebilmesi için Arduino IDE üzerinde aşağıdaki kütüphanelerin yüklü olması gerekmektedir:

- SPI.h (Dahili)
- Wire.h (Dahili)
- SD.h (Dahili)
- EEPROM.h (Dahili)
- [SFE_BMP180](https://github.com/sparkfun/BMP180_Breakout_Arduino_Library)
- [TinyGPS++](https://github.com/mikalhart/TinyGPSPlus)
- [LoRaLib](https://github.com/jgromes/LoRaLib)
- [SimpleKalmanFilter](https://github.com/denyssene/SimpleKalmanFilter)

---

## Kurulum ve Kullanım

1. Depoyu klonlayın veya zip olarak indirin:
   ```bash
   git clone https://github.com/AltayAtaman/avionics.git
   ```
2. Gerekli Arduino kütüphanelerini Arduino IDE'ye ekleyin (Sketch -> Include Library -> Add .ZIP Library...).
3. Her bir sensörü tekil olarak test etmek için ilgili `_ornek.ino` dosyalarını kartınıza yükleyin.
4. Ana sistemi çalıştırmak için `semruk3_anaBilgisayar.ino` dosyasını Arduino Mega 2560 kartınıza yükleyin.

---

## Kaynaklar ve İletişim

- TEKNOFEST 2020 Roket Yarışması 2.'si — Semruk-3 Roketi
- Resmi Takım Web Sitesi: [https://yildizroket.com/tr](https://yildizroket.com/tr)

---
[Yıldız Roket Takımı](https://yildizroket.com/tr) tarafından açık kaynak olarak sunulmuştur.
