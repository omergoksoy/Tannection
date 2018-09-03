/*
  Tannection.h - Tannection library
  Copyright (c) 2018 Omer GOKSOY.  All right reserved.
*/

/*
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
	#include "WProgram.h"
#endif
*/
#ifndef Tannection_H

#define Tannection_H

#define _tanCommHardSerial 10
#define _tanCommSoftSerial 20
#define _tanCommI2CSerial 30

#define _tanNodeStatusActive 125
#define _tanNodeStatusUnknown 100

#define _tanNetStatusOn 60
#define _tanNetStatusOff 70
#define _tanNetStatusUnknown 80

#define _tanErrNoError 10000
#define _tanErrInvalidPortNo 10001
#define _tanErrInvalidPortType 10002
#define _tanErrInvalidPortValue 10003

#define _tanInfoEmptyPort 20001

#define _tanPortDigitalIn 1
#define _tanPortDigitalOut 2
#define _tanPortDigitalWrite 3
#define _tanPortPwmWrite 3
#define _tanPortAnalogIn 4
#define _tanPortServo 5


#include "Arduino.h"
#include "Wire.h"
//#include "SoftwareSerial.h"
//#include "WProgram.h"

// library interface description
const char sbtMesajBaslangicChr[] 	PROGMEM = "$$";
const char sbtMesajBitisChr[] 		PROGMEM = "##";


class Tannection{
  // user-accessible "public" interface
  //friend void serialEvent();  
  
  public:
	//volatile unsigned char okunanStr[15]={'\0'};
	char okunanStr[15] = {'\0'};
	boolean okumaBitti = false;
	byte okumaDiziSiraNo = 0;

	Tannection(void);
	
    void cevapVer(char gelenMesaj[]); //dhcp durumu belirtiliyor...
    boolean setDHCP(boolean durum); //dhcp durumu belirtiliyor...
	boolean setIP(byte adres1,byte adres2,byte adres3,byte adres4); //ip adresi tanımlanıyor...
    boolean setDNS(byte adres1,byte adres2,byte adres3,byte adres4); // dns adresi tanımlanıyor...
    boolean setGateWay(byte adres1,byte adres2,byte adres3,byte adres4); // gate way adresi tanımlanıyor...
    boolean setSubNetMask(byte adres1,byte adres2,byte adres3,byte adres4); // sub net mask tanımlanıyor...
	void update();
    byte isOnline(); //çevrim içi olma durumu kontrol ediliyor...
    void connect(); // net bağlantısı başlatılıyor...

    //void setDigitalInPort(byte portNo, int portType); //portun durumu tanımlanıyor...
    //void setDigitalOutPort(byte portNo, int portType); //portun durumu tanımlanıyor...
    //void setAnalogPort(byte portNo, int portType); //portun durumu tanımlanıyor...
	
	int getPortType(byte portNo); //portun türünü geri gönderiyor...
    boolean isPortEmpty(byte portNo); //portun durumu tanımlanıyor...
    void setPort(byte portNo, int portType); //portun durumu tanımlanıyor...
    int setValue(byte portNo, int portValue); //portun değeri atanıyor...
    int getValue(byte portNo); //portun değeri alınıyor...
    void attachOnValueChanged(void (*callback)(uint8_t,uint16_t)); //portun değeri alınıyor...

	void setNetConfig(void (*callback)()); //portun değeri alınıyor...
	void defineAllPorts(void (*callback)()); //portun değeri alınıyor...
	void onLostComm(void (*callback)()); //portun değeri alınıyor...
	void onReady(void (*callback)()); //portun değeri alınıyor...
	void onDisconnect(void (*callback)()); //portun değeri alınıyor...
	void onConnect(void (*callback)()); //portun değeri alınıyor...
	
	void begin(byte connectionType=_tanCommI2CSerial,byte slaveID=0);
  private:
	//#define portDiziBoyut = 100; //kullanıcılara açılacak port adeti
    long portCheckSum=0; //portların durumlarina gore hesaplanan checksum degeri
    long degerCheckSum=0; //portların degerlerine gore hesaplanan checksum degeri
    void calculateCheckSum(); //portun durumu tanımlanıyor...
	
	unsigned int portDizi[100] = {0}; //TÜM PORTLARIN DURUM VE DEĞERLERİNİ TUTAN DİZİ
	byte _baglantiTuru = _tanCommHardSerial;  //0 - bilinmiyor, 1-online, 2-offline
	//SoftwareSerial mySerial(10, 11); // RX, TX	
	volatile byte cihazDurum = _tanNodeStatusUnknown; //#define _tanNodeStatusActive 125 		//#define _tanNodeStatusUnknown 100
    volatile unsigned long cihazDurumZaman=0; //cihaz ile son bağlantı zamani
	
	volatile byte netDurum = _tanNetStatusUnknown;  //0 - bilinmiyor, 1-online, 2-offline
    volatile unsigned long netDurumZaman=0; //son internet'e bağlantı zamani
	
	volatile boolean onReady_Tanimlandi=false;
	volatile boolean onDisconnect_Tanimlandi=false;
	volatile boolean onConnect_Tanimlandi=false;
	volatile boolean defineAllPorts_Tanimlandi=false;
	volatile boolean onLostComm_Tanimlandi=false;
	
	boolean defineAllPorts_Calisti=false;
	boolean onReady_Calisti=false;
	byte onDisconnect_Calisti=0; //0 = çalışmadı, 1=
	boolean onConnect_Calisti=false; //0 = çalışmadı, 1=
	byte onLostComm_Calisti=0; //
	byte setNetConfig_Calisti=0; //
	
	boolean dhcpAktif = true; 
	byte ipAdres[4] = {192, 168, 1, 105}; //ip adresi
	byte dnsAdres[4] = {199, 168, 1, 1}; //dns adresi
	byte subMask[4] = {255, 255, 255, 0}; //alt ağ maskesi
	byte gateway[4] = {192, 168, 1, 1}; //geçiş kapısı
    unsigned int portDurumDegistir(boolean portDurum, byte portTur, int portDeger);
	
	unsigned int zamanAsimiSure=5000;
    boolean ipAdresYazdir(byte adres1,byte adres2,byte adres3,byte adres4);
    void zamanAsimKontrol();
    void mesajiTamamla();
    void mesajBaslangicGonder();
    void gelenCheckSumHesapla(byte baslangicNo);
    void komutKontrol();
	boolean seriMesajOku();
    boolean cevapBekle(void);
    //boolean cevapBekleV2(void);
    //void starting(void);
	//starting();
};

//Tannection _Tannection;
#endif

