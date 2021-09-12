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
** File Name:            Fixajhmi.cpp
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



#include "Arduino.h"
#include "FixajEkran.h"

const uint8_t head[1] = {CMD_HEAD};
const uint8_t kuyruk[4] =  {CMD_TAIL};
byte rc;

FixajEkran::FixajEkran(HardwareSerial& serial){  
_serial = &serial;
}

void FixajEkran::begin(unsigned long baud){
_serial->begin(baud);

delay(100);
 
_tmr1 = millis();
while(_serial->available() > 0){
if((millis() - _tmr1) > 100UL){    // Reading... Waiting... But not forever......
break;
}
_serial->read();                // Read and delete bytes
}

 

if(baglantiKur()){
	_serial->println(F("")); 
	_serial->println(F("Bağlantı Kuruldu. Başlıyoruz..")); 
}
else{

	_serial->println(F("")); 
	_serial->println(F("HATA-1 Bağlantı Başarısız. Kablo bağlantılarını kontrol edip, Sistemi Yeniden Başlatın")); 
	while(1){}
}

}

bool FixajEkran::baglantiKur(){
	handshake();
	if(Tara(0x30)){
		String str((char*) receivedChars);
		if(str.equals("Topway HMT Ready"))
		return true;
	}else{
		return false;
	}
	
}

String FixajEkran::versiyonOku(){
	versiyon();
	if(Tara(0x31)){
		String str((char*) receivedChars);		 
		return str;
	}else{
		return "";
	}
}

String FixajEkran::oku(uint32_t Addr){ 
	if(okur(Addr)){		
		if(Tara(0x43)){ 
		String str((char*) receivedChars);
		return str;
	}else{
		return "";
	}
	}else{
	return "";
	}
}

int FixajEkran::say(uint32_t Addr){
	int w=0;
	if(say16(Addr)){		
		if(Tara(2,0x3e)){
			for (byte i = 0; i < 2; i++) {
			w = w + ((int)(receivedChars[2 - (i + 1)]) << (i * 8));		
			} 
			return w;
		}else{
		return false;
		}
	}else{
	return false;
	}
}

long FixajEkran::saylong(uint32_t Addr){
	long w=0;
	if(say32(Addr)){		
		if(Tara(4,0x45)){
		for (byte i = 0; i < 4; i++) {
		w = w + ((long)(receivedChars[4 - (i + 1)]) << (i * 8));
		} 
		return w;
	}else{
		return false;
	}
	}else{
	return false;
	}
}

int64_t FixajEkran::say64(uint32_t Addr){
	int64_t w=0;
	if(sayi64(Addr)){		
		if(Tara(8,0x49)){
		for (byte i = 0; i < 8; i++) {
		w = w + ((int64_t)(receivedChars[8 - (i + 1)]) << (i * 8));
		} 
		return w;
	}else{
		return false;
	}
	}else{
	return false;
	}
}

