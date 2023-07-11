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

#include <FixajEkran.h>
FixajEkran FixajSS(&Serial, UART_BPS_RATE_115200_Ekran);

unsigned long kanalBekleme_sure = 0;
int kanalBekleme_bekleme = 2000;

#define temp 0x080000
#define nem 0x080002
#define yazi 0x000080

Buton Anasayfa(0, 0);
Buton Ayarlar(0, 1);
Buton Geri(1, 0);

int a = 0, b = 0;

void setup() {
  FixajSS.begin();
  delay(500);
  if (!FixajSS.EkranTest()) {
    Serial.println("bağlantı hatası, kabloları kontrol edin");  //ESP yi veya NANO yu PCB den çıkarıp programı yükleyin sonra PCB takın
                                                                //Ekran nın Baud Rate ni de ayarlamayı unutmuş olabilirsiniz.
                                                                //SGTools programından projenize sağ tıklayın Prpject properties-> Baud rate
                                                                //sonra arayüz tasarımını tekrar Ekrana yükleyin.
  }
  delay(100);
  Serial.println("Fixaj.com Basliyor...");
}
void loop() {

  if (FixajSS.dokunmaDinle()) {
    if (FixajSS.butonBasildiMi(Anasayfa)) {
      Serial.println("Anasayfa click");
    } else if (FixajSS.butonBasildiMi(Ayarlar)) {
      Serial.println("Ayarlar click");
    } else if (FixajSS.butonBasildiMi(Geri)) {
      Serial.println("Geri click");
    }
  }

  if (millis() > kanalBekleme_sure + kanalBekleme_bekleme) {
    kanalBekleme_sure = millis();

    a++;
    if (a < 30) {

      FixajSS.yaz(temp, a * 7);
      FixajSS.yaz(nem, a * 3);
      FixajSS.yaz(yazi, "Dokunmatik Ekranlar Geliyor..");

    } else {
      a = 0;
    }

    FixajSS.oku(temp, &b);
    Serial.print(a);
    Serial.print(",b =");
    Serial.println(b);
  }
}
