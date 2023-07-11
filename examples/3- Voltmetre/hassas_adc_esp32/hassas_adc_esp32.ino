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
#include <stdio.h>
#include <stdlib.h>
#include "driver/adc.h"
#include "esp_adc_cal.h"

unsigned long kanalBekleme_sure = 0;
int kanalBekleme_bekleme = 2000;

static esp_adc_cal_characteristics_t adc1_chars;
uint32_t voltage;

#define graf 0x060000

#define RX 17  //  Lora nın 3. pini RX in ESP 32 de hangi pine bağlı olduğu
#define TX 16  // Lora nın 4. pini TX in ESP 32 de hangi pine bağlı olduğu


#define SIZE 269        // Array boyutu
#define OFFSET 0        // Orta değer (genliği 2'ye böleriz)
#define AMPLITUDE 1000  // Maksimum genlik

int sin_wave[SIZE];  // Sinüs dalgası için array

HardwareSerial fixajSerial(2);  //esp32 hardware serial kullanıyoruz
FixajEkran FixajSS(TX, RX, &fixajSerial, UART_BPS_RATE_115200_Ekran);

char AIN = 0;
char i;
INT16U Alog[269];


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

  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_6, ADC_WIDTH_BIT_12, 0, &adc1_chars);
  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_6);  // ADC1_CH0 :GPIO 36 pinini temsil eder.
}

void loop() {


  for (int i = 0; i < SIZE; i++) {
    //sin_wave[i] = round(OFFSET + AMPLITUDE * sin(2 * PI * i / SIZE));
    int adc_value = adc1_get_raw(ADC1_CHANNEL_0);
    voltage = esp_adc_cal_raw_to_voltage(adc_value, &adc1_chars);

    /*
    ESP32 ler lineer olmayan bir ADC sistemine sahip oldukları için çok hassas ADC okuması yapamazlar 
    ancak hatayı minimuma indirmek için okunacak volt aralığını 107mV ile 1650mV değer aralığına
    çekersek bu aralıktaki değerleri standard 0-3.3v aralığına göre daha sağlıklı okunduğu görülmektedir.

    bu teknik ile ADC1_CH0 :GPIO 36 pinini önüne bir gerilim bölücü koyularak 0-30V -> 0-1.6 V a düşürülerek
    ölçümleme yapılabilir. Ancak yinede ESP32 yi ben direk olarak ADC olarak kullanmanızı tavsiye etmem bunun
    yerine ADS1115 modülünü kullanmanızı tavsiye ederim

    Faydalı linkler:
    https://esp32tutorials.com/esp32-adc-esp-idf/
    https://docs.espressif.com/projects/esp-idf/en/v4.1.1/api-reference/peripherals/adc.html //hatalı bilgiler var
    https://lastminuteengineers.com/esp32-basics-adc/
    https://github.com/G6EJD/ESP32-ADC-Accuracy-Improvement-function/blob/master/ESP32_Read_Voltage_Simple.ino
    https://www.youtube.com/watch?v=UAJMLTzrM9Q
    */

    int finalValue = map(voltage, 107, 1650, 0, 2000);
    sin_wave[i] = finalValue;
    delay(1);
  }

  FixajSS.grafikCiz(graf, 269, sin_wave);
}
