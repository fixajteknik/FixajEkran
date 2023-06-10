/*! 										Copyright (c) 
 *                              		Fixaj  Mehmet YILDIZ
 * 
 *                              		https://fixaj.com/  
 * 
 *  Tüm hizmetler Mehmet Yıldız şirketi Fixaj tarafından sağlanmaktadır.
 *  FİXAJ MAĞAZALARIMIZ DIŞINDA SATIN ALINAN TOPWAY MARKA DOKUNMATİK EKRANLARDA BU KÜTÜPHANENİN TİCARİ OLARAK KULLANIMI YASAKTIR.
 *  Herhangi bir program, belge, test sonucu, çözüm, destek ve diğer malzeme ve bilgiler yalnızca referans amaçlıdır
 *  ve müşterilerin kullanmama veya atıfta bulunma ve kendileri tarafından değişiklik yapma hakları vardır.
 *  Müşterinin kullanımı sırasında herhangi bir özel, tesadüfi veya dolaylı kayıp meydana gelirse, 
 *  herhangi bir bütünlük, güvenilirlik ve diğer garantileri sağlamak için şirket sorumlu olmayacaktır.
 *  HERHANGİ BİR SORUMLULUĞUN KABUL EDİLMEMESİ.
 *                                                                         Fixaj
 * 
 * 
 * --------------File Info-------------------------------------------------------------------------------
 *  Modified by:            	MEHMET YILDIZ
 *  Modified date:        	8.06.2023
 *  Version:                	v0.1
 *  Description:    
 * 
 */

//#include "esp32-hal.h"
//#include "soc/soc_memory_layout.h"
//#include "Arduino.h"
#include "HardwareSerial.h"
//#include "sys/_types.h"
#include "FixajEkran.h"

INT8U const __GucFrameHead[1] = { CMD_HEAD };               // 帧头 1字节
INT8U const __GucFrameTail[4] = { CMD_TAIL };               // 帧尾 4字节
INT8U const __GucFrameSpecialFuc[4] = { CMD_SPECIALFUNC };  // 特殊功能命令 4字节+
byte __GucFrameSorgu3[3];


#ifdef ACTIVATE_SOFTWARE_SERIAL
FixajEkran::FixajEkran(byte txE32pin, byte rxE32pin, UART_BPS_RATE bpsRate) {
  this->txE32pin = txE32pin;
  this->rxE32pin = rxE32pin;
  SoftwareSerial *mySerial = new SoftwareSerial((uint8_t)this->txE32pin, (uint8_t)this->rxE32pin);  // "RX TX" // @suppress("Abstract class cannot be instantiated")
  this->ss = mySerial;
  this->hs = NULL;

  this->bpsRate = bpsRate;
}

#endif

FixajEkran::FixajEkran(HardwareSerial *serial, UART_BPS_RATE bpsRate) {  //, uint32_t serialConfig
  this->txE32pin = txE32pin;
  this->rxE32pin = rxE32pin;

#ifdef ACTIVATE_SOFTWARE_SERIAL
  this->ss = NULL;
#endif

  this->hs = serial;

  //    this->serialConfig = serialConfig;

  this->bpsRate = bpsRate;
}

#ifdef HARDWARE_SERIAL_SELECTABLE_PIN
FixajEkran::FixajEkran(byte txE32pin, byte rxE32pin, HardwareSerial *serial, UART_BPS_RATE bpsRate, uint32_t serialConfig) {
  this->txE32pin = txE32pin;
  this->rxE32pin = rxE32pin;

#ifdef ACTIVATE_SOFTWARE_SERIAL
  this->ss = NULL;
#endif

  this->serialConfig = serialConfig;

  this->hs = serial;

  this->bpsRate = bpsRate;
}

#endif

#ifdef ACTIVATE_SOFTWARE_SERIAL

FixajEkran::FixajEkran(SoftwareSerial *serial, UART_BPS_RATE bpsRate) {
  this->txE32pin = txE32pin;
  this->rxE32pin = rxE32pin;

  this->ss = serial;
  this->hs = NULL;

  this->bpsRate = bpsRate;
}


#endif

bool FixajEkran::begin() {
  delay(2000);  //elektrik kesintilerinde sonra  ESP veya NANO nun
                // ekrandan önce start almasını önlemek için bekletiliyor.
  DEBUG_PRINTLN("Begin");
  if (this->hs) {
    DEBUG_PRINTLN("Begin Hardware Serial");

#ifdef HARDWARE_SERIAL_SELECTABLE_PIN
    if (this->txE32pin != -1 && this->rxE32pin != -1) {
      this->serialDef.begin(*this->hs, this->bpsRate, this->serialConfig, this->txE32pin, this->rxE32pin);
    } else {
      this->serialDef.begin(*this->hs, this->bpsRate, this->serialConfig);
    }
#endif
#ifndef HARDWARE_SERIAL_SELECTABLE_PIN
    this->serialDef.begin(*this->hs, this->bpsRate);
#endif
    while (!this->hs) {
      ;  // wait for serial port to connect. Needed for native USB
    }

#ifdef ACTIVATE_SOFTWARE_SERIAL
  } else if (this->ss) {
    DEBUG_PRINTLN("Begin Software Serial");

    this->serialDef.begin(*this->ss, this->bpsRate);
  } else {
    DEBUG_PRINTLN("Begin Software Serial Pin");
    SoftwareSerial *mySerial = new SoftwareSerial((int)this->txE32pin, (int)this->rxE32pin);  // "RX TX" // @suppress("Abstract class cannot be instantiated")
    this->ss = mySerial;

    //		SoftwareSerial mySerial(this->txE32pin, this->rxE32pin);
    DEBUG_PRINT("RX Pin: ");
    DEBUG_PRINT((int)this->txE32pin);
    DEBUG_PRINT("TX Pin: ");
    DEBUG_PRINTLN((int)this->rxE32pin);

    this->serialDef.begin(*this->ss, this->bpsRate);
#endif
  }

  this->serialDef.stream->setTimeout(1000);
  return true;  //ekleme
}

int FixajEkran::available() {
  return this->serialDef.stream->available();
}

void FixajEkran::cleanUARTBuffer() {
  //  bool IsNull = true;
  while (this->available()) {
    this->serialDef.stream->read();
  }
}

//eklediğim kodlar fixaj.com

//public:
void FixajEkran::UART_SendChar(uint8_t ucDat) {
  this->serialDef.stream->write(ucDat);  // Seri porttan veri gönderme
#ifndef ACTIVATE_SOFTWARE_SERIAL
  while (!this->serialDef.stream->availableForWrite())
    ;  // Verinin gönderildiğinden emin olmak için bekleyin, software serial da çalışmıyor
#endif
}

bool FixajEkran::yaz(INT32U Addr, String abc) {
  return HMT_WirteVPString(Addr, (INT8U *)abc.c_str());
}

bool FixajEkran::yaz(INT32U Addr, INT8U *Data) {
  return HMT_WirteVPString(Addr, Data);
}

bool FixajEkran::yaz(INT32U Addr, char *Data) {
  INT8U *convertedData = (INT8U *)Data;
  return HMT_WirteVPString(Addr, convertedData);
}

bool FixajEkran::yaz(INT32U Addr, const char *abc) {
  const INT8U *Data = (const INT8U *)abc;
  return HMT_WirteVPString(Addr, (INT8U *)Data);
}

bool FixajEkran::yaz(INT32U Addr, int xyz) {
  return HMT_WriteVPN16(Addr, xyz);
}

bool FixajEkran::yaz(INT32U Addr, unsigned int xyz) {
  return HMT_WriteVPN16(Addr, xyz);
}

bool FixajEkran::yaz(INT32U Addr, long xyz) {
  return HMT_WriteVPN32(Addr, xyz);
}

bool FixajEkran::yaz(INT32U Addr, uint64_t xyz) {
  uint32_t dataH = static_cast<uint32_t>(xyz >> 32);  // Yüksek 32-bit değeri
  uint32_t dataL = static_cast<uint32_t>(xyz);        // Düşük 32-bit değeri
  return HMT_WriteVPN64(Addr, dataH, dataL);
}

bool FixajEkran::sayfayaGit(INT16U usPageID) {
  return HMT_DispPage(usPageID);
}

bool FixajEkran::EkranTest() {
  cleanUARTBuffer();
  HMT_HandShake();
  int a = 0;
  return read_and_process_data(&a);
}


bool FixajEkran::dokunmaDinle() {
  if (available()) {
    delay(10);  //tamponun tam dolması için
    if (sorgu(__GucFrameSorgu3)) {
      return true;
    }
  }
  return false;
}

bool FixajEkran::butonBasildiMi(const Buton &btn) {
  return (btn.data[0] == __GucFrameSorgu3[0]) && (btn.data[1] == __GucFrameSorgu3[1]) && (btn.data[2] == __GucFrameSorgu3[2]);
}


//bu sorgu fonksiyonu sadece LCD ekrandan gelen verileri okumak için kullanılır ve
// sadece 0x78 touche release ve 0x79 touch down metodlarını tespit etmek için çalışır
// touch down ve touch release masaüstü uygulama SGTOOLS üzerinden dokunma özelliğinin RETURN sekmesinden atanır.
// sadece touch down veya release özelliklerinden birini seçin.
// Mantık şu: bunu basıldı mı (evet), butondan basılıp çekildi mi (evet).

