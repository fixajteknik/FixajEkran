/****************************************Copyright (c)****************************************************
**                             		Fixaj  Mehmet YILDIZ
**
**                             		https://fixaj.com/  
**
** Tüm hizmetler Mehmet Yıldız şirketi Fixaj tarafından sağlanmaktadır.
** FİXAJ MAĞAZALARIMIZ DIŞINDA SATIN ALINAN TOPWAY MARKA DOKUNMATİK EKRANLARDA BU KÜTÜPHANENİN TİCARİ OLARAK KULLANIMI YASAKTIR.
** Herhangi bir program, belge, test sonucu, çözüm, destek ve diğer malzeme ve bilgiler yalnızca referans amaçlıdır
** ve müşterilerin kullanmama veya atıfta bulunma ve kendileri tarafından değişiklik yapma hakları vardır.
** Müşterinin kullanımı sırasında herhangi bir özel, tesadüfi veya dolaylı kayıp meydana gelirse, 
** herhangi bir bütünlük, güvenilirlik ve diğer garantileri sağlamak için şirket sorumlu olmayacaktır.
** HERHANGİ BİR SORUMLULUĞUN KABUL EDİLMEMESİ.
**                                                                        Fixaj
**
**
**--------------File Info-------------------------------------------------------------------------------
** Modified by:            	MEHMET YILDIZ
** Modified date:        	8.06.2023
** Version:                	v0.1
** Description:    
 
********************************************************************************************************/

#include <HardwareSerial.h>
#include <FixajEkran.h>

#include <Wire.h>                    //HDC1080 Kütüphanesi
#include "ClosedCube_HDC1080.h"

#include <Preferences.h>            //esp32 de EEPROM kullanabilmek için
                                    // değer saklamak için

Preferences preferences;
ClosedCube_HDC1080 hdc1080;

#define RX 17  //  Lora nın 3. pini RX in ESP 32 de hangi pine bağlı olduğu
#define TX 16  // Lora nın 4. pini TX in ESP 32 de hangi pine bağlı olduğu

HardwareSerial fixajSerial(2);  //esp32 hardware serial kullanıyoruz
FixajEkran FixajSS(TX, RX, &fixajSerial, UART_BPS_RATE_115200);

unsigned long kanalBekleme_sure = 0;
int kanalBekleme_bekleme = 2000;


//ekrandaki değişkenlerin adresleri
#define temp 0x080000
#define nem 0x080002
#define tempAyar 0x080004
#define nemAyar 0x080006
#define fan 0x080008
#define sprey 0x08000A
#define kaydetbasar 0x08000C
#define yazi 0x000080

//Ekran Buton Nesneleri
Buton kaydet(1, 1);
Buton Ayarlar(0, 0);
Buton Geri(1, 0);

#define saveSicaklik  "setsicakdgr"
#define saveNEm "setnemdgr"

int SETSicaklik = 0, SetNem = 0, OkunanSicaklik = 0, OkunanNem = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ;
  }
  delay(100);
  FixajSS.begin();
  delay(500);
  if (!FixajSS.EkranTest()) {
    Serial.println("bağlantı hatası, kabloları kontrol edin");  //ESP yi veya NANO yu PCB den çıkarıp programı yükleyin sonra PCB takın
                                                                //Ekran nın Baud Rate ni de ayarlamayı unutmuş olabilirsiniz.
                                                                //SGTools programından projenize sağ tıklayın Prpject properties-> Baud rate
                                                                //sonra arayüz tasarımını tekrar Ekrana yükleyin.
  }
  delay(100);

  hdc1080.begin(0x40);
  preferences.begin("fixaj", false);

  mevcutAyarlar(); // elektrik kesintisinden sonra bile en son kaydedilen değerle başlaması için
  Serial.println("Fixaj.com Basliyor...");
}

void loop() {

  if (FixajSS.dokunmaDinle()) {
    if (FixajSS.butonBasildiMi(kaydet)) {
      Serial.println("kaydet click");
      KAYDETayarlar();
    } else if (FixajSS.butonBasildiMi(Ayarlar)) {
      Serial.println("Ayarlar click");
      mevcutAyarlar();
    } else if (FixajSS.butonBasildiMi(Geri)) {
      Serial.println("Geri click");
    }
  }

  if (millis() > kanalBekleme_sure + kanalBekleme_bekleme) {
    kanalBekleme_sure = millis();
    OkunanSicaklik = (int)(round(hdc1080.readTemperature() * 10));
    Serial.print("T=");
    Serial.print(OkunanSicaklik);
    Serial.print("C, RH=");
    OkunanNem = (int)(round(hdc1080.readHumidity()));
    Serial.print(OkunanNem);
    Serial.println("%");

    FixajSS.yaz(temp, OkunanSicaklik);
    FixajSS.yaz(nem, OkunanNem);

    if (OkunanSicaklik > (SETSicaklik * 10)) {
      FixajSS.yaz(fan, 1);
      Serial.println("fan aç");
    } else {
      FixajSS.yaz(fan, 0);
      Serial.println("fan kapat");
    }

    if (OkunanNem < SetNem) {
      FixajSS.yaz(sprey, 1);
    } else {
      FixajSS.yaz(sprey, 0);
    }
  }
}

void KAYDETayarlar() {
  if (!FixajSS.oku(tempAyar, &SETSicaklik)) {
    Serial.println("Okuma Hatası");
    return;
  }

  if (!FixajSS.oku(nemAyar, &SetNem)) {
    Serial.println("Okuma Hatası");
    return;
  }

  preferences.putInt(saveSicaklik, SETSicaklik);
  preferences.putInt(saveNEm, SetNem);
  // Close the Preferences
  preferences.end();

  Serial.printf("Sicaklik: %dC, Nem : %d    \n", SETSicaklik, SetNem);
  FixajSS.sayfayaGit(1);
  FixajSS.yaz(kaydetbasar, 1);
}

void mevcutAyarlar() {
  SETSicaklik = preferences.getInt(saveSicaklik, 20);
  SetNem = preferences.getInt(saveNEm, 55);
  FixajSS.yaz(tempAyar, SETSicaklik);
  FixajSS.yaz(nemAyar, SetNem);
}
