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


#include "stdint.h"
#ifndef FixajEkran_h
#define FixajEkran_h

#if !defined(ARDUINO_ARCH_STM32) && !defined(ESP32) && !defined(ARDUINO_ARCH_SAMD) && !defined(ARDUINO_ARCH_MBED) && !defined(__STM32F1__) && !defined(__STM32F4__)
#define ACTIVATE_SOFTWARE_SERIAL
#endif
#if defined(ESP32)
#define HARDWARE_SERIAL_SELECTABLE_PIN
#endif

#ifdef ACTIVATE_SOFTWARE_SERIAL
#include <SoftwareSerial.h>
#endif

#include "sabitler.h"

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define DEBUG_PRINTER Serial

// Setup debug printing macros.
#ifdef FixajEkran_DEBUG
#define DEBUG_PRINT(...) \
  { DEBUG_PRINTER.print(__VA_ARGS__); }
#define DEBUG_PRINTLN(...) \
  { DEBUG_PRINTER.println(__VA_ARGS__); }
#else
#define DEBUG_PRINT(...) \
  {}
#define DEBUG_PRINTLN(...) \
  {}
#endif

#pragma pack(push, 1)
//optimizasyon ayarları
//Bellek hizalama
#pragma pack(pop)

//Butonlar nesne tabanlı tanımlandı kullanıcı
// daha kolay değişken tanımlaması yapabilmesi için

class Buton {
public:
  byte data[3];

  Buton(uint16_t sayfaNo, byte touchID) {
    data[0] = (sayfaNo >> 8) & 0xFF;
    data[1] = sayfaNo & 0xFF;
    data[2] = touchID;
  }
};

class FixajEkran {
public:
#ifdef ACTIVATE_SOFTWARE_SERIAL
  FixajEkran(byte txE32pin, byte rxE32pin, UART_BPS_RATE bpsRate = UART_BPS_RATE_9600);
#endif
  FixajEkran(HardwareSerial* serial, UART_BPS_RATE bpsRate = UART_BPS_RATE_9600);

#ifdef HARDWARE_SERIAL_SELECTABLE_PIN
  FixajEkran(byte txE32pin, byte rxE32pin, HardwareSerial* serial, UART_BPS_RATE bpsRate, uint32_t serialConfig = SERIAL_8N1);
#endif

#ifdef ACTIVATE_SOFTWARE_SERIAL
  FixajEkran(SoftwareSerial* serial, UART_BPS_RATE bpsRate = UART_BPS_RATE_9600);
#endif
  bool begin();
  int available();
  //public kodlarım
  bool yaz(INT32U Addr, INT8U* Data);      //INT8U data[] = "hellop";  yaz(adr, data);
  bool yaz(INT32U Addr, char* Data);       //char data[] = "hellox";   yaz(adr, data);
  bool yaz(INT32U Addr, String abc);       //String abc = "helloy";    yaz(adr, abc)
  bool yaz(INT32U Addr, const char* abc);  //                          yaz(adr, "hello");

  bool yaz(INT32U Addr, int xyz);           //-32,768 ile 32,767
  bool yaz(INT32U Addr, unsigned int xyz);  // 0 ile 65,535

  bool yaz(INT32U Addr, long xyz);
  bool yaz(INT32U Addr, uint64_t xyz);

  bool sayfayaGit(INT16U usPageID);

  template<typename T>
  bool oku(INT32U Addr, T* deger);

  bool okuStr(INT32U Addr, String* deger);  //String abc;       okuStr(adr, &abc);
  bool okuStr(INT32U Addr, char* deger);    //char okuma[64];   okuStr(adr, okuma);

  bool EkranTest();
  bool dokunmaDinle();
  bool butonBasildiMi(const Buton& btn);
  void EkranParlaklikSeviyesi(INT8U ucPWMLevel);
  void BuzzerSesSeviyesi(INT8U ucLevel);
  BOOLEAN SaatTarihAyarla(INT8U Year, INT8U Month, INT8U Date, INT8U Hour, INT8U Minute, INT8U Second);
  BOOLEAN OnplanRengiDegis(INT8U elementType, INT16U pageID, INT8U elementID, INT16U foreGroundcolor);
  BOOLEAN ArkaPlanRengiDegis(INT8U elementType, INT16U pageID, INT8U elementID, INT8U transparentMode, INT16U backGroundcolor);