//private:
template<size_t N>
bool FixajEkran::sorgu(byte (&array)[N]) {

  byte command, data_length;

  if (this->serialDef.stream->read() == __GucFrameHead[0]) {
    // Komut byte'ını oku
    command = this->serialDef.stream->read();

    // Komuta göre veri uzunluğunu belirle
    switch (command) {
      case 0x78:
        data_length = 3;
        break;
      case 0x79:
        data_length = 3;
        break;
      case 0x77:  //tuş takımından gelen anlamsız 0x77 data entered sorgularını göz ardı et.
        cleanUARTBuffer();
        return false;
        break;
      default:
        cleanUARTBuffer();
        return false;
        break;
    }

    // Veriyi oku
    byte data[data_length];
    for (int i = 0; i < data_length; i++) {
      data[i] = this->serialDef.stream->read();
    }

    // Bitiş byte'larını kontrol et
    bool valid_end = true;
    for (int i = 0; i < 4; i++) {
      if (this->serialDef.stream->read() != __GucFrameTail[i]) {
        valid_end = false;
        break;
      }
    }

    // Eğer bitiş byte'ları doğru ise, komutu işle ve sonucu al
    if (valid_end) {
      if (command == 0x78 || command == 0x79) {
        /*for (int i = 0; i < data_length; i++) {
          array[i] = data[i];
        }
        */
        memcpy(array, data, sizeof(data));
        return true;
      }
    }
  }

  cleanUARTBuffer();  //gelen veride hata varsa komple gelen verileri boşalt
                      // çok emin değilim zamanla bu satırı yoruma çekebilirim
  return false;
}

template<typename T>
bool FixajEkran::oku(INT32U Addr, T *result) {
  if ((Addr >= 0x00080000) && (Addr <= 0x0009FFFF)) {
    HMT_SendCMD_ReadVPN16(Addr);
  } else if ((Addr >= 0x00020000) && (Addr <= 0x0002FFFF)) {
    HMT_SendCMD_ReadVPN32(Addr);
  } else if ((Addr >= 0x00030000) && (Addr <= 0x0003FFFF)) {
    HMT_SendCMD_ReadVPN64(Addr);
  } else {
    return false;
  }
  return read_and_process_data(result);
}

template bool FixajEkran::oku<int>(INT32U Addr, int *result);
template bool FixajEkran::oku<long>(INT32U Addr, long *result);
//template bool FixajEkran::oku<double>(INT32U Addr, double *result);

template<typename T>
bool FixajEkran::read_and_process_data(T *result) {
  // Fonksiyonun geri kalan kısmı aynı kalır

  delay(10);  //kritik bekleme süresi, aksi taktirde sorgu sonrası bekleme olmayınca
              // gelen veri kaybolabilir.

  byte command, data_length;
  // Başlangıç byte'ını kontrol et
  if (this->serialDef.stream->read() == __GucFrameHead[0]) {
    // Komut byte'ını oku
    command = this->serialDef.stream->read();

    // Komuta göre veri uzunluğunu belirle
    switch (command) {
      case 0x3E:
        data_length = 2;
        break;
      case 0x45:
        data_length = 4;
        break;
      case 0x49:
        data_length = 8;
        break;
      case 0x30:
        data_length = 17;
        break;
      default:
        cleanUARTBuffer();
        return false;
        break;
    }

    // Veriyi oku
    byte data[data_length];
    for (int i = 0; i < data_length; i++) {
      data[i] = this->serialDef.stream->read();
    }

    // Bitiş byte'larını kontrol et
    bool valid_end = true;
    for (int i = 0; i < 4; i++) {
      if (this->serialDef.stream->read() != __GucFrameTail[i]) {
        valid_end = false;
        break;
      }
    }

    // Eğer bitiş byte'ları doğru ise, komutu işle ve sonucu al
    if (valid_end) {
      if (command == 0x3E || command == 0x45 || command == 0x49) {
        process_command(command, data, data_length, result);
        return true;
      } else if (command == 0x30) {
       String str((char *)data);
        if (str.equals("Topway HMT Ready")) {
            Serial.println(str);
          return true;
        } 
      }
    }
  }
  //Serial.println("HATA-bağlantı YOK");
  cleanUARTBuffer();
  return false;
}


template bool FixajEkran::read_and_process_data<int>(int *result);
template bool FixajEkran::read_and_process_data<long>(long *result);
//template bool FixajEkran::read_and_process_data<double>(double *result);


template<typename T>
void FixajEkran::process_command(byte command, byte *data, int data_length, T *result) {

  switch (command) {
    case 0x3E:
      *result = byteArrayToDecimal<int>(data, data_length);
      break;
    case 0x45:
      *result = byteArrayToDecimal<int>(data, data_length);
      break;
    case 0x49:
      *result = byteArrayToDecimal<long>(data, data_length);
      break;
    default:
      // Hatalı komut
      break;
  }
}

// Şablon örneklemelerini burada belirtin
template void FixajEkran::process_command<int>(byte command, byte *data, int data_length, int *result);
template void FixajEkran::process_command<long>(byte command, byte *data, int data_length, long *result);

// burada gelen butün sayıları işleyebilecek ve gelen veri tipine göre dnüş yapacak fonksiyonu yazdık
template<typename T>
T FixajEkran::byteArrayToDecimal(byte *data, int data_length) {
  T result = 0;
  for (int i = 0; i < data_length; i++) {
    result = (result << 8) | data[i];
  }
  return result;
}


// Explicit template instantiation declarations
template int FixajEkran::byteArrayToDecimal<int>(byte *data, int data_length);
template long FixajEkran::byteArrayToDecimal<long>(byte *data, int data_length);
//template double FixajEkran::byteArrayToDecimal<double>(byte *data, int data_length);

bool FixajEkran::okuStr(INT32U Addr, String *deger) {
  HMT_SendCMD_ReadVPString(Addr);
  return sorguSTR(deger);
}

bool FixajEkran::okuStr(INT32U Addr, char *deger) {
  HMT_SendCMD_ReadVPString(Addr);
  return sorguSTR(deger);
}


bool FixajEkran::sorguSTR(String *deger) {
  byte command, data_length;
  delay(10);
  if (this->serialDef.stream->read() == __GucFrameHead[0]) {
    // Komut byte'ını oku
    command = this->serialDef.stream->read();

    // Komuta göre veri uzunluğunu belirle
    switch (command) {
      case 0x43:
        data_length = 128;
        break;
      default:
        cleanUARTBuffer();
        return false;
        break;
    }

    // Veriyi oku
    char data[data_length];
    int rlen = 0;
    rlen = this->serialDef.stream->readBytesUntil('\0', data, data_length - 1);
    data[rlen] = '\0';

    // Bitiş byte'larını kontrol et
    bool valid_end = true;
    for (int i = 0; i < 4; i++) {
      if (this->serialDef.stream->read() != __GucFrameTail[i]) {
        valid_end = false;
        break;
      }
    }

    // Eğer bitiş byte'ları doğru ise, komutu işle ve sonucu al
    if (valid_end) {
      if (command == 0x43) {
        // *deger = data;
        //*deger = String(reinterpret_cast<const char *>(data), rlen);
        char dataStr[rlen + 1];
        strncpy(dataStr, data, rlen);
        dataStr[rlen] = '\0';
        *deger = String(dataStr);
      }
      return true;
    }
  }
  cleanUARTBuffer();
  return false;
}

bool FixajEkran::sorguSTR(char *deger) {
  byte command, data_length;
  delay(10);
  if (this->serialDef.stream->read() == __GucFrameHead[0]) {
    // Komut byte'ını oku
    command = this->serialDef.stream->read();

    // Komuta göre veri uzunluğunu belirle
    switch (command) {
      case 0x43:
        data_length = 128;
        break;
      default:
        cleanUARTBuffer();
        return false;
        break;
    }

    // Veriyi oku
    char data[data_length];
    int rlen = 0;
    rlen = this->serialDef.stream->readBytesUntil('\0', data, data_length - 1);
    data[rlen] = '\0';

    // Bitiş byte'larını kontrol et
    bool valid_end = true;
    for (int i = 0; i < 4; i++) {
      if (this->serialDef.stream->read() != __GucFrameTail[i]) {
        valid_end = false;
        break;
      }
    }

    // Eğer bitiş byte'ları doğru ise, komutu işle ve sonucu al
    if (valid_end) {
      if (command == 0x43) {
        strncpy(deger, data, rlen);
        deger[rlen] = '\0';
      }
      return true;
    }
  }
  cleanUARTBuffer();
  return false;
}

void FixajEkran::EkranParlaklikSeviyesi(INT8U ucPWMLevel) {
  HMT_SetBacklightLevel(ucPWMLevel);
}

void FixajEkran::BuzzerSesSeviyesi(INT8U ucLevel) {
  HMT_SetBeepLevel(ucLevel);
}

BOOLEAN FixajEkran::SaatTarihAyarla(INT8U Year, INT8U Month, INT8U Day, INT8U Hour, INT8U Minute, INT8U Second) {
  return HMT_SetRTC(Year, Month, Day, Hour, Minute, Second);
}

