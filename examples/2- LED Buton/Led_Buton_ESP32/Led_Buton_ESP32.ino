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

unsigned long kanalBekleme_sure = 0;
int kanalBekleme_bekleme = 2000;

#define sayi1 0x080002
#define sayi2 0x020000
#define sayi3 0x030000
#define yaz1 0x000080

#define RX 17  //  Lora nın 3. pini RX in ESP 32 de hangi pine bağlı olduğu
#define TX 16  // Lora nın 4. pini TX in ESP 32 de hangi pine bağlı olduğu

#define kirmiziPin 32
#define yesilPin 33
#define maviPin 27

HardwareSerial fixajSerial(2);  //esp32 hardware serial kullanıyoruz
FixajEkran FixajSS(TX, RX, &fixajSerial, UART_BPS_RATE_115200_Ekran);


Buton KirmiziLED(0, 0);
Buton YesilLED(0, 1);
Buton MaviLED(0, 2);
Buton keypadOK(0, 15);
Buton keypadESC(0, 3);
Buton keypadTest(0, 16);

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
  Serial.println("Fixaj.com Basliyor...");

  pinMode(kirmiziPin, OUTPUT);
  pinMode(yesilPin, OUTPUT);
  pinMode(maviPin, OUTPUT);
}

void loop() {

  if (FixajSS.dokunmaDinle()) {
    if (FixajSS.butonBasildiMi(KirmiziLED)) {
      Serial.println("Kırmızı click");
      digitalWrite(kirmiziPin, HIGH);
      digitalWrite(yesilPin, LOW);
      digitalWrite(maviPin, LOW);
    } else if (FixajSS.butonBasildiMi(YesilLED)) {
      Serial.println("Yeşil click");
      digitalWrite(kirmiziPin, 0);
      digitalWrite(yesilPin, 1);
      digitalWrite(maviPin, 0);
    } else if (FixajSS.butonBasildiMi(MaviLED)) {
      Serial.println("Mavi click");
      digitalWrite(kirmiziPin, 0);
      digitalWrite(yesilPin, 0);
      digitalWrite(maviPin, 1);
    } else if (FixajSS.butonBasildiMi(keypadOK)) {
      Serial.println("ok click");
    } else if (FixajSS.butonBasildiMi(keypadESC)) {
      Serial.println("esc click");
    } else if (FixajSS.butonBasildiMi(keypadTest)) {
      Serial.println("test click");
      digitalWrite(kirmiziPin, 0);
      digitalWrite(yesilPin, 0);
      digitalWrite(maviPin, 1);
    }
  }
}