  BOOLEAN grafikCiz(INT32U Addr, INT16U Length, INT16U* Data);
  BOOLEAN grafikCiz(INT32U Addr, int Length, int* Data); 


private:
  HardwareSerial* hs;
  void cleanUARTBuffer();
  //hmt kendi kodlarım

  template<typename T>
  T byteArrayToDecimal(byte* data, int data_length);
  template<typename T>
  void process_command(byte command, byte* data, int data_length, T* result);
  template<typename T>
  bool read_and_process_data(T* result);
  template<size_t N>
  bool sorgu(byte (&array)[N]);

  bool sorguSTR(String* deger);
  bool sorguSTR(char* deger);



  //temel kodlar

  void UART_SendChar(uint8_t ucDat);
  void __SendByte(INT8U ucData);
  void __SendDByte(INT16U usData);
  void __SendByteStr(INT8U const* pucStr, INT32U ulNum);
  void __SendHead(void);
  void __SendTail(void);
  void __SendSpecialFuncCode(void);

  //hmt eklenti başlangıç
  // buradaki kodlar private olduğu için direk erişilmez
  // ancak ekranın desteklediği bütün kodları buraya gömdüm
  // size lazım olan ancak private olan bir fonksiyonu yukarıdaki public: içine kopyaladığınızda
  // erişime açılacaktır.
  void HMT_HandShake(void);
  void HMT_SendCMD_ReadVersion(void);
  void HMT_SendCMD_ReadPageID(void);
  BOOLEAN HMT_SystemConfig(INT8U usBaudrate, INT8U usTouchAction);
  BOOLEAN HMT_SelectProject(INT8U projectID);
  void HMT_TouchCalibration(void);
  void HMT_SetScreenSaveTime(INT16U ucTime, INT8U ucPWMLevel);
  void HMT_SetBacklightLevel(INT8U ucPWMLevel);
  void HMT_SetBeepLevel(INT8U ucLevel);
  BOOLEAN HMT_BuzzerCtrl(INT8U loopCount, INT8U playTime1, INT8U playTime2, INT8U playFre1, INT8U playFre2);
  BOOLEAN HMT_WriteFlash(INT32U Addr, INT16U Length, INT8U* Data);
  BOOLEAN HMT_SendCMD_ReadFlash(INT32U Addr, INT16U Length);
  void HMT_SendCMD_ReadRTC(void);
  BOOLEAN HMT_SetRTC(INT8U Year, INT8U Month, INT8U Date, INT8U Hour, INT8U Minute, INT8U Second);
  BOOLEAN HMT_SendCMD_ReadUSR_Bin(INT32U Addr, INT16U Length);
  void HMT_FormatUdisk(void);
  void HMT_OpenUdisk(INT8U* Data);
  BOOLEAN HMT_DispPage(INT16U usPageID);
  BOOLEAN HMT_SetEleForeground(INT8U elementType, INT16U pageID, INT8U elementID, INT16U foreGroundcolor);
  BOOLEAN HMT_SetEleBackground(INT8U elementType, INT16U pageID, INT8U elementID, INT8U transparentMode, INT16U backGroundcolor);
  void HMT_Suspend_VpRefresh(INT8U suspendMode);
  BOOLEAN HMT_WirteVPString(INT32U Addr, INT8U* Data);
  BOOLEAN HMT_SendCMD_ReadVPString(INT32U Addr);
  BOOLEAN HMT_FillVPSTR(INT32U Addr, INT16U Length, INT8U* Data);
  BOOLEAN HMT_WriteVPN16(INT32U Addr, int Data);
  BOOLEAN HMT_WriteVPN16(INT32U Addr, unsigned int Data);
  BOOLEAN HMT_SendCMD_ReadVPN16(INT32U Addr);
  BOOLEAN HMT_FillVPN16(INT32U Addr, INT16U Length, INT16U Data);
  BOOLEAN HMT_WriteVPN32(INT32U Addr, long Data);
  BOOLEAN HMT_SendCMD_ReadVPN32(INT32U Addr);
  BOOLEAN HMT_FillVPN32(INT32U Addr, INT16U Length, INT32U Data);
  BOOLEAN HMT_WriteVPN64(INT32U Addr, INT32U DataH, INT32U DataL);
  BOOLEAN HMT_SendCMD_ReadVPN64(INT32U Addr);
  BOOLEAN HMT_FillVPN64(INT32U Addr, INT16U Length, INT32U DataH, INT32U DataL);