BOOLEAN FixajEkran::OnplanRengiDegis(INT8U elementType, INT16U pageID, INT8U elementID, INT16U foreGroundcolor) {
  return HMT_SetEleForeground(elementType, pageID, elementID, foreGroundcolor);
}

BOOLEAN FixajEkran::ArkaPlanRengiDegis(INT8U elementType, INT16U pageID, INT8U elementID, INT8U transparentMode, INT16U backGroundcolor) {
  return HMT_SetEleBackground(elementType, pageID, elementID, transparentMode, backGroundcolor);
}

//kütüphanenin önerdiği şekilde grafik kullanımı

BOOLEAN FixajEkran::grafikCiz(INT32U Addr, INT16U Length, INT16U *Data) {
  return HMT_WriteG16ALL(Addr, Length, Data);
}

//benim eklediğim NEGATİF sayılarıda gösterebilen fonksiyon
//test aşamasındadır.

BOOLEAN FixajEkran::grafikCiz(INT32U Addr, int Length, int *Data) {
  return HMT_WriteG16ALL2(Addr, Length, Data);
}

//Firmanın gönderdiği kodlar, kodun çinçe açıklamaları üstte yazıyor

/*********************************************************************************************************
** 函数名称:  __SendByte
** 函数功能:  单字节发送函数
** 输入参数:  ucData：        数据
** 返 回 值:  无
*********************************************************************************************************/
void FixajEkran::__SendByte(INT8U ucData) {
  UART_SendChar(ucData);  // 根据不同的芯片配置
}


/*********************************************************************************************************
** 函数名称:  __SendDByte
** 函数功能:  双字节发送函数
** 输入参数:  usData：  数据
** 返 回 值:  无
*********************************************************************************************************/
void FixajEkran::__SendDByte(INT16U usData) {
  __SendByte((usData >> 8) & 0xFF);  // 发送高8位数据
  __SendByte(usData & 0xFF);         // 发送低8位数据
}


/*********************************************************************************************************
** 函数名称:  __SendByteStr
** 函数功能:  单字节字符串发送函数
** 输入参数:  *pucStr:        要发送的命令字符串指针
**        ulNum:    要发送的数据个数
** 返 回 值:  无
*********************************************************************************************************/
void FixajEkran::__SendByteStr(INT8U const *pucStr, INT32U ulNum) {
  INT32U i;

  for (i = 0; i < ulNum; i++)  // 发送指定字节数据
  {
    __SendByte(pucStr[i]);
  }
}


/*********************************************************************************************************
** 函数名称:  __SendHead
** 函数功能:  发送命令帧头
** 输入参数:  无
** 输出参数:  无
** 返 回 值:  无
*********************************************************************************************************/
void FixajEkran::__SendHead(void) {
  __SendByteStr(__GucFrameHead, 1);
}


/*********************************************************************************************************
** 函数名称:  __SendTail
** 函数功能:  发送命令帧尾
** 输入参数:  无
** 输出参数:  无
** 返 回 值:  无
*********************************************************************************************************/
void FixajEkran::__SendTail(void) {
  __SendByteStr(__GucFrameTail, 4);
}

/*********************************************************************************************************
** 函数名称:  __SendSpecialFuncCode
** 函数功能:  发送特殊功能指令
** 输入参数:  无
** 输出参数:  无
** 返 回 值:  无
*********************************************************************************************************/
void FixajEkran::__SendSpecialFuncCode(void) {
  __SendByteStr(__GucFrameSpecialFuc, QUANTITYOF_ARRAY_MEMBER(__GucFrameSpecialFuc));  // 发送特殊功能指令
}
/****************************** 串口显示终端应用层函数 **************************************************/

/****************************************************************************
* 名    称： HMT_HandShake() 
* 功    能： 发送握手指令  0x30
* 入口参数： 无       
* 出口参数： 无
****************************************************************************/
void FixajEkran::HMT_HandShake(void) {
  __SendHead();      // 发送帧头
  __SendByte(0x30);  // 发送命令
  __SendTail();      // 发送帧尾
}


/****************************************************************************
* 名    称： HMT_SendCMD_ReadVersion 
* 功    能： 发送 读取版本信息的指令  0x31
* 入口参数： 无       
* 出口参数： 无
****************************************************************************/
void FixajEkran::HMT_SendCMD_ReadVersion(void) {
  __SendHead();      // 发送帧头
  __SendByte(0x31);  // 发送命令
  __SendTail();      // 发送帧尾
}


/****************************************************************************
* 名    称： HMT_SendCMD_ReadPageID 
* 功    能： 发送 读页面ID号的指令0x32
* 入口参数： 无       
* 出口参数： 无
****************************************************************************/
void FixajEkran::HMT_SendCMD_ReadPageID(void) {
  __SendHead();      // 发送帧头
  __SendByte(0x32);  // 发送命令
  __SendTail();      // 发送帧尾
}

/*********************************************************************************************************
** 函数名称:  HMT_SystemConfig
** 函数功能:  配置波特率以及触摸返回动作0xe0
** 输入参数:    usBaudrate:串口波特率：0x00~0x07对应波特率从1200-115200bps
** usTouchAction:触摸返回动作
** 输出参数:  无
** 返 回 值:  正确返回TRUE，否则返回FALSE
*********************************************************************************************************/
BOOLEAN FixajEkran::HMT_SystemConfig(INT8U usBaudrate, INT8U usTouchAction) {
  //  参数检查
  if (usBaudrate > SET_BAUD115200) {
    return (FALSE);
  }

  __SendHead();               // 发送帧头
  __SendByte(0xE0);           // 发送命令
  __SendSpecialFuncCode();    // 发送特殊功能指令
  __SendByte(usBaudrate);     // 发送波特率
  __SendByte(usTouchAction);  // 发送触摸模式
  __SendByte(0x00);           // 发送预留功能
  __SendTail();               // 发送帧尾

  return (TRUE);
}

/*********************************************************************************************************
** 函数名称:  HMT_SelectProject
** 函数功能:  多工程时，用于切换工程0xe1
** 输入参数:    projectID:工程ID    
** 输出参数:  无
** 返 回 值:  正确返回TRUE，否则返回FALSE
*********************************************************************************************************/
BOOLEAN FixajEkran::HMT_SelectProject(INT8U projectID) {
  //  参数检查
  if (projectID > PROJECT_MAX) {
    return (FALSE);
  }

  __SendHead();           // 发送帧头
  __SendByte(0xE1);       // 发送命令
  __SendByte(projectID);  // 发送工程ID
  __SendTail();           // 发送帧尾

  return (TRUE);
}


/*********************************************************************************************************
** 函数名称:  HMT_TouchCalibration
** 函数功能:  触摸屏校准(注意只有第一次使用触摸屏时需要校准) 0xe4
** 输入参数:  无
** 输出参数:  无
** 返 回 值:  无
*********************************************************************************************************/
void FixajEkran::HMT_TouchCalibration(void) {
  __SendHead();             // 发送帧头
  __SendByte(0xE4);         // 发送命令
  __SendSpecialFuncCode();  // 发送特殊功能指令
  __SendTail();             // 发送帧尾
}

/*********************************************************************************************************
** 函数名称:  HMT_SetScreenSaveTime
** 函数功能:  屏幕睡眠时间设置  0x5E
**        ucTime     睡眠时间（单位s）
** 输入参数:  ucPWMLevel：    暗屏等级0-0x3F
** 输出参数:  无
** 返 回 值:  正确返回TRUE，否则返回FALSE
*********************************************************************************************************/
void FixajEkran::HMT_SetScreenSaveTime(INT16U ucTime, INT8U ucPWMLevel) {
  //  参数检查
  if (ucPWMLevel > LIGHT_PWMLEVEL_MAX) {
    ucPWMLevel = LIGHT_PWMLEVEL_MAX;
  }

  __SendHead();  // 发送帧头
  __SendByte(0x5E);
  __SendDByte(ucTime);
  __SendByte(ucPWMLevel);
  __SendTail();  // 发送帧尾
}


/*********************************************************************************************************
** 函数名称:  HMT_SetBacklightLevel
** 函数功能:  背光亮度等级  0x5F
** 输入参数:  ucPWMLevel：    0-0x3F
** 输出参数:  无
** 返 回 值:  正确返回TRUE，否则返回FALSE
*********************************************************************************************************/
void FixajEkran::HMT_SetBacklightLevel(INT8U ucPWMLevel) {
  //  参数检查
  if (ucPWMLevel > LIGHT_PWMLEVEL_MAX) {
    ucPWMLevel = LIGHT_PWMLEVEL_MAX;
  }

  __SendHead();  // 发送帧头
  __SendByte(0x5F);
  __SendByte(ucPWMLevel);
  __SendTail();  // 发送帧尾
}


/*********************************************************************************************************
** 函数名称:  HMT_SetBeepLevel
** 函数功能:  蜂鸣器操作 0x79
** 输入参数:  ucLevel:        蜂鸣器时间等级0~0x3f 
** 输出参数:  无
** 返 回 值:  正确返回TRUE，否则返回FALSE
*********************************************************************************************************/
void FixajEkran::HMT_SetBeepLevel(INT8U ucLevel) {
  // 参数检查
  if (ucLevel > BEEP_LEVEL_MAX) {
    ucLevel = BEEP_LEVEL_MAX;
  }

  __SendHead();  // 发送帧头
  __SendByte(0x79);
  __SendByte(ucLevel);
  __SendTail();  // 发送帧尾
}

/*********************************************************************************************************
** 函数名称:  HMT_Buzzer_Ctrl 
** 函数功能:  蜂鸣器控制0x7A
** 输入参数:  
** LoopCount:        鸣叫循环次数
** PlayTime1:        第一次鸣叫时长  单位100ms    0x00~0xFF
** PlayTime2:        第二次鸣叫时长  单位100ms    0x00~0xFF
** PlayFre1:        第一次鸣叫频率    单位100Hz   0x05~0x32
** PlayFre2:        第二次鸣叫频率    单位100Hz   0x05~0x32
** 输出参数:  无
** 返 回 值:  正确返回TRUE，否则返回FALSE
*********************************************************************************************************/
BOOLEAN FixajEkran::HMT_BuzzerCtrl(INT8U loopCount, INT8U playTime1, INT8U playTime2, INT8U playFre1, INT8U playFre2) {
  // 参数检查
  if ((playFre1 > BUZZERFRE_MAX) || (playFre2 > BUZZERFRE_MAX)) {
    return (FALSE);
  }

  __SendHead();           // 发送帧头
  __SendByte(0x7A);       // 发送命令
  __SendByte(loopCount);  // 发送鸣叫循环次数
  __SendByte(playTime1);  // 发送第一次鸣叫时长
  __SendByte(playTime2);  // 发送第二次鸣叫时长
  __SendByte(playFre1);   // 发送第一次鸣叫频率
  __SendByte(playFre2);   // 发送第二次鸣叫频率

  __SendTail();  // 发送帧尾

  return (TRUE);
}

/*********************************************************************************************************
** 函数名称:  HMT_WriteFlash
** 函数功能:  写Flash 数据       0x90 地址范围：0x0000 0000 ~0x0003 FFFF 
** 输入参数:  
        Addr1 Flash地址
        Length 字长  0x0000 ~0x0400
        Data    写入数据
** 输出参数:  无
** 返 回 值:  正确返回TRUE，否则返回FALSE
*********************************************************************************************************/
BOOLEAN FixajEkran::HMT_WriteFlash(INT32U Addr, INT16U Length, INT8U *Data) {
  INT16U i;
  INT16U AddrH = 0, AddrL = 0;

  if ((Addr > 0x0003FFFF) || (Length > 0x0400)) {
    return (FALSE);
  }

  AddrH = Addr >> 16;  // 取高16数据
  AddrL = Addr;        // 取低16数据

  __SendHead();      // 发送帧头
  __SendByte(0x90);  // 发送命令
  __SendDByte(AddrH);
  __SendDByte(AddrL);
  __SendDByte(Length);

  for (i = 0; i < Length; i++) {
    __SendByte(Data[i]);
  }

  __SendTail();  // 发送帧尾

  return (TRUE);
}


/*********************************************************************************************************
** 函数名称:  HMT_SendCMD_ReadFlash
** 函数功能:  发送   读Flash 数据的指令   0x91    地址范围：0x0000 0000 ~0x0003 FFFF 
** 输入参数:  
        Addr    读Flash地址
        Length 字长  x0001 ~0x0400
        Data    读出数据
** 输出参数:  无
** 返 回 值:  正确返回TRUE，否则返回FALSE
*********************************************************************************************************/
BOOLEAN FixajEkran::HMT_SendCMD_ReadFlash(INT32U Addr, INT16U Length) {
  INT16U AddrH = 0, AddrL = 0;

  if ((Addr > 0x0003FFFF) || (Length > 0x0400)) {
    return (FALSE);
  }

  AddrH = Addr >> 16;  // 取高16数据
  AddrL = Addr;        // 取低16数据

  __SendHead();      // 发送帧头
  __SendByte(0x91);  // 发送命令
  __SendDByte(AddrH);
  __SendDByte(AddrL);
  __SendDByte(Length);
  __SendTail();  // 发送帧尾

  return (TRUE);
}

/*********************************************************************************************************
** 函数名称:  HMT_SendCMD_ReadRTC
** 函数功能:  发送 读取RTC时钟值 指令 0x9B
** 输入参数:  无
** 输出参数:  无
** 返 回 值:  无
*********************************************************************************************************/
void FixajEkran::HMT_SendCMD_ReadRTC(void) {
  __SendHead();      // 发送帧头
  __SendByte(0x9B);  // 发送命令
  __SendTail();      // 发送帧尾
}


/*********************************************************************************************************
** 函数名称:  HMT_SetRTC
** 函数功能:  RTC时钟值设置0x9C        
** 输入参数:        Year       年 0-99
        Month       月 0-12
        Date       日 0-31
        Hour       时 0-23
        Minute       分 0-59
        Second        秒 0-59
** 输出参数:  无
** 返 回 值:  正确返回TRUE，否则返回FALSE
*********************************************************************************************************/
BOOLEAN FixajEkran::HMT_SetRTC(INT8U Year, INT8U Month, INT8U Date, INT8U Hour, INT8U Minute, INT8U Second) {
  //参数检查
  if (Year > 99 || Month > 12 || Date > 31 || Hour > 23 || Minute > 59 || Second > 59) {
    return (FALSE);
  }

  __SendHead();      // 发送帧头
  __SendByte(0x9C);  // 发送命令
  __SendByte(Year);
  __SendByte(Month);
  __SendByte(Date);
  __SendByte(Hour);
  __SendByte(Minute);
  __SendByte(Second);
  __SendTail();  // 发送帧尾

  return (TRUE);
}



/*********************************************************************************************************
** 函数名称:  HMT_SendCMD_ReadUSR_Bin
** 函数功能:  发送  读USR_Bin文件数据 指令   0x93    地址范围：0x0000 0000 ~0x0003 FFFF 
** 输入参数:  
        Addr    读USR_Bin文件地址
        Length 字长  x0001 ~0x0400
        Data    读出数据
** 输出参数:  无
** 返 回 值:  正确返回TRUE，否则返回FALSE
*********************************************************************************************************/
BOOLEAN FixajEkran::HMT_SendCMD_ReadUSR_Bin(INT32U Addr, INT16U Length) {
  INT16U AddrH = 0, AddrL = 0;

  if ((Addr > 0x0003FFFF) || (Length > 0x0400)) {
    return (FALSE);
  }

  AddrH = Addr >> 16;  // 取高16数据
  AddrL = Addr;        // 取低16数据

  __SendHead();      // 发送帧头
  __SendByte(0x93);  // 发送命令
  __SendDByte(AddrH);
  __SendDByte(AddrL);
  __SendDByte(Length);
  __SendTail();  // 发送帧尾

  return (TRUE);
}

/*********************************************************************************************************
** 函数名称:  HMT_FormatUdisk
** 函数功能:  格式化u盘 0xE2
** 输入参数:  无
** 输出参数:  无
** 返 回 值:  无
*********************************************************************************************************/
void FixajEkran::HMT_FormatUdisk(void) {
  __SendHead();             // 发送帧头
  __SendByte(0xE2);         // 发送命令
  __SendSpecialFuncCode();  // 发送特殊功能指令
  __SendTail();             // 发送帧尾
}

/*********************************************************************************************************
** 函数名称:  HMT_OpenUdisk
** 函数功能:  使U盘能在电脑上识别 0xE3
** 输入参数:  Data ： 密码数据，需要以'\0'结束
** 输出参数:  无
** 返 回 值:  无
*********************************************************************************************************/
void FixajEkran::HMT_OpenUdisk(INT8U *Data) {
  INT16U i = 0;

  __SendHead();      // 发送帧头
  __SendByte(0xE3);  // 发送命令

  while (Data[i] != '\0') {
    __SendByte(Data[i]);
    i++;
  }

  __SendByte('\0');  // 发送结束符
  __SendTail();      // 发送帧尾
}

/*********************************************************************************************************
** 函数名称:  HMT_DispPage
** 函数功能:  跳转页面 0x70
** 输入参数:  usPageID： 页面ID
** 输出参数:  无
** 返 回 值:  正确返回TRUE，否则返回FALSE
*********************************************************************************************************/
BOOLEAN FixajEkran::HMT_DispPage(INT16U usPageID) {
  // 参数检查
  if (usPageID > PAGE_ID_MAX) {
    return (FALSE);
  }

  __SendHead();  // 发送帧头
  __SendByte(0x70);
  __SendDByte(usPageID);
  __SendTail();  // 发送帧尾

  return (TRUE);
}


/*********************************************************************************************************
** 函数名称:  HMT_SetEleForeground
** 函数功能:  设置元素前景色 0x7E
** 输入参数:  elementType： 元素类型
** pageID: 页面ID 
** elementID: 元素ID
** foreGroundcolor: 前景色
** 输出参数:  无
** 返 回 值:  正确返回TRUE，否则返回FALSE
*********************************************************************************************************/
BOOLEAN FixajEkran::HMT_SetEleForeground(INT8U elementType, INT16U pageID, INT8U elementID, INT16U foreGroundcolor) {
  // 参数检查
  if ((elementType > SETElEMENTTYPE_MAX) || (pageID > PAGE_ID_MAX)) {
    return (FALSE);
  }

  if (ELEMENTTYPE_STR == elementType) {
    if (elementID > STRID_MAX) {
      return (FALSE);
    }
  } else {
    if (elementID > NUMID_MAX) {
      return (FALSE);
    }
  }

  __SendHead();                  // 发送帧头
  __SendByte(0x7E);              // 发送命令
  __SendByte(elementType);       // 发送元素类型
  __SendDByte(pageID);           // 发送页面ID
  __SendByte(elementID);         // 发送元素ID
  __SendByte(0x00);              // 预留
  __SendDByte(foreGroundcolor);  // 发送前景色
  __SendTail();                  // 发送帧尾

  return (TRUE);
}