bool FixajEkran::Tara(byte komut){
	
	byte ndx = 0;  			
	
	if(!Bekleme(801)) return false;
	rc = _serial->read();
	if(rc==0xAA){
		if(!Bekleme(802)) return false;
	    rc = _serial->read();
		if(rc==komut){
		 _tmr12 = millis();
			while(1){
			 
				if((millis() - _tmr12) > 900UL){    
					_serial->println("HATA-899 Okuma Zaman Aşımına Uğradı");
					return false;
				}
				
				if(!Bekleme(803)) return false;			
				rc=_serial->read();						
				if ( rc == 0xcc) {  
					if(!Bekleme(804)) return false;					
					rc=_serial->read();
					if (rc == 0x33) {  
					  if(!Bekleme(804)) return false;
					  rc=_serial->read();
					  if (rc == 0xc3) {  
							if(!Bekleme(805)) return false;
							rc=_serial->read();
							if (rc== 0x3c) {   
							ndx = 0;
							return true; 
							
							}else{
							receivedChars[ndx] = 0xcc;
							ndx++;
							receivedChars[ndx] = 0x33;
							ndx++;
							receivedChars[ndx] = 0xc3;
							ndx++;
							receivedChars[ndx] = rc;
							ndx++;
							}
						}else{
							receivedChars[ndx] = 0xcc;
							ndx++;
							receivedChars[ndx] = 0x33;
							ndx++;
							receivedChars[ndx] = rc;
							ndx++;
							}
					}else{
						receivedChars[ndx] = 0xcc;
						ndx++;
						receivedChars[ndx] = rc;
						ndx++;
						}
				}else{
					receivedChars[ndx] = rc;
					ndx++;
					
						if (ndx >= MAX_SIZE) {  
						ndx = MAX_SIZE - 1;
						}
					}				
				}				
						
		}//rc=komut
		else{			
			Bekleme(0);			
			_tmr1 = millis();		
			while(rc != 0x3C){            
				rc = _serial->read();                                    
					if((millis() - _tmr1) > 100UL){      
					return false;                               
					}		
			}
		
			if(rc==0x3C){
				if(_serial->available() > 6){    
				Tara(komut);	 
			}else{
				return false;
			}		
			}	
		}
	} //rc=AA
	else{
		
		Bekleme(0);			
		_tmr1 = millis();		
		while(rc != 0x3C){            
			rc = _serial->read();                                       
			if((millis() - _tmr1) > 100UL){      
			return false;                            
			}   
		}
		
		if(rc==0x3C){
			if(_serial->available() > 6){    
			Tara(komut);	
			
			}else{
				return false;
			}
		}	
		
	}
			
}

bool FixajEkran::Tara(byte karakter,byte komut){
	
	byte ndx = 0;  			
	
	if(!Bekleme(701)) return false;
	rc = _serial->read();
	if(rc==0xAA){
		if(!Bekleme(702)) return false;
	    rc = _serial->read();
		if(rc==komut){
			
			for(byte i=0; i<karakter; i++){
			
			if(!Bekleme(703)) return false;
	        rc = _serial->read();
			receivedChars[ndx]=rc;
			ndx++; 			
			} 
			
			if(KuyrukKontrol()){
				return true;
			}else{
				return false;
			}
			
			
		}//rc=komut
		else{
		Bekleme(0);	
		
		_tmr1 = millis();		
		while(rc != 0x3C){            
		rc = _serial->read();                                    
		if((millis() - _tmr1) > 100UL){      
        return -1;                               
		}   
		}
		
		if(rc==0x3C){
			if(_serial->available() > 6){    
			Tara(karakter,komut);	 
			}else{
				return -1;
			}		
		}	
		}
	} //rc=AA
	else{
		Bekleme(0);	
		
		_tmr1 = millis();		
		while(rc != 0x3C){            
		rc = _serial->read();                                       
		if((millis() - _tmr1) > 100UL){      
        return -1;                            
		}   
		}
		
		if(rc==0x3C){
			if(_serial->available() > 6){    
			Tara(karakter,komut);	
			
			}else{
				return -1;
			}
		}	
		
	}
	
}

bool FixajEkran::KuyrukKontrol(){
	if(!Bekleme(601)) return false;
	        rc = _serial->read();
			if(rc==0xCC){
				if(!Bekleme(601)) return false;
	            rc = _serial->read();
				if(rc==0x33){
				if(!Bekleme(601)) return false;
	            rc = _serial->read(); 
					if(rc==0xC3){
					if(!Bekleme(601)) return false;
					rc = _serial->read();
						if(rc==0x3C){ 
							return true;
						}else{
							shortCut();
						}
					}else{
						shortCut();
					}
				}else{
					shortCut();
					}			
			}else{
				shortCut();
            }
	return false;
	
}

bool FixajEkran::yaz(uint32_t Addr, String abc){

if(Addr > 0x0001FFFF)
{
_serial->println(F("HATA-100 adres hatalı"));
return false;
}

sendHead();
_serial->write(0x42);
kolayadres(Addr);
_serial->print(abc);
_serial->write(0x00);
sendKuyruk();

return true;
}

bool FixajEkran::yaz(uint32_t Addr, int xyz){

if((Addr >= 0x00080000) && (Addr <= 0x0009FFFF))
{
return yazN16(Addr,xyz);

}else if((Addr >= 0x00020000) && (Addr <= 0x0002FFFF))
{
return yazN32(Addr,xyz);

}else if((Addr >= 0x00030000) && (Addr <= 0x0003FFFF))
{
return yazN64(Addr,xyz);

}else{
_serial->println(F("HATA-116 adres hatalı"));
return false;
}

}

bool FixajEkran::yaz(uint32_t Addr, long xyz){


if((Addr >= 0x00080000) && (Addr <= 0x0009FFFF))
{
return yazN16(Addr,xyz);

}else if((Addr >= 0x00020000) && (Addr <= 0x0002FFFF))
{
return yazN32(Addr,xyz);

}else if((Addr >= 0x00030000) && (Addr <= 0x0003FFFF))
{
return yazN64(Addr,xyz);

}else{
_serial->println(F("HATA-132 adres hatalı"));
return false;
}


}

bool FixajEkran::yaz(uint32_t Addr, int64_t xyz){


if((Addr >= 0x00080000) && (Addr <= 0x0009FFFF))
{
return yazN16(Addr,xyz);

}else if((Addr >= 0x00020000) && (Addr <= 0x0002FFFF))
{
return yazN32(Addr,xyz);

}else if((Addr >= 0x00030000) && (Addr <= 0x0003FFFF))
{
return yazN64(Addr,xyz);

}else{
_serial->println(F("HATA-164 adres hatalı"));
return false;
}
}

bool FixajEkran::yazN16(uint32_t Addr, int xyz){


if( xyz > 32767 || xyz < -32767){
_serial->println(F("HATA-216 Sayı değeri sınırların dışında. Olması gereken ±32767"));
return false;
}

sendHead();
_serial->write(0x3D);
kolayadres(Addr);
Send16(xyz);
sendKuyruk();

return true;
}

bool FixajEkran::yazN32(uint32_t Addr, long xyz){

if( xyz > 2147483647 || xyz < -2147483647){
_serial->println(F("HATA-232 Sayı değeri sınırların dışında. Olması gereken ±2147483647 "));
return false;
}

sendHead();
_serial->write(0x44);
kolayadres(Addr);
Send32(xyz);
sendKuyruk();

return true;
}

bool FixajEkran::yazN64(uint32_t Addr, int64_t xyz){

if( xyz > 9223372036854775806 || xyz < -9223372036854775807){
_serial->println(F("HATA-264 Sayı değeri sınırların dışında. Olması gereken ±9223372036854775807"));
return false;
}

sendHead();
_serial->write(0x48);
kolayadres(Addr);
Send64(xyz);
sendKuyruk();

return true;


}

void FixajEkran::Send16(int myNum) {

byte buffer[2];
buffer[1] = (byte)myNum;
buffer[0] = (byte)( myNum >> 8 );
_serial->write(buffer, sizeof buffer);

}

void FixajEkran::Send32(long myNum) {
byte buffer[4];
buffer[3] = (byte)myNum;
buffer[2] = (byte)( myNum >> 8 );
buffer[1] = (byte)( myNum >> 16 );
buffer[0] = (byte)( myNum >> 24 );
_serial->write(buffer, sizeof buffer);
}

void FixajEkran::Send64(int64_t myNum) {
byte buffer[8];
buffer[7] = (byte)myNum;
buffer[6] = (byte)( myNum >> 8 );
buffer[5] = (byte)( myNum >> 16 );
buffer[4] = (byte)( myNum >> 24 );
buffer[3] = (byte)( myNum >> 32 );
buffer[2] = (byte)( myNum >> 40 );
buffer[1] = (byte)( myNum >> 48 );
buffer[0] = (byte)( myNum >> 56 );
_serial->write(buffer, sizeof buffer);
}

bool FixajEkran::okur(uint32_t Addr){

  if(Addr > 0x0001FFFF)
    { 
		_serial->println("HATA-2AA Yanlış adres");
        return false;
    }
	
	Bekleme(0);
		while (_serial->available() > 0) { 
        _serial->read(); 	
		}
	
	
	sendHead();
	_serial->write(0x43);
	kolayadres(Addr); 
	sendKuyruk();	
	
	return true;


}