  INT8U NumdataArray[MAX_NUMARRAY_SIZE];

  void HMT_NumToHexArray(INT32U numHigh32Bit, INT32U numLow32Bit, INT8U NumType, INT8U* outData);
  BOOLEAN HMT_Successive_WriteNumVp(INT32U Addr, INT8U dataNum, INT8U* Arraydata);
  BOOLEAN HMT_Successive_ReadNumVp(INT32U Addr, INT8U dataNum);
  BOOLEAN HMT_WriteBP1(INT32U Addr, INT32U Length, const INT8U* bp1date);
  BOOLEAN HMT_WriteBP1Compress(INT32U Addr, INT32U Length, INT8U* bp1date);
  INT32U HMT_VPBP1_IMAGE_CompressData(INT8U* IMAGE_VALUE_Target, INT8U* IMAGE_Value_Original, INT16U bp1_h, INT16U bp1_w);
  BOOLEAN HMT_WriteG16ALL(INT32U Addr, INT16U Length, INT16U* Data);
  BOOLEAN HMT_WriteG16ALL2(INT32U Addr, unsigned int Length, int* Data);
  BOOLEAN HMT_WriteG16Last(INT32U Addr, INT16U Length, INT16U Data);
  BOOLEAN HMT_SysRegisterWrite(INT32U Addr, INT8U Data);
  BOOLEAN HMT_SysRegisterRead(INT32U Addr);

  //hmt son


#ifdef ACTIVATE_SOFTWARE_SERIAL
  SoftwareSerial* ss;
#endif

  bool isSoftwareSerial = true;

  int8_t txE32pin = -1;
  int8_t rxE32pin = -1;

#ifdef HARDWARE_SERIAL_SELECTABLE_PIN
  uint32_t serialConfig = SERIAL_8N1;
#endif
  UART_BPS_RATE bpsRate = UART_BPS_RATE_9600;

  struct NeedsStream {
    template<typename T>
    void begin(T& t, uint32_t baud) {
      DEBUG_PRINTLN("Begin ");
      t.setTimeout(500);
      t.begin(baud);
      stream = &t;
    }

#ifdef HARDWARE_SERIAL_SELECTABLE_PIN

    template< typename T >
    void begin(T& t, uint32_t baud, uint32_t config) {
      DEBUG_PRINTLN("Begin ");
      t.setTimeout(500);
      t.begin(baud, config);
      stream = &t;
    }

    template< typename T >
    void begin(T& t, uint32_t baud, uint32_t config, int8_t txE32pin, int8_t rxE32pin) {
      DEBUG_PRINTLN("Begin ");
      t.setTimeout(500);
      t.begin(baud, config, txE32pin, rxE32pin);
      stream = &t;
    }
#endif

    void listen() {}

    Stream* stream;
  };
  NeedsStream serialDef;
};

// Explicit template instantiation declarations
extern template int FixajEkran::byteArrayToDecimal<int>(byte* data, int data_length);
extern template long FixajEkran::byteArrayToDecimal<long>(byte* data, int data_length);
//extern template double FixajEkran::byteArrayToDecimal<double>(byte* data, int data_length);

extern template bool FixajEkran::oku<int>(INT32U Addr, int* deger);
extern template bool FixajEkran::oku<long>(INT32U Addr, long* deger);
//extern template bool FixajEkran::oku<double>(INT32U Addr, double *deger);

extern template void FixajEkran::process_command<int>(byte command, byte* data, int data_length, int* result);
extern template void FixajEkran::process_command<long>(byte command, byte* data, int data_length, long* result);

extern template bool FixajEkran::read_and_process_data<int>(int* result);
extern template bool FixajEkran::read_and_process_data<long>(long* result);


#endif