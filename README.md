
## UYARI

BU KÜTÜPHANENİN FİXAJ MAĞAZALARI DIŞINDA ALINAN DOKUNMATİK EKRANLARDA TİCARİ OLARAK KULLANIMI YASAKTIR.

#### Satın Almak için Mağazamızı Ziyaret Edebilirsiniz-> https://shop.fixaj.com/dokunmatik-ekran

  

## KÜTÜPHANE TANITIM VİDEOSU

[![fixaj Youtube](https://i.ytimg.com/an_webp/pP-oeIcGcR4/mqdefault_6s.webp?du=3000&sqp=COCupaQG&rs=AOn4CLBrrfHx3Uv3H8P1ub0iTj7s6HKUlA)](http://www.youtube.com/watch?v=pP-oeIcGcR4 "Ekran Kütüphanesi Kullanımı")
## API Kullanımı

Kütüphaneyi indirip Arduinu İDE ye kurduktan sonra Örnek uygulamalar üzerinden işe başlayabilirsiniz. Uygulamaları Basit, Orta ve Uzman şeklinde sınıflandırdım Basit kategorisinde Arduina da SoftwareSerial olarak ve Hardware Serial olarak aynı örneğin çalışma kodlarını yükledim. Ayrıca örneği çalıştıracağınız 4.3 inch ve 7 inch için aynı tasarımı da github dan indirdiğiniz kütüphane içinde örnekler/Basit klasörü içinde bulabilirsiniz.

#### Başlıca Fonksiyonlar

```http
  "yaz" fonksiyonu
```

bu fonksiyon ile ekran hafızasındaki bir adrese istediğiniz tipde bir değeri kayıt etmenizi sağlar ve bu değer globaldir. İstediğiniz yerden ulaşılabilir. 

| Fonksiyon | Dönüş Tipi       | Açıklama/ Örnek                |
| :-------- | :------- | :------------------------- |
| `bool yaz(INT32U Addr, INT8U* Data)` | `unsigned char ` | INT8U data[] = "hello";  yaz(adr, data); |
| `bool yaz(INT32U Addr, char* Data)` | `char` | char data[] = "hello";   yaz(adr, data); |
| `bool yaz(INT32U Addr, String abc) ` | `string` | String abc = "hello";    yaz(adr, abc) |
| `bool yaz(INT32U Addr, const char* abc)` | `string` | yaz(adr, "hello"); |

Yine aynı `yaz` fonksiyonu ile sayısal değerlerde bir adrese yazdırılabilir. Tabiki burada sayıların minimum ve maximum değerleri kullanılan çipe göre NANO veya ESP32 ye göre değişkenlik gösterebiliri.


| Fonksiyon | Dönüş Tipi       | Açıklama/ Örnek                |
| :-------- | :------- | :------------------------- |
| `bool yaz(INT32U Addr, int xyz) ` | `int` | -32,768 ile 32,767 |
| `bool yaz(INT32U Addr, unsigned int xyz)` | `unsigned int` | 0 ile 65,535 |
| `bool yaz(INT32U Addr, long xyz) ` | `long` |   |
| `bool yaz(INT32U Addr, uint64_t xyz)` | `uint64_t` |   |

```http
  "oku" fonksiyonu
```
Bir diğer önemli fonksiyonumuz ise okuma fonksiyonudur. Yine aynı fonksiyon ile sayısal verileri Ekrandaki belirli bir adresten okuma yapabilirsiniz. Bütün sayı tipleri desteklenmeye çalışmıştır.


| Fonksiyon | Dönüş Tipi       | Açıklama/ Örnek                |
| :-------- | :------- | :------------------------- |
| `bool oku(INT32U Addr, int* deger) ` | `int` | int b = 0; oku(adr, &b); |
| `bool oku(INT32U Addr, long* deger)` | `long` | long b=0; oku(adr, &b);|


```http
  "okuStr" fonksiyonu
```

 Bu fonksiyon ise Ekran üzerinden yazıları okumanızı sağlayacaktır. Örnek kullanımları şu şekildedir.

 | Fonksiyon | Dönüş Tipi       | Açıklama/ Örnek                |
| :-------- | :------- | :------------------------- |
| `okuStr(INT32U Addr, String* deger) ` | `String` | String abc;       okuStr(adr, &abc); |
| `okuStr(INT32U Addr, char* deger)` | `char` | char okuma[64];   okuStr(adr, okuma);|

#### Diğer Fonksiyonlar

 | Fonksiyon |  Açıklama/ Örnek                |
| :-------- |  :------------------------- |
| `   bool EkranTest() ` |   ekran bağlantılarını kontrol eder |
| `bool dokunmaDinle();` |  basılan butonun id bilgilerini verir|
| `  bool butonBasildiMi(const Buton& btn);` |  Oluşturulan Buton nesnesi ile basılan buton aynı mı kontrol eder.|
| `void EkranParlaklikSeviyesi(INT8U ucPWMLevel);` | Ekran parlıklığını Ayarlar|
| `void BuzzerSesSeviyesi(INT8U ucLevel);` |  Buzzer sesini Ayarlar|
| ` BOOLEAN SaatTarihAyarla(INT8U Year, INT8U Month, INT8U Date, INT8U Hour, INT8U Minute, INT8U Second);` |  Ekranın RTC saatini ayarlarmak için|
| `BOOLEAN OnplanRengiDegis(INT8U elementType, INT16U pageID, INT8U elementID, INT16U foreGroundcolor);` |  Bir nesnenin Ön planını değiştirmek için|
| `BOOLEAN ArkaPlanRengiDegis(INT8U elementType, INT16U pageID, INT8U elementID, INT8U transparentMode, INT16U backGroundcolor);` |  Bir nesnenin Arka planını değiştirmek için|
| `BOOLEAN grafikCiz(INT32U Addr, INT16U Length, INT16U* Data);` |   Grafik arayüz oluşturmak için|
| `BOOLEAN grafikCiz(INT32U Addr, int Length, int* Data); ` |  Grafik arayüz oluşturmak için|
 

  
## İletişim

- Bu Kütüphane Mehmet Yıldız Tarafından Hazırlanmıştır.

- Yer: Şanlıurfa
- web: https://fixaj.com/
- mail: destek@fixaj.com
- Youtube : https://www.youtube.com/fixaj
- Instagram : https://www.instagram.com/fixajcom/
- Linkedin : https://www.linkedin.com/in/mehmet-yildiz-65934829/



  
## Destek

Destek için ve aklınıza takılan sorular için https://fixaj.com/sorucevap/ sayfamızı ziyaret edebilirsiniz.

Ürünler için indirme Dosyalarını https://fixaj.com/dokunmatik-ekranlar/ sayfamızdan indirebilirsiniz.

  
## Test Edilen Cihazlar

- ESP32 30 pin Dev Kit V1
- Arduino NANO

  
## Satın Alma Sayfaları

- [4.3 inch Kapasitif Dokunmatik Ekran Seti](https://shop.fixaj.com/full-set-43-inch-dokunmatik-ekran) Satın almak için Tıklayın.
- [7 inch Kapasitif Dokunmatik Ekran Seti](https://shop.fixaj.com/7-inch-full-set-kapasitif-dokunmatik-ekran) Satın almak için Tıklayın.

- [Fixaj Mağazamız](https://shop.fixaj.com/) LoRa ürün ekipmanlarını bulabilirsiniz.



  
![Logo](https://fixaj.com/wp-content/uploads/2021/01/cropped-fixaj-logo-1.png)

    