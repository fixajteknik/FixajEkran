/****************************************Copyright (c)****************************************************
**                       FIXAJ, ARDUINO TOPWAY HMI KÜTÜPHANESİ
**
**                                 www.fixaj.com
**
**              instagram:      fixajcom
**              youtube:        fixaj
**              n11 Mağazası:   AlternatifTicaret
**              mail:           destek@fixaj.com
**
**    Bu örnekte kolayca okuma ve yazma yapacağız. HMI ekranda tasarımda tanımlanmış olan
**   0x080002 adresine 1453 sayısını yazıp, sonra o adresteki değeri sayıp "b" değişkenine
**   atayacağız. Sonrada Seri ekrana bu b değerini yazdıracağız.
**
**    Saygılarımla
**                                                                        ——Mehmet YILDIZ
**
**
*********************************************************************************************************/

#include<FixajEkran.h>

FixajEkran fixaj(Serial);
int yazilacakDeger = 0, OkunanDeger = 0;

void setup() {
  fixaj.begin(9600);

  yazilacakDeger = 1453;
  fixaj.yaz(0x080002, yazilacakDeger);
  OkunanDeger = fixaj.say(0x080002);

  Serial.print("Okunan Değer = ");
  Serial.println(OkunanDeger);

}

void loop() {

  
}