/*********************************************************************************************************
** 函数名称:  HMT_SetEleBackground
** 函数功能:  设置元素背景色 0x7F
** 输入参数:  elementType： 元素类型
** pageID: 页面ID 
** elementID: 元素ID
** transparentMode: 透明模式 0:不透明 1:透明
** backGroundcolor: 背景色
** 输出参数:  无
** 返 回 值:  正确返回TRUE，否则返回FALSE
*********************************************************************************************************/
BOOLEAN FixajEkran::HMT_SetEleBackground(INT8U elementType, INT16U pageID, INT8U elementID, INT8U transparentMode, INT16U backGroundcolor) {
  // 参数检查
  if ((elementType > SETElEMENTTYPE_MAX) || (pageID > PAGE_ID_MAX)) {
    return (FALSE);
  }

  if (ELEMENTTYPE_STR == elementType) {
    if (elementID > STRID_MAX) {
      return (FALSE);
    }
  } else {
    if (elementID > NUMID_MAX) {
      return (FALSE);
    }
  }

  __SendHead();                  // 发送帧头
  __SendByte(0x7F);              // 发送命令
  __SendByte(elementType);       // 发送元素类型
  __SendDByte(pageID);           // 发送页面ID
  __SendByte(elementID);         // 发送元素ID
  __SendByte(transparentMode);   // 发送透明模式
  __SendDByte(backGroundcolor);  // 发送背景色
  __SendTail();                  // 发送帧尾

  return (TRUE);
}


/*********************************************************************************************************
** 函数名称:  HMT_Suspend_VpRefresh
** 函数功能:  暂停VP 刷新0xE8
** 输入参数:  suspendMode：暂停模式
** 0: 退出暂停状态，刷新VP  并且使能触摸按键
** 1: 暂停刷新VP 并且使触摸按键无效
** 输出参数:  无
** 返 回 值:  无
*********************************************************************************************************/
void FixajEkran::HMT_Suspend_VpRefresh(INT8U suspendMode) {
  __SendHead();             // 发送帧头
  __SendByte(0xE8);         // 发送命令
  __SendSpecialFuncCode();  // 发送特殊功能指令
  __SendByte(suspendMode);  // 发送模式
  __SendTail();             // 发送帧尾
}


/*********************************************************************************************************
** 函数名称:  HMT_WirteVPString
** 函数功能:  写VPFlash 字符数据    0x42    地址范围：0x0000 0000 ~0x0001 FFFF
** 输入参数:  
        Addr    写VPFlash地址
        Length 字长  0~128
        Data    读出数据
** 输出参数:  无
** 返 回 值:  正确返回TRUE，否则返回FALSE
*********************************************************************************************************/
BOOLEAN FixajEkran::HMT_WirteVPString(INT32U Addr, INT8U *Data) {
  INT16U i;
  INT16U AddrH = 0, AddrL = 0;

  if (Addr > 0x0001FFFF) {
    return (FALSE);
  }

  AddrH = Addr >> 16;  // 取高16数据
  AddrL = Addr;        // 取低16数据

  __SendHead();      // 发送帧头
  __SendByte(0x42);  // 发送命令
  __SendDByte(AddrH);
  __SendDByte(AddrL);

  for (i = 0; Data[i] != '\0'; i++)  // 如果遇到结束符‘\0? 结束发送
  {
    __SendByte(Data[i]);
  }

  __SendByte('\0');
  __SendTail();  // 发送帧尾

  return (TRUE);
}


/*********************************************************************************************************
** 函数名称:  HMT_SendCMD_ReadVPString
** 函数功能:  发送 读VPFlash 字符数据  指令  0x43    地址范围：0x0000 0000 ~0x0001 FFFF
** 输入参数:  
        Addr 读VPFlash 字符地址
** 输出参数:  无
** 返 回 值:  正确返回TRUE，否则返回FALSE
*********************************************************************************************************/
BOOLEAN FixajEkran::HMT_SendCMD_ReadVPString(INT32U Addr) {
  INT16U AddrH = 0, AddrL = 0;

  if (Addr > 0x0001FFFF) {
    return (FALSE);
  }

  AddrH = Addr >> 16;  // 取高16数据
  AddrL = Addr;        // 取低16数据

  __SendHead();      // 发送帧头
  __SendByte(0x43);  // 发送命令
  __SendDByte(AddrH);
  __SendDByte(AddrL);
  __SendTail();  // 发送帧尾

  return (TRUE);
}

/*********************************************************************************************************
** 函数名称:  HMT_FillVPSTR
** 函数功能:  写字符串VP数据    0x46    地址范围：0x0000 0000 ~0x0001 FFFF
** 输入参数:  
        Addr  写字符串VP地址

** 输出参数:  无
** 返 回 值:  正确返回TRUE，否则返回FALSE
*********************************************************************************************************/
BOOLEAN FixajEkran::HMT_FillVPSTR(INT32U Addr, INT16U Length, INT8U *Data) {
  INT16U i;
  INT16U AddrH = 0, AddrL = 0;

  if ((Addr > 0x0001FFFF) || (Length > 128)) {
    return (FALSE);
  }

  AddrH = Addr >> 16;  // 取高16数据
  AddrL = Addr;        // 取低16数据

  __SendHead();      // 发送帧头
  __SendByte(0x46);  // 发送命令
  __SendDByte(AddrH);
  __SendDByte(AddrL);
  __SendDByte(Length);

  for (i = 0; Data[i] != '\0'; i++)  //如果遇到结束符‘\0? 结束发送
  {
    __SendByte(Data[i]);
  }

  __SendByte('\0');
  __SendTail();  // 发送帧尾

  return (TRUE);
}

/*********************************************************************************************************
** 函数名称:  HMT_WriteVPN16
** 函数功能:  写16bit数字VP数据    0x3D    地址范围：0x0008 0000 ~0x0008 FFFF
** 输入参数:  
        Addr 写16bit数字地址
        Data  数据

** 输出参数:  无
** 返 回 值:  正确返回TRUE，否则返回FALSE
*********************************************************************************************************/
BOOLEAN FixajEkran::HMT_WriteVPN16(INT32U Addr, int Data) {

  INT16U AddrH = 0, AddrL = 0;

  if ((Addr < 0x00080000) || (Addr > 0x0009FFFF)) {
    return (FALSE);
  }

  AddrH = Addr >> 16;  // 取高16数据
  AddrL = Addr;        // 取低16数据

  __SendHead();      // 发送帧头
  __SendByte(0x3D);  // 发送命令
  __SendDByte(AddrH);
  __SendDByte(AddrL);
  __SendDByte(Data);  // 发送数据
  __SendTail();       // 发送帧尾

  return (TRUE);
}

//v2 negatif hariç

BOOLEAN FixajEkran::HMT_WriteVPN16(INT32U Addr, unsigned int Data) {

  INT16U AddrH = 0, AddrL = 0;

  if ((Addr < 0x00080000) || (Addr > 0x0009FFFF)) {
    return (FALSE);
  }

  AddrH = Addr >> 16;  // 取高16数据
  AddrL = Addr;        // 取低16数据

  __SendHead();      // 发送帧头
  __SendByte(0x3D);  // 发送命令
  __SendDByte(AddrH);
  __SendDByte(AddrL);
  __SendDByte(Data);  // 发送数据
  __SendTail();       // 发送帧尾

  return (TRUE);
}


/*********************************************************************************************************
** 函数名称:  HMT_SendCMD_ReadVPN16 
** 函数功能:  发送 读16bit数字VP数据的指令  0x3E    地址范围：0x0008 0000 ~0x0008 FFFF
** 输入参数:  
        Addr 读16bit数字VP地址        
** 输出参数:  无
** 返 回 值:  正确返回TRUE，否则返回FALSE
*********************************************************************************************************/
BOOLEAN FixajEkran::HMT_SendCMD_ReadVPN16(INT32U Addr) {
  INT16U AddrH = 0, AddrL = 0;

  if ((Addr < 0x00080000) || (Addr > 0x0009FFFF)) {
    return (FALSE);
  }

  AddrH = Addr >> 16;  // 取高16数据
  AddrL = Addr;        // 取低16数据

  __SendHead();      // 发送帧头
  __SendByte(0x3E);  // 发送命令
  __SendDByte(AddrH);
  __SendDByte(AddrL);
  __SendTail();  // 发送帧尾

  return (TRUE);
}

