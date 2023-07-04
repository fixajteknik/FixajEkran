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
#include <vector>

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
FixajEkran FixajSS(TX, RX, &fixajSerial, UART_BPS_RATE_115200);

std::vector<Buton> buttons; // this way we can loop through our vector of "buton"s
buttons.push_back(Buton(0, 0));      // KirmiziLED
buttons.push_back(Buton(0, 1));      // YesilLED
buttons.push_back(Buton(0, 2));      // MaviLED
buttons.push_back(Buton(0, 15));     // keypadOK
buttons.push_back(Buton(0, 3));      // keypadESC
buttons.push_back(Buton(0, 16));     // keypadTest


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

  // Attach callback functions to handle button click events
  buttons[0].onClick([]() {
    Serial.println("Kırmızı click");
    digitalWrite(kirmiziPin, HIGH);
    digitalWrite(yesilPin, LOW);
    digitalWrite(maviPin, LOW);
  });

  buttons[1].onClick([]() {
    Serial.println("Yeşil click");
    digitalWrite(kirmiziPin, LOW);
    digitalWrite(yesilPin, HIGH);
    digitalWrite(maviPin, LOW);
  });

  buttons[2].onClick([]() {
    Serial.println("Mavi click");
    digitalWrite(kirmiziPin, LOW);
    digitalWrite(yesilPin, LOW);
    digitalWrite(maviPin, HIGH);
  });

  buttons[3].onClick([]() {
    Serial.println("ok click");
  });

  buttons[4].onClick([]() {
    Serial.println("esc click");
  });

  buttons[5].onClick([]() {
    Serial.println("test click");
    digitalWrite(kirmiziPin, LOW);
    digitalWrite(yesilPin, LOW);
    digitalWrite(maviPin, HIGH);
  });
}

void loop() {
  for (auto& buton : buttons) {
    buton.checkState();
  }
}
