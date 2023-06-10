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

#define graf 0x060000

#define RX 17  //  Lora nın 3. pini RX in ESP 32 de hangi pine bağlı olduğu
#define TX 16  // Lora nın 4. pini TX in ESP 32 de hangi pine bağlı olduğu 

#define SIZE 269        // Array boyutu
#define OFFSET 0        // Orta değer (genliği 2'ye böleriz)
#define AMPLITUDE 1000  // Maksimum genlik

int sin_wave[SIZE];  // Sinüs dalgası için array  
INT16U Alog[SIZE];

HardwareSerial fixajSerial(2);  //esp32 hardware serial kullanıyoruz
FixajEkran FixajSS(TX, RX, &fixajSerial, UART_BPS_RATE_115200);

 

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

  analogReadResolution(10);
}

void loop() {
 /*
  for (int i = 0; i < SIZE; i++) {
    Alog[i] = analogReadMilliVolts(32);
    delay(1);
  }
    FixajSS.grafikCiz(graf, SIZE, Alog);
*/

//grafiği çizdirdiğinizde sinüs ün negatif değerleri grafiğin altında kaldığı için görünmeyecektir
//siz arayüz programında grafik değer aralığını negatif değerler yapınca görünecektir.
//grafik tasarımı öncelikli olarak DC voltmetre olarak tasarlandığı için 

  for (int i = 0; i < SIZE; i++) {
    sin_wave[i] = round(OFFSET + AMPLITUDE * sin(2 * PI * i / SIZE));
  }

  FixajSS.grafikCiz(graf, SIZE, sin_wave);

 
}