/*********************************************************************************************************
** 函数名称:  HMT_FillVPN16
** 函数功能:  填充16bit数字VP数据       0x3F    地址范围：0x0008 0000 ~0x0008 FFFF
** 输入参数:  
        Addr  填充16bit数字VP地址
        Length 字长  0~32767
        Data 数据

** 输出参数:  无
** 返 回 值:  正确返回TRUE，否则返回FALSE
*********************************************************************************************************/
BOOLEAN FixajEkran::HMT_FillVPN16(INT32U Addr, INT16U Length, INT16U Data) {

  INT16U AddrH = 0, AddrL = 0;

  if ((Addr < 0x00080000) || (Addr > 0x0009FFFF) || (Length > 32767)) {
    return (FALSE);
  }

  AddrH = Addr >> 16;  // 取高16数据
  AddrL = Addr;        // 取低16数据

  __SendHead();      // 发送帧头
  __SendByte(0x3F);  // 发送命令
  __SendDByte(AddrH);
  __SendDByte(AddrL);
  __SendDByte(Length);
  __SendDByte(Data);
  __SendTail();  // 发送帧尾

  return (TRUE);
}


/*********************************************************************************************************
** 函数名称:  HMT_WriteVPN32
** 函数功能:  写32bit数字VP数据    0x44    地址范围：0x0002 0000 ~0x0002 FFFF
** 输入参数:  
        Addr 写32bit数字地址
        Data  数据

** 输出参数:  无
** 返 回 值:  正确返回TRUE，否则返回FALSE
*********************************************************************************************************/
BOOLEAN FixajEkran::HMT_WriteVPN32(INT32U Addr, long Data) {
  INT16U Hdata = 0, Ldata = 0;
  INT16U AddrH = 0, AddrL = 0;

  if ((Addr < 0x00020000) || (Addr > 0x0002FFFF)) {
    return (FALSE);
  }

  AddrH = Addr >> 16;                 // 取高16数据
  AddrL = Addr;                       // 取低16数据
  Hdata = (0xFFFF0000 & Data) >> 16;  // 取32位高字节
  Ldata = 0x0000FFFF & Data;          // 取32位低字节

  __SendHead();      // 发送帧头
  __SendByte(0x44);  // 发送命令
  __SendDByte(AddrH);
  __SendDByte(AddrL);
  __SendDByte(Hdata);
  __SendDByte(Ldata);
  __SendTail();  // 发送帧尾

  return (TRUE);
}


/*********************************************************************************************************
** 函数名称:  HMT_SendCMD_ReadVPN32 
** 函数功能:  发送  读32bit数字VP数据的指令    0x45    地址范围：0x0002 0000 ~0x0002 FFFF
** 输入参数:  
        Addr1 读32bit数字VP地址        
** 输出参数:  无
** 返 回 值:  正确返回TRUE，否则返回FALSE
*********************************************************************************************************/
BOOLEAN FixajEkran::HMT_SendCMD_ReadVPN32(INT32U Addr) {
  INT16U AddrH = 0, AddrL = 0;

  if ((Addr < 0x00020000) || (Addr > 0x0002FFFF)) {
    return (FALSE);
  }

  AddrH = Addr >> 16;  // 取高16数据
  AddrL = Addr;        // 取低16数据

  __SendHead();      // 发送帧头
  __SendByte(0x45);  // 发送命令
  __SendDByte(AddrH);
  __SendDByte(AddrL);
  __SendTail();  // 发送帧尾

  return (TRUE);
}


/*********************************************************************************************************
** 函数名称:  HMT_FillVPN32
** 函数功能:  填充32bit数字VP数据       0x47    地址范围：0x0002 0000 ~0x0002 FFFF
** 输入参数:  
        Addr  填充32bit数字VP地址
        Length 字长  0~16384 
        Data 数据

** 输出参数:  无
** 返 回 值:  正确返回TRUE，否则返回FALSE
*********************************************************************************************************/
BOOLEAN FixajEkran::HMT_FillVPN32(INT32U Addr, INT16U Length, INT32U Data) {
  INT16U Hdata = 0, Ldata = 0;
  INT16U AddrH = 0, AddrL = 0;

  if ((Addr < 0x00020000) || (Addr > 0x0002FFFF) || (Length > 16384)) {
    return (FALSE);
  }

  AddrH = Addr >> 16;                 // 取高16数据
  AddrL = Addr;                       // 取低16数据
  Hdata = (0xFFFF0000 & Data) >> 16;  // 取32位高字节
  Ldata = 0x0000FFFF & Data;          // 取32位低字节

  __SendHead();      // 发送帧头
  __SendByte(0x47);  // 发送命令
  __SendDByte(AddrH);
  __SendDByte(AddrL);
  __SendDByte(Length);
  __SendDByte(Hdata);
  __SendDByte(Ldata);
  __SendTail();  // 发送帧尾

  return (TRUE);
}


/*********************************************************************************************************
** 函数名称:  HMT_WriteVPN64
** 函数功能:  写64bit数字VP数据    0x48    地址范围：0x0003 0000 ~0x0003 FFFF
** 输入参数:  
        Addr 写64bit数字VP地址 
        DataH 64bit分解为32bit发送
        DataL
** 输出参数:  无
** 返 回 值:  正确返回TRUE，否则返回FALSE
*********************************************************************************************************/
BOOLEAN FixajEkran::HMT_WriteVPN64(INT32U Addr, INT32U DataH, INT32U DataL) {
  INT16U Hdata1 = 0, Hdata2 = 0, Ldata1 = 0, Ldata2 = 0;
  INT16U AddrH = 0, AddrL = 0;

  if ((Addr < 0x00030000) || (Addr > 0x0003FFFF)) {
    return (FALSE);
  }

  AddrH = Addr >> 16;                   // 取高16数据
  AddrL = Addr;                         // 取低16数据
  Hdata1 = (0xFFFF0000 & DataH) >> 16;  // 取32位高字节
  Hdata2 = 0x0000FFFF & DataH;          // 取32位低字节
  Ldata1 = (0xFFFF0000 & DataL) >> 16;  // 取32位高字节
  Ldata2 = 0x0000FFFF & DataL;          // 取32位低字节

  __SendHead();      // 发送帧头
  __SendByte(0x48);  // 发送命令
  __SendDByte(AddrH);
  __SendDByte(AddrL);
  __SendDByte(Hdata1);
  __SendDByte(Hdata2);
  __SendDByte(Ldata1);
  __SendDByte(Ldata2);
  __SendTail();  // 发送帧尾

  return (TRUE);
}


/*********************************************************************************************************
** 函数名称:  HMT_SendCMD_ReadVPN64 
** 函数功能:  发送 读64bit数字VP数据的指令    0x49    地址范围：0x0003 0000 ~0x0003 FFFF
** 输入参数:  
        Addr  读64bit数字VP地址 
     
** 输出参数:  无
** 返 回 值:  正确返回TRUE，否则返回FALSE
*********************************************************************************************************/
BOOLEAN FixajEkran::HMT_SendCMD_ReadVPN64(INT32U Addr) {
  INT16U AddrH = 0, AddrL = 0;

  if (Addr < 0x00030000 || Addr > 0x0003FFFF) {
    return (FALSE);
  }

  AddrH = Addr >> 16;  // 取高16数据
  AddrL = Addr;        // 取低16数据

  __SendHead();      // 发送帧头
  __SendByte(0x49);  // 发送命令
  __SendDByte(AddrH);
  __SendDByte(AddrL);
  __SendTail();  // 发送帧尾

  return (TRUE);
}


/*********************************************************************************************************
** 函数名称:  HMT_FillVPN64
** 函数功能:  填充64bit数字VP数据       0x4A    地址范围：0x0003 0000 ~0x0003 FFFF
** 输入参数:  
        Addr  填充64bit数字VP地址

        Length 字长  0 ~8192
        DataH 64bit分解为32bit发送
        DataL
** 输出参数:  无
** 返 回 值:  正确返回TRUE，否则返回FALSE
*********************************************************************************************************/
BOOLEAN FixajEkran::HMT_FillVPN64(INT32U Addr, INT16U Length, INT32U DataH, INT32U DataL) {
  INT16U Hdata1 = 0, Hdata2 = 0, Ldata1 = 0, Ldata2 = 0;
  INT16U AddrH = 0, AddrL = 0;

  if ((Addr < 0x00030000) || (Addr > 0x0003FFFF) || (Length > 8192)) {
    return (FALSE);
  }

  AddrH = Addr >> 16;                   // 取高16数据
  AddrL = Addr;                         // 取低16数据
  Hdata1 = (0xFFFF0000 & DataH) >> 16;  // 取32位高字节
  Hdata2 = 0x0000FFFF & DataH;          // 取32位低字节
  Ldata1 = (0xFFFF0000 & DataL) >> 16;  // 取32位高字节
  Ldata2 = 0x0000FFFF & DataL;          // 取32位低字节

  __SendHead();      // 发送帧头
  __SendByte(0x4A);  // 发送命令
  __SendDByte(AddrH);
  __SendDByte(AddrL);
  __SendDByte(Length);
  __SendDByte(Hdata1);
  __SendDByte(Hdata2);
  __SendDByte(Ldata1);
  __SendDByte(Ldata2);
  __SendTail();  // 发送帧尾

  return (TRUE);
}

