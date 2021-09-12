/****************************************Copyright (c)****************************************************
**                       FIXAJ, ARDUINO TOPWAY HMI KÜTÜPHANESİ
**
**                             		www.fixaj.com
**										
**							instagram: 		fixajcom
**							youtube: 		fixaj
**							n11 Mağazası:	AlternatifTicaret
**							mail:			destek@fixaj.com
**
**    Bu kütüphane TOPWAY marka HMI dokunmatik ekranların kolay kullanımı için hazırlanmıştır.
**  Çoğu kullanıcıya karmaşık gelebilecek RS232 komutları maskelenerek, basit ve anlaşılır kod
**  yapıları oluşturulmuştur. Güzel projeler yapmanız dileği ile.
**  
**	  Saygılarımla
**                                                                        ——Mehmet YILDIZ
**
**
**--------------File Info-------------------------------------------------------------------------------
** File Name:            Fixajhmi.h
** Modified by:          Mehmet YILDIZ
** Modified date:        2021-10-01
** Version:              v1.0
** Description:          TOPWAY HMI kütüphanesi
** Başlıca Komutlar：
       1. Okuma ve yazma komutları
			 begin()...........................Arduino ile Ekran arasındaki bağlantıyı kurmaya çalışır.				 
	         int say(uint32_t).................Ekrandan belirli adresteki int sayısını getirir.
			 long saylong(uint32_t)............Ekrandan belirli adresteki long sayısını getirir.
			 int64_t say64(uint32_t)...........Ekrandan belirli adresteki int64 sayısını getirir. 
			 String oku(uint32_t)..............Ekrandan belirli adresdeki String verisini okur.			 
			 bool yaz (uint32_t, int)..........Arduinodan belirli adrese int tipinde sayıyı yazar.
			 bool yaz (uint32_t, long).........Arduinodan belirli adrese long tipinde sayıyı yazar.
			 bool yaz (uint32_t, int64_t)......Arduinodan belirli adrese int64 tipinde sayıyı yazar.
			 bool yaz (uint32_t, String).......Arduinodan belirli adrese String tipinde sayıyı yazar.
       2. Sorgu komutları	
			 bool baglantiKur()................Arduino ile Ekran arasında bağlantı var mı, yok mu kontrol eder.
			 String versiyonOku()..............Ekran versiyonunu String tipinde getirir.	
			 void sayfaAc(byte)................Belirtilen sayfayı açar
			 byte sayfaNo()....................Mevcut sayfanın id sini getirir.
			 bool dokunmaVarsa()...............Tanımlanmış dokunma alanlarında Return değeri tanımlanmışsa dokunma esnasında kod yollar.
    
*********************************************************************************************************/



#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#ifndef FixajEkran_h
#define FixajEkran_h

#include "Arduino.h"

#define  CMD_HEAD             0xAA
#define  CMD_TAIL             0xCC, 0x33, 0xC3, 0x3C
#define  MAX_SIZE             64

class FixajEkran {
  public:
    FixajEkran(HardwareSerial& serial);
    void begin(unsigned long baud = 9600);
	
	String oku (uint32_t);
	int say(uint32_t);
	long saylong(uint32_t);
	int64_t say64(uint32_t);
	
	bool yaz (uint32_t, int);
	bool yaz (uint32_t, long);
	bool yaz (uint32_t, int64_t);
	bool yaz (uint32_t, String);

	bool baglantiKur();
	String versiyonOku();	
	void sayfaAc(byte);
	byte sayfaNo();
	bool dokunmaVarsa();

  private:
	void handshake();
	void versiyon();
	
	void sendHead();
	void sendKuyruk();
	bool yazN16(uint32_t, int);
	bool yazN32(uint32_t, long);
	bool yazN64(uint32_t, int64_t);
	void Send16(int);
	void Send32(long);
	void Send64(int64_t);
	void adres(String);
	HardwareSerial* _serial;
	unsigned long _tmr1;  
	unsigned long _tmr12; 
	void kolayadres(uint32_t);  
    byte receivedChars[MAX_SIZE]; 
	
    bool komut = false;
	bool Tara(byte, byte);
	bool Tara(byte);
	bool KuyrukKontrol();
	bool Bekleme(int);
	
	bool okur (uint32_t);	
	bool say16 (uint32_t);
	bool say32 (uint32_t);	
	bool sayi64 (uint32_t);
	void shortCut();
	
};

#endif