bool FixajEkran::say16(uint32_t Addr){
	
	 if((Addr < 0x00080000) || (Addr > 0x0009FFFF))
    {
		_serial->println("HATA-216 Yanlış adres");
        return false;
    } 
	
	Bekleme(0);			
	while (_serial->available() > 0) { 
        _serial->read(); 	
	}
	
	sendHead();
	_serial->write(0x3E);
	kolayadres(Addr); 
	sendKuyruk();	 
	
	return true;

}

bool FixajEkran::say32(uint32_t Addr){
	
	 if((Addr < 0x00020000) || (Addr > 0x0002FFFF))
    {
		_serial->println("HATA-232 Yanlış adres");
        return false;
    }
	Bekleme(0);	
	while (_serial->available() > 0) { 
        _serial->read(); 	
	}
	
	sendHead();
	_serial->write(0x45);
	kolayadres(Addr); 
	sendKuyruk();
	
	return true;

}

bool FixajEkran::sayi64(uint32_t Addr){
	
	 if((Addr < 0x00030000 || Addr > 0x0003FFFF))
    {
		_serial->println("HATA-264 Yanlış adres");
        return false;
    }
	Bekleme(0);	
	while (_serial->available() > 0) { 
        _serial->read(); 	
	}
	
	sendHead();
	_serial->write(0x49);
	kolayadres(Addr); 
	sendKuyruk();	
	
	return true;

}

void FixajEkran::sayfaAc(byte id){ 

    sendHead();
	_serial->write(0x70);
	_serial->write(0x00);
	
	if(id<99){
	_serial->write(id);
	}else{
		return;
	}
	
	sendKuyruk();	


}

byte FixajEkran::sayfaNo(){ 
byte w=0;
    sendHead();
	_serial->write(0x32);	
	sendKuyruk();	
	
	
	if(Tara(2,0x32)){
			for (byte i = 0; i < 2; i++) {
			w = w + ((int)(receivedChars[2 - (i + 1)]) << (i * 8));		
			} 
			return w;
		}else{
		return 0;
		}
}

bool FixajEkran::dokunmaVarsa(){
	if(!Bekleme(0)) return false;
	rc = _serial->read();
	if(rc==0xAA){ 
		if(!Bekleme(0)) return false;		
	    rc = _serial->read();
		if(rc==0x78 || rc==0x79){
		Bekleme(0);
		shortCut();		
		return true;
			}
		else{
		return false;
		}
	}
	else{
		return false;
	}
		
	
}

void FixajEkran::sendHead(){
_serial->write(head, 1);
}

void FixajEkran::sendKuyruk(){
_serial->write(kuyruk, 4);
}

bool FixajEkran::Bekleme(int hatakodu){
	_tmr1 = millis();
	while (!_serial->available()) {
		if((millis() - _tmr1) > 100UL){    // Reading... Waiting... But not forever......
			if(hatakodu!=0){
			_serial->println("HATA-");
			_serial->println(hatakodu);
			_serial->println(" Veri okuma hatası"); 
			}
			return false;
			}
	}return true;  
}

void FixajEkran::shortCut(){
		_tmr1 = millis();		
		while(rc != 0x3C){            
		rc = _serial->read();        // whille the start_char is not the start command symbol                                 
		if((millis() - _tmr1) > 100UL){     //   Waiting... But not forever...... 
        break;                            
		}   
		}
}

void FixajEkran::kolayadres(uint32_t myNum){
byte buffer[4];
buffer[3] = (byte)myNum;
buffer[2] = (byte)( myNum >> 8 );
buffer[1] = (byte)( myNum >> 16 );
buffer[0] = (byte)( myNum >> 24 );
_serial->write(buffer, sizeof buffer);

}

void FixajEkran::handshake(){
sendHead();
_serial->write(0x30);
sendKuyruk();
}

void FixajEkran::versiyon(){
sendHead();
_serial->write(0x31);
sendKuyruk();
}