//sorunlu [?]
INT8U NumdataArray[MAX_NUMARRAY_SIZE];  //将32bit/16bit 转换成16进制保存
/*********************************************************************************************************
** 函数名称:  HMT_Successive_WriteNumVp
** 函数功能:  连续写数字变量0x82
** 输入参数:  Addr：32bit 变量地址
** dataNum: 数据个数
** data:  外部数据,使用HMT_NumToHexArray将32bit/16bit 数值转换成数组保存
** 输出参数:  无
** 返 回 值:  正确返回TRUE，否则返回FALSE
*********************************************************************************************************/
BOOLEAN FixajEkran::HMT_Successive_WriteNumVp(INT32U Addr, INT8U dataNum, INT8U *Arraydata) {
  INT16U AddrH = 0, AddrL = 0;
  INT32U i = 0;
  INT16U Length = 0;

  // 参数检查
  if ((Addr >= 0x00080000) && (Addr < 0x0009FFFF))  //N16
  {
    Length = dataNum * 2;
  } else if ((Addr >= 0x00020000) && (Addr < 0x0002FFFF))  //N32
  {
    Length = dataNum * 4;
  } else if ((Addr >= 0x00030000) && (Addr < 0x0003FFFF))  //N64
  {
    Length = dataNum * 8;
  } else {
    return (FALSE);
  }

  if (Length == 0) {
    return (FALSE);
  }

  AddrH = Addr >> 16;  // 取高16数据
  AddrL = Addr;        // 取低16数据

  __SendHead();         // 发送帧头
  __SendByte(0x82);     // 发送命令
  __SendDByte(AddrH);   // 发送高16位地址
  __SendDByte(AddrL);   // 发送低16位地址
  __SendByte(dataNum);  // 发送数据个数

  for (i = 0; i < Length; i++) {
    __SendByte(Arraydata[i]);
  }

  __SendTail();  // 发送帧尾

  return (TRUE);
}


/*********************************************************************************************************
** 函数名称:  HMT_NumToHexArray
** 函数功能:    数字转换成16进制保存
** 输入参数:  numHigh32Bit：  高32位数据来源 numLow32Bit：  低32位数据来源 
** outData: 指向输出数组  NumType: 数据类型 Num16Bit/Num32Bit/Num64Bit
** 输出参数:  无
** 返 回 值:    无
*********************************************************************************************************/
void FixajEkran::HMT_NumToHexArray(INT32U numHigh32Bit, INT32U numLow32Bit, INT8U NumType, INT8U *outData) {
  static INT16U arrayIndex = 0;
  static INT8U LastType = 0;

  INT16U High32MDByte = 0, High32LDByte = 0, Low32MDByte = 0, Low32LDByte = 0;

  if (arrayIndex >= MAX_NUMARRAY_SIZE) {
    arrayIndex = 0;  //超过最大值从零开始保存
  }
  if ((NumType != VP_N16) && (NumType != VP_N32) && (NumType != VP_N64))  //检查数据类型
  {
    return;
  }
  if ((LastType != 0) && (LastType != NumType))  //使用过，并且与上一次类型不同时从零开始
  {
    arrayIndex = 0;
  }

  if (NumType == VP_N16)  //16Bit
  {
    Low32LDByte = 0x0000FFFF & numLow32Bit;

    outData[arrayIndex] = (INT8U)(Low32LDByte >> 8);  //写入高8位
    outData[arrayIndex + 1] = (INT8U)(Low32LDByte);   //写入低8位

    arrayIndex += 2;
  } else if (NumType == VP_N32)  //32Bit
  {
    Low32MDByte = (0xFFFF0000 & numLow32Bit) >> 16;
    Low32LDByte = 0x0000FFFF & numLow32Bit;

    outData[arrayIndex] = (INT8U)(Low32MDByte >> 8);  //写入最高位字节
    outData[arrayIndex + 1] = (INT8U)(Low32MDByte);
    outData[arrayIndex + 2] = (INT8U)(Low32LDByte >> 8);
    outData[arrayIndex + 3] = (INT8U)(Low32LDByte);  //写入最低位字节

    arrayIndex += 4;
  } else if (NumType == VP_N64)  //64Bit
  {
    High32MDByte = (0xFFFF0000 & numHigh32Bit) >> 16;
    High32LDByte = 0x0000FFFF & numHigh32Bit;
    Low32MDByte = (0xFFFF0000 & numLow32Bit) >> 16;
    Low32LDByte = 0x0000FFFF & numLow32Bit;

    outData[arrayIndex] = (INT8U)(High32MDByte >> 8);  //写入最高位字节
    outData[arrayIndex + 1] = (INT8U)(High32MDByte);
    outData[arrayIndex + 2] = (INT8U)(High32LDByte >> 8);
    outData[arrayIndex + 3] = (INT8U)(High32LDByte);
    outData[arrayIndex + 4] = (INT8U)(Low32MDByte >> 8);
    outData[arrayIndex + 5] = (INT8U)(Low32MDByte);
    outData[arrayIndex + 6] = (INT8U)(Low32LDByte >> 8);
    outData[arrayIndex + 7] = (INT8U)(Low32LDByte);  //写入最低位字节

    arrayIndex += 8;
  }

  LastType = NumType;
}


/*********************************************************************************************************
** 函数名称:  HMT_Successive_ReadNumVp
** 函数功能:  发送 连续读数字变量的指令0x83
** 输入参数:  Addr：32bit 变量地址
** dataNum: 数据个数
** 输出参数:  无
** 返 回 值:  正确返回TRUE，否则返回FALSE
*********************************************************************************************************/
BOOLEAN FixajEkran::HMT_Successive_ReadNumVp(INT32U Addr, INT8U dataNum) {
  INT16U AddrH = 0, AddrL = 0;

  if (((Addr >= 0x00080000) && (Addr < 0x0009FFFF))
      || ((Addr >= 0x00020000) && (Addr < 0x0002FFFF))
      || ((Addr >= 0x00030000) && (Addr < 0x0003FFFF))) {
    AddrH = Addr >> 16;  // 取高16数据
    AddrL = Addr;        // 取低16数据

    __SendHead();         // 发送帧头
    __SendByte(0x83);     // 发送命令
    __SendDByte(AddrH);   // 发送高16位地址
    __SendDByte(AddrL);   // 发送低16位地址
    __SendByte(dataNum);  // 发送数据个数
    __SendTail();         // 发送帧尾

    return (TRUE);
  } else {
    return (FALSE);
  }
}

/*********************************************************************************************************
** 函数名称:  HMT_WriteBP1
** 函数功能:  BP1图片输入写入       0x4B    地址范围：0x0004 0000 ~0x0005 FFFF
** 输入参数:  
        Addr1 BP1图片输入如写入地址

        Length 数据长度 0~ 98304 

** 输出参数:  无
** 返 回 值:  正确返回TRUE，否则返回FALSE
*********************************************************************************************************/
BOOLEAN FixajEkran::HMT_WriteBP1(INT32U Addr, INT32U Length, const INT8U *bp1date) {
  INT32U i = 0;

  if ((Addr < 0x00040000) || (Addr > 0x0005FFFF) || (Length > 98304)) {
    return (FALSE);
  }

  __SendHead();
  __SendByte(0x4B);
  __SendByte((Addr >> 24) & 0xFF);
  __SendByte((Addr >> 16) & 0xFF);
  __SendByte((Addr >> 8) & 0xFF);
  __SendByte(Addr & 0xFF);
  __SendByte((Length >> 24) & 0xFF);
  __SendByte((Length >> 16) & 0xFF);
  __SendByte((Length >> 8) & 0xFF);
  __SendByte(Length & 0xFF);
  __SendTail();

  for (i = 0; i < Length; i++) {
    __SendByte(bp1date[i]);
  }

  return (TRUE);
}


/*********************************************************************************************************
** 函数名称:  HMT_WriteBP1Compress
** 函数功能:  BP1图片压缩输入写入       0x4C    地址范围：0x0004 0000 ~0x0005 FFFF
** 输入参数:  
        Addr1 地址高字节 
        Addr2 地址低字节    
        Length 数据长度 0~ 98304 

** 输出参数:  无
** 返 回 值:  正确返回TRUE，否则返回FALSE
*********************************************************************************************************/
BOOLEAN FixajEkran::HMT_WriteBP1Compress(INT32U Addr, INT32U Length, INT8U *bp1date) {
  INT32U i = 0;

  if ((Addr < 0x00040000) || (Addr > 0x0005FFFF) || (Length > 98304)) {
    return (FALSE);
  }

  __SendHead();
  __SendByte(0x4C);
  __SendByte((Addr >> 24) & 0xFF);
  __SendByte((Addr >> 16) & 0xFF);
  __SendByte((Addr >> 8) & 0xFF);
  __SendByte(Addr & 0xFF);
  __SendByte((Length >> 24) & 0xFF);
  __SendByte((Length >> 16) & 0xFF);
  __SendByte((Length >> 8) & 0xFF);
  __SendByte(Length & 0xFF);
  __SendTail();

  for (i = 0; i < Length; i++) {
    __SendByte(bp1date[i]);
  }

  return (TRUE);
}

/*********************************************************************************************************
** 函数名称:  HMT_VPBP1_IMAGE_CompressData
** 函数功能:  BP1压缩数据函数
** 输入参数:    
        IMAGE_VALUE_Target    存放压缩后数据指针
        IMAGE_Value_Original  图片原始数据
        bp1_h BP1图片高
        bp1_w  图片宽
** 输出参数:  无
** 返 回 值:  压缩后数据长度
*********************************************************************************************************/
INT32U FixajEkran::HMT_VPBP1_IMAGE_CompressData(INT8U *IMAGE_VALUE_Target, INT8U *IMAGE_Value_Original, INT16U bp1_h, INT16U bp1_w) {
  INT16U Convert_Index = 0;  //原始数据偏移地址
  INT16U Zero_Count = 0;     //0x00个数
  INT16U FF_Count = 0;       //0xFF个数
  INT16U Rev_Index = 0;      //输出数据偏移地址
  INT16U Flag_Zero = 0;
  INT16U Flag_FF = 0;

  while (Convert_Index < bp1_h * bp1_w / 8)  //原始数据偏移地址 一共   [(长x宽)/8] 个字节
  {
    if (IMAGE_Value_Original[Convert_Index] == 0x00)  //当前原始数据是0x00
    {
      if (Flag_FF == 1)  //如果上一次原始数据是0xFF
      {
        Rev_Index = Rev_Index + 2;  //输出数据的偏移地址以2递增,否则输出数据的偏移地址不变
        Flag_FF = 0;                //清除标志位
        FF_Count = 0;               //清除0xFF计数
      }

      Zero_Count++;  //0x00 个数 +1

      if (Zero_Count > 255)  //如果个数大于255
      {
        Rev_Index = Rev_Index + 2;  //输出数据的偏移地址以2递增
        Zero_Count = 1;             //从1开始计数
      }

      IMAGE_VALUE_Target[Rev_Index] = 0x00;                  //在当前输出数据的偏移地址存入0x00
      IMAGE_VALUE_Target[Rev_Index + 1] = Zero_Count;        //下一个地址存入0x00的个数
      Convert_Index++;                                       //原始数据偏移地址递增
      Flag_Zero = 1;                                         //表示遇到过0x00
    } else if (IMAGE_Value_Original[Convert_Index] == 0xFF)  //当前原始数据是0xFF
    {
      if (Flag_Zero == 1)  //如果上一次原始数据是0x00
      {
        Rev_Index = Rev_Index + 2;  //输出数据的偏移地址以2递增,否则输出数据的偏移地址不变
        Flag_Zero = 0;              //清除标志位
        Zero_Count = 0;             //清除0x00计数
      }

      FF_Count++;  //0xFF 个数 +1

      if (FF_Count > 255)  //如果个数大于255
      {
        Rev_Index = Rev_Index + 2;  //输出数据的偏移地址以2递增
        FF_Count = 1;               //从1开始计数
      }

      IMAGE_VALUE_Target[Rev_Index] = 0xFF;          //在当前输出数据的偏移地址存入0xFF
      IMAGE_VALUE_Target[Rev_Index + 1] = FF_Count;  //下一个地址存入0xFF的个数
      Convert_Index++;                               //原始数据偏移地址递增
      Flag_FF = 1;                                   //表示遇到过0xFF
    } else                                           //当前原始数据不是0xFF或者0x00
    {
      if (Flag_Zero == 1)  //上一次遇到了0x00
      {
        Rev_Index = Rev_Index + 2;  //输出数据的偏移地址以2递增
        Flag_Zero = 0;              //清除标志位
        Zero_Count = 0;             //清除0x00计数
      } else if (Flag_FF == 1)      //上一次遇到了0xFF
      {
        Rev_Index = Rev_Index + 2;  //输出数据的偏移地址以2递增
        Flag_FF = 0;                //清除标志位
        FF_Count = 0;               //清除0xFF计数
      }

      IMAGE_VALUE_Target[Rev_Index] = IMAGE_Value_Original[Convert_Index];  //清除0xFF计数
      Rev_Index++;
      Convert_Index++;
    }
  }

  if (Flag_Zero == 1 || Flag_FF == 1)  //遇到过0xFF或者0x00
  {
    Rev_Index = Rev_Index + 2;  //需要偏移2个字节
    Flag_Zero = 0;
    Flag_FF = 0;
  }

  return Rev_Index;
}



/*********************************************************************************************************
** 函数名称:  HMT_WriteG16ALL
** 函数功能:  G16静态数据写入    0x4D    地址范围：0x0006 0000 ~0x0007 FFFF
** 输入参数:  
        Addr 静态数据写入地址

        Length 数据长度
        Data  数据
** 输出参数:  无
** 返 回 值:  正确返回TRUE，否则返回FALSE
*********************************************************************************************************/
BOOLEAN FixajEkran::HMT_WriteG16ALL(INT32U Addr, INT16U Length, INT16U *Data) {
  INT16U i, H_Addr = 0, L_Addr = 0;

  if ((Addr < 0x00060000) || (Addr > 0x0007FFFF) || (Length > 1024)) {
    return (FALSE);
  }

  H_Addr = Addr >> 16;
  L_Addr = Addr & 0x0000FFFF;

  __SendHead();      // 发送帧头
  __SendByte(0x4D);  // 发送命令
  __SendDByte(H_Addr);
  __SendDByte(L_Addr);
  __SendDByte(Length);

  for (i = 0; i < Length; i++) {
    __SendDByte(Data[i]);
  }

  __SendTail();  // 发送帧尾

  return (TRUE);
}

BOOLEAN FixajEkran::HMT_WriteG16ALL2(INT32U Addr, unsigned int Length , int *Data) {
  INT16U i, H_Addr = 0, L_Addr = 0;

  if ((Addr < 0x00060000) || (Addr > 0x0007FFFF) || (Length > 1024)) {
    return (FALSE);
  }

  H_Addr = Addr >> 16;
  L_Addr = Addr & 0x0000FFFF;

  __SendHead();      // 发送帧头
  __SendByte(0x4D);  // 发送命令
  __SendDByte(H_Addr);
  __SendDByte(L_Addr);
  __SendDByte(Length);

  for (i = 0; i < Length; i++) {
    __SendDByte(Data[i]);
  }

  __SendTail();  // 发送帧尾

  return (TRUE);
}

/*********************************************************************************************************
** 函数名称:  HMT_WriteG16Last
** 函数功能:  G16最后一个数据写入    0x4E    地址范围：0x0006 0000 ~0x0007 FFFF
** 输入参数:  
        Addr 最后一个数据写入地址    
    
        Length 数据长度
        Data  数据
** 输出参数:  无
** 返 回 值:  正确返回TRUE，否则返回FALSE
*********************************************************************************************************/
BOOLEAN FixajEkran::HMT_WriteG16Last(INT32U Addr, INT16U Length, INT16U Data) {
  INT16U H_Addr = 0, L_Addr = 0;

  if ((Addr < 0x00060000) || (Addr > 0x0007FFFF) || (Length > 1024)) {
    return (FALSE);
  }

  H_Addr = Addr >> 16;
  L_Addr = Addr & 0x0000FFFF;

  __SendHead();      // 发送帧头
  __SendByte(0x4E);  // 发送命令
  __SendDByte(H_Addr);
  __SendDByte(L_Addr);
  __SendDByte(Length);
  __SendDByte(Data);
  __SendTail();  // 发送帧尾

  return (TRUE);
}

/*********************************************************************************************************
** 函数名称:  HMT_SysRegisterWrite
** 函数功能:  系统寄存器写操作0x3B
** 输入参数:  
        Addr 写32bit数字地址
        Data  数据
** 输出参数:  无
** 返 回 值:  正确返回TRUE，否则返回FALSE
*********************************************************************************************************/
BOOLEAN FixajEkran::HMT_SysRegisterWrite(INT32U Addr, INT8U Data) {

  INT16U AddrH = 0, AddrL = 0;

  if ((Addr < 0x00FFFF00) || (Addr > 0x00FFFFFF)) {
    return (FALSE);
  }

  AddrH = Addr >> 16;  // 取高16数据
  AddrL = Addr;        // 取低16数据

  __SendHead();        // 发送帧头
  __SendByte(0x3B);    // 发送命令
  __SendDByte(AddrH);  // 发送高16位地址
  __SendDByte(AddrL);  // 发送低16位地址
  __SendByte(Data);    // 发送数据
  __SendTail();        // 发送帧尾

  return (TRUE);
}

/*********************************************************************************************************
** 函数名称:  HMT_SysRegisterRead
** 函数功能:  发送 读取系统寄存器的指令 0x3C
** 输入参数:  
        Addr 写32bit数字地址
** 输出参数:  无
** 返 回 值:  正确返回TRUE，否则返回FALSE
*********************************************************************************************************/
BOOLEAN FixajEkran::HMT_SysRegisterRead(INT32U Addr) {

  INT16U AddrH = 0, AddrL = 0;

  if ((Addr < 0x00FFFF00) || (Addr > 0x00FFFFFF)) {
    return (FALSE);
  }

  AddrH = Addr >> 16;  // 取高16数据
  AddrL = Addr;        // 取低16数据

  __SendHead();        // 发送帧头
  __SendByte(0x3C);    // 发送命令
  __SendDByte(AddrH);  // 发送高16位地址
  __SendDByte(AddrL);  // 发送低16位地址
  __SendTail();        // 发送帧尾

  return (TRUE);
}


/*********************************************************************************************************
 END FILE
*********************************************************************************************************/





//ekleme son
