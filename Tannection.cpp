/*
  Tannection.cpp - library for Wiring - implementation
  Copyright (c) 2018 Omer GOKSOY.
*/
#include "Tannection.h"
#include "Wire.h"
#include "SoftwareSerial.h"

static void (*on_tannection_connect)(); // Pointer to callback function to handle result of current HTTP request
static void (*on_tannection_disconnect)(); // Pointer to callback function to handle result of current HTTP request
static void (*set_internet_configuration)(); // Pointer to callback function to handle result of current HTTP request
static void (*define_all_ports)(); // Pointer to callback function to handle result of current HTTP request
static void (*on_tannection_on_lost_comm)(); // Pointer to callback function to handle result of current HTTP request
static void (*on_tannection_ready)(); // Pointer to callback function to handle result of current HTTP request
static void (*on_value_changed_cb)(uint8_t,uint16_t); // Pointer to callback function to handle result of current HTTP request
/*
#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

// include description files for other libraries used (if any)
#include "HardwareSerial.h"
*/
// Constructor /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances


// Public Methods //////////////////////////////////////////////////////////////
// Functions available in Wiring sketches, this library, and other libraries

Tannection::Tannection(void){
}

boolean Tannection::setDHCP(boolean durum){ //dhcp durumu belirtiliyor...
	mesajBaslangicGonder();
	Serial.print("dhcp:");
	if(durum==true){
		Serial.print("1");
	}else{
		Serial.print("0");
	}
	dhcpAktif = durum;
	mesajiTamamla();
	/*
	boolean sonuc=cevapBekle();
	if(sonuc==true){
		//Serial.println("DHCP true");
	}else{
		//Serial.println("DHCP false");
	}
	return sonuc;
	*/
}

boolean Tannection::setIP(byte adres1,byte adres2,byte adres3,byte adres4){ //ip adresi tanımlanıyor...
	mesajBaslangicGonder();
	Serial.print("ip:");
	boolean sonuc=ipAdresYazdir(adres1,adres2,adres3,adres4);
	ipAdres[0] = adres1;
	ipAdres[1] = adres2;
	ipAdres[2] = adres3;
	ipAdres[3] = adres4;
	return sonuc;
}

boolean Tannection::setDNS(byte adres1,byte adres2,byte adres3,byte adres4){ // dns adresi tanımlanıyor...
	mesajBaslangicGonder();
	Serial.print("dns:");
	boolean sonuc=ipAdresYazdir(adres1,adres2,adres3,adres4);
	dnsAdres[0] = adres1;
	dnsAdres[1] = adres2;
	dnsAdres[2] = adres3;
	dnsAdres[3] = adres4;
	return sonuc;
}

boolean Tannection::setGateWay(byte adres1,byte adres2,byte adres3,byte adres4){ // gate way adresi tanımlanıyor...
	mesajBaslangicGonder();
	Serial.print("gate:");
	boolean sonuc=ipAdresYazdir(adres1,adres2,adres3,adres4);
	gateway[0] = adres1;
	gateway[1] = adres2;
	gateway[2] = adres3;
	gateway[3] = adres4;
	return sonuc;
}

/*
void Tannection::setDigitalInPort(byte portNo, int portType){ //belirtilen portu dijital giriş olarak tanımlıyor...

}
*/
byte Tannection::isOnline(){ //eğer çevrim içiyse işlem yapacak
	if(okumaBitti==true){
		if( okunanStr[0]=='r' && okunanStr[1]=='d' && okunanStr[2]=='y' && okunanStr[3]==';' ){
			netDurum=_tanNetStatusUnknown;
		}else{
			if( okunanStr[0]=='o' && okunanStr[1]=='n' && okunanStr[2]=='n' && okunanStr[3]==';'){ //çevrim içi
				netDurum=_tanNetStatusOn;
			}else{
				if( okunanStr[0]=='o' && okunanStr[1]=='f' && okunanStr[2]=='f' && okunanStr[3]==';'){ //çevrim dışı
					netDurum=_tanNetStatusOff;
				}else{
				}
			}
		}
		okumaBitti=false;
		okumaDiziSiraNo=0;
		memset(okunanStr, 0, sizeof(okunanStr)); //burada diziyi sıfırla
	}
	return netDurum;
}

boolean Tannection::setSubNetMask(byte adres1,byte adres2,byte adres3,byte adres4){ // sub net mask tanımlanıyor...
	mesajBaslangicGonder();
	Serial.print("mask:");
	boolean sonuc=ipAdresYazdir(adres1,adres2,adres3,adres4);
	subMask[0] = adres1;
	subMask[1] = adres2;
	subMask[2] = adres3;
	subMask[3] = adres4;
	return sonuc;
}

void Tannection::connect(){ // net bağlantısı başlatılıyor...
	netDurum=_tanNetStatusUnknown;
	mesajBaslangicGonder();
	Serial.print("run");
	mesajiTamamla();
	/*
	boolean sonuc=cevapBekle();
	if(sonuc==true){
		Serial.println("true");
	}else{
		Serial.println("false");
	}
	*/
}

int Tannection::getPortType(byte portNo){ //port kullanımda mı diye kontrol ediyor..
	if(portNo>0 && portNo<101){ //1 ila 100 arasındaki değerleri diziye yönlendiriyor...
		unsigned int portIntDeger=portDizi[portNo - 1];
		portIntDeger = portIntDeger >> 1;
		int portTurDeger = ((portIntDeger & 1) == 1 ? 1 : 0);
		portTurDeger = portTurDeger | ((portIntDeger & 2) == 2 ? 2 : 0);
		portTurDeger = portTurDeger | ((portIntDeger & 4) == 4 ? 4 : 0);
		portTurDeger = portTurDeger | ((portIntDeger & 8) == 8 ? 8 : 0);
		portTurDeger = portTurDeger | ((portIntDeger & 16) == 16 ? 16 : 0);
		return portTurDeger;
	}else{
		return 0;
	}
}

boolean Tannection::isPortEmpty(byte portNo){ //port kullanımda mı diye kontrol ediyor..
	if(portNo>0 && portNo<101){ //1 ila 100 arasındaki değerleri diziye yönlendiriyor...
		return ((portDizi[portNo-1] & 1) == 1 ? false : true);
	}else{
		//return _tanErrInvalidPortNo;
	}
}

int Tannection::setValue(byte portNo, int newValue){ //portun değeri atanıyor...
	if(portNo>0 && portNo<101){ //1 ila 100 arasındaki değerleri diziye yönlendiriyor...
		int portTur =getPortType(portNo);
		unsigned int sonucDeger = 1;
		unsigned int yedSayi = portTur << 1;
		sonucDeger = sonucDeger | yedSayi;
		unsigned int yeniPortDeger = newValue << 6;
		portDizi[portNo-1]= sonucDeger | yeniPortDeger;
		mesajBaslangicGonder();
		Serial.print("v");
		Serial.print(portNo);
		Serial.print(":");
		Serial.print(newValue);
		mesajiTamamla();
		calculateCheckSum();
		/*
		boolean sonuc=cevapBekle();
		if(sonuc==true){
			return _tanErrNoError;
		}else{
		}
		*/
	}else{
		return _tanErrInvalidPortNo;
	}
}
int Tannection::getValue(byte portNo){ //portun değeri alınıyor...
	if(portNo>0 && portNo<101){ //1 ila 100 arasındaki değerleri diziye yönlendiriyor...
		unsigned int portIntDeger=portDizi[portNo-1];
		boolean portDurum = ((portIntDeger & 1) == 1 ? true : false);
		if(portDurum==true){
			unsigned int portDeger = portIntDeger >> 6;
			return portDeger;
		}else{
			return _tanInfoEmptyPort;
		}
	}else{
		return _tanErrInvalidPortNo;
	}
}

void Tannection::calculateCheckSum(){ //portun durumu tanımlanıyor...
	const unsigned long crc_table[16] = {
		0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
		0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
		0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
		0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
	};
	portCheckSum=0;
	degerCheckSum=0;
	for(byte portNo=1; portNo<101; portNo++){
		int portTur=0;
		int portDeger=0;
		if(isPortEmpty(portNo)==false){
			portTur=portNo + getPortType(portNo);
			portDeger=portNo + getValue(portNo);
		}

		portCheckSum = crc_table[(portCheckSum ^ portTur) & 0x0f] ^ (portCheckSum >> 4);
		portCheckSum = crc_table[(portCheckSum ^ (portTur >> 4)) & 0x0f] ^ (portCheckSum >> 4);
		portCheckSum = ~portCheckSum;
		
		degerCheckSum = crc_table[(degerCheckSum ^ portDeger) & 0x0f] ^ (degerCheckSum >> 4);
		degerCheckSum = crc_table[(degerCheckSum ^ (portDeger >> 4)) & 0x0f] ^ (degerCheckSum >> 4);
		degerCheckSum = ~degerCheckSum;
		
		/*
		$$off:1082876417;##
		$$off:1082876417;##
		$$off:1082876417;##
		
		portCheckSum = crc_table[(portCheckSum ^ portDizi[portNo]) & 0x0f] ^ (portCheckSum >> 4);
		portCheckSum = crc_table[(portCheckSum ^ (portDizi[portNo] >> 4)) & 0x0f] ^ (portCheckSum >> 4);
		portCheckSum = ~portCheckSum;
		*/
		//long portCheckSum=0; //portların durumlarina gore hesaplanan checksum degeri
		//long degerCheckSum=0; //portların degerlerine gore hesaplanan checksum degeri
	}
	/*
	Serial.print("portCheckSum :");
	Serial.println(portCheckSum);
	$$off;##
	*/
}
void Tannection::setPort(byte portNo, int portType){ //portun durumu tanımlanıyor...
	if(portNo>0 && portNo<101){ //1 ila 100 arasındaki değerleri diziye yönlendiriyor...
		unsigned int donenDeger=portDurumDegistir(true, portType,0);
		portDizi[portNo-1]=donenDeger;
		mesajBaslangicGonder();
		Serial.print("p");
		Serial.print(portNo);
		Serial.print(":");
		Serial.print(portType);
		//boolean sonuc=cevapBekle();
		mesajiTamamla();
		calculateCheckSum();
	}else{
		Serial.println("hatali port");
		
	}
}

unsigned int Tannection::portDurumDegistir(boolean portDurum, byte portTur, int portDeger){
  int sonucDeger = (portDurum == true ? 1 : 0);
  int yedSayi = portTur << 1;
  sonucDeger = sonucDeger | yedSayi;
  int yeniPortDeger = portDeger << 6;
  sonucDeger = sonucDeger | yeniPortDeger;
  return sonucDeger;
}

void Tannection::setNetConfig(void (*callback)()){ 
	set_internet_configuration=callback;
}
void Tannection::defineAllPorts(void (*callback)()){ 
	for(byte portNo=0; portNo<100; portNo++){ portDizi[portNo]=0; }
	defineAllPorts_Tanimlandi=true;
	define_all_ports=callback;
}
void Tannection::onLostComm(void (*callback)()){ 
	on_tannection_on_lost_comm=callback; 
	onLostComm_Tanimlandi=true;
}
void Tannection::onReady(void (*callback)()){ 
	on_tannection_ready=callback; 
	onReady_Tanimlandi=true;
}
void Tannection::onConnect(void (*callback)()){ //portun değeri alınıyor...
	on_tannection_connect=callback;
	onConnect_Tanimlandi=true;
}
void Tannection::onDisconnect(void (*callback)()){ //portun değeri alınıyor...
	on_tannection_disconnect=callback;
	onDisconnect_Tanimlandi=true;
}

void Tannection::attachOnValueChanged(void (*callback)(uint8_t,uint16_t)){ //port değeri değişince
	on_value_changed_cb=callback;
	/*
	(*on_value_changed_cb)(4,0); //fonksiyonun çalışma şekli
	*/
}

//void Tannection::begin(_tanCommHardSerial ){ //portun değeri atanıyor...
//void Tannection::begin(_tanCommI2CSerial ){ //portun değeri atanıyor...
//void Tannection::begin(byte connectionType=_tanCommI2CSerial){ //portun değeri atanıyor...
void Tannection::begin(byte connectionType=_tanCommI2CSerial,byte slaveID=0 ){ //portun değeri atanıyor...
	/*
	#define _tanCommHardSerial 10
	#define _tanCommI2CSerial 30
	#define _tanCommSoftSerial 20
	*/
	
	_baglantiTuru=connectionType;
	if(_baglantiTuru==_tanCommHardSerial){
		Serial.begin(19200);
		while (!Serial) {
			; // wait for serial port to connect. Needed for native USB port only
		}
		//mesajBaslangicGonder();
		//Serial.print("rdy");
		//mesajiTamamla();
	}else if(_baglantiTuru==_tanCommSoftSerial){
		
	}else if(_baglantiTuru==_tanCommI2CSerial){
		
	}
}
// Private Methods /////////////////////////////////////////////////////////////
// Functions only available to other functions in this library

void Tannection::mesajBaslangicGonder(){
  for (byte i = 0; i < strlen_P(sbtMesajBaslangicChr); i++) {
    char harf =  pgm_read_byte_near(sbtMesajBaslangicChr +  i);
    Serial.print(harf);
  }
}

void Tannection::mesajiTamamla(void){
	Serial.print(";");
	for (byte i = 0; i < strlen_P(sbtMesajBitisChr); i++) {
		char harf =  pgm_read_byte_near(sbtMesajBitisChr +  i);
		Serial.print(harf);
	}
	Serial.println("");
	Serial.flush();
}
boolean Tannection::seriMesajOku(){
	if(okumaBitti==false){
		while(Serial.available()){
			char inChar = (char)Serial.read(); // get the new byte:
			if (inChar == '\n'){
				okumaBitti=true;
			}else{
				okunanStr[okumaDiziSiraNo] = inChar;
				okumaDiziSiraNo++;
			}
		}
	}
	if(okumaBitti==true){
		boolean mesajUygunMu=false;
		if(okunanStr[0] == pgm_read_byte_near(sbtMesajBaslangicChr) && okunanStr[1] == pgm_read_byte_near(sbtMesajBaslangicChr + 1)) {
			for (byte i = 0; i < (sizeof(okunanStr) - 2); i++) {
				okunanStr[i] = okunanStr[i + 2];
			}
			for (byte i = 0; i < (sizeof(okunanStr) - 1); i++) {
				if (okunanStr[i] == pgm_read_byte_near(sbtMesajBitisChr)) {
					if (okunanStr[i + 1] == pgm_read_byte_near(sbtMesajBitisChr + 1)) {
						okunanStr[i] = '\0';
						mesajUygunMu=true;
					}
				}
			}
		}
		return mesajUygunMu;
	}else{
		return false;
	}
}
boolean Tannection::cevapBekle(void){
	mesajiTamamla();
	return true;
	/*
	$$off;##
	boolean Sonuc=false;
	boolean cevapGeldi=false;
	//for(unsigned long sayac=0; sayac<5000 && cevapGeldi==false;sayac++){
	for(unsigned long sayac=0; sayac<5000000 && cevapGeldi==false;sayac++){
		seriMesajOku();
		if(okumaBitti==true){
			cevapGeldi=true;
		}
	}
	*/
	
	/*
	Serial.println(okunanStr);
	if(okumaBitti==true){
		//Serial.println("bitti");
	}else{
		//Serial.println("bitmedi");
	}
	if(cevapGeldi==true){
		if(okunanStr[0]=='o' && okunanStr[1]=='k' && okunanStr[2]==';'){
			for(byte i=0;i<sizeof(okunanStr); i++){ okunanStr[i]='\0'; }
			okumaDiziSiraNo=0;
			okumaBitti=false;
			Sonuc=true;
			//Serial.println("uygun cevap geldi");
		}else{
			//Serial.print("gelen yed cevap : ");
			//Serial.print("gg ( ");
			//Serial.print(okunanStr);
			//Serial.print(" ) gg");
			//Serial.println("");
			for(byte i=0;i<sizeof(okunanStr); i++){ okunanStr[i]='\0'; }
			okumaDiziSiraNo=0;
			okumaBitti=false;
			Sonuc=true;
		}
	}else{
		Sonuc=false;
		for(byte i=0;i<sizeof(okunanStr); i++){ okunanStr[i]='\0'; }
		okumaDiziSiraNo=0;
		okumaBitti=false;
		//Serial.println("cevap yok");
	}
	return Sonuc;
	*/
}

boolean Tannection::ipAdresYazdir(byte adres1,byte adres2,byte adres3,byte adres4){
	Serial.print(String(adres1));
	Serial.print(".");
	Serial.print(String(adres2));
	Serial.print(".");
	Serial.print(String(adres3));
	Serial.print(".");
	Serial.print(String(adres4));
	mesajiTamamla();
	//boolean sonuc=cevapBekle();
	//return sonuc;
}

void Tannection::zamanAsimKontrol(){ //gelen komutlar kontrol ediliyor...
	unsigned long suAnMillis=millis();
	if (suAnMillis - cihazDurumZaman >= zamanAsimiSure) {
		cihazDurum=_tanNodeStatusUnknown;
		netDurum=_tanNetStatusUnknown;
	}
	//Serial.println(portCheckSum);
	//Serial.println(portCheckSum);
	mesajBaslangicGonder();
	Serial.print("rdy:");
	//long result = atol(yedBuffer);
	if(portCheckSum==0){
		Serial.print("0");
	}else{
		char yedBuffer[14]={'\0'};
		ltoa(portCheckSum,yedBuffer,10);
		for(byte i=0;i<sizeof(yedBuffer);i++){
			if(yedBuffer[i]!='\0'){
				Serial.print(yedBuffer[i]);
			}
		}
	}
	mesajiTamamla();

	/*
	Serial.print("fark : ");
	Serial.println((suAnMillis - netDurumZaman));
	if (suAnMillis - netDurumZaman >= ( zamanAsimiSure * 2 ) ) {
		//burada online veya offline durumunda
		netDurum=_tanNetStatusUnknown;
	}
	*/
}

void Tannection::gelenCheckSumHesapla(byte baslangicNo){ //gelen komutlar kontrol ediliyor...
	boolean bulundu=false;
	char yedBuffer[15]={'\0'};
	byte diziKonumNo=0;
	for(byte i=baslangicNo; i<sizeof(okunanStr) && bulundu==false; i++){
		if(okunanStr[i]==';'){
			bulundu=true;
		}else{
			yedBuffer[diziKonumNo]=okunanStr[i];
			diziKonumNo++;
		}
	}
	long hesaplananCheckSum=atol(yedBuffer);
	if(portCheckSum==hesaplananCheckSum){
		Serial.println("esit");
	}else{
		Serial.println("farkli");
	}
}
void Tannection::komutKontrol(){ //gelen komutlar kontrol ediliyor...
	//Serial.print("gelen mesaj : ");
	//Serial.println(okunanStr);
	unsigned long suAnMillis=millis();
	if( okunanStr[0]=='o' && okunanStr[1]=='n' && okunanStr[2]==':'){ //çevrim içi
		netDurum=_tanNetStatusOn;
		netDurumZaman=suAnMillis;
		gelenCheckSumHesapla(3);
	}else{
		if( okunanStr[0]=='o' && okunanStr[1]=='f' && okunanStr[2]=='f' && okunanStr[3]==':'){ //çevrim dışı
			netDurum=_tanNetStatusOff;
			netDurumZaman=suAnMillis;
			gelenCheckSumHesapla(4);
		}else{ //tanımlanmayan mesaj geldi
		}
	}
	cihazDurum=_tanNodeStatusActive;
	cihazDurumZaman=suAnMillis;
}

void Tannection::update(){
	//burada eventler kontrol ediliyor...
	if(defineAllPorts_Tanimlandi==true && defineAllPorts_Calisti==false){
		(*define_all_ports)(); //şimdi de port tipleri tanımlanıyor...
		/*
		$$off:1082876417;## //içerik boş ise bu şekilde sonuç oluyor...
		$$rdy:165343027;##
		*/
		
		calculateCheckSum();
		defineAllPorts_Calisti=true;
	}
	
	if(cihazDurum==_tanNodeStatusUnknown){
		/*
		$$rst;##
		$$rdy;##
		*/
		onReady_Calisti=false;
		if(onLostComm_Calisti==1){
			if(onLostComm_Tanimlandi==true){
				(*on_tannection_on_lost_comm)();
			}
			onLostComm_Calisti=2;
			if(onDisconnect_Calisti==1){
				if(onDisconnect_Tanimlandi==true){
					(*on_tannection_disconnect)();
				}
				onDisconnect_Calisti=2;
			}
		}
	}else{
		if(onReady_Calisti==false){
			if(onReady_Tanimlandi==true){
				(*on_tannection_ready)(); //önce ready event'ı çalıştırılıyor...
				(*define_all_ports)(); //şimdi de port tipleri tanımlanıyor...
				calculateCheckSum();
			}
			onReady_Calisti=true;
			onLostComm_Calisti=1;
		}
		if(netDurum==_tanNetStatusOff){
			if(onDisconnect_Calisti==1){
				if(onDisconnect_Tanimlandi==true){
					(*on_tannection_disconnect)();
				}
				onDisconnect_Calisti=2;
			}
			onConnect_Calisti=false;
		}else if(netDurum==_tanNetStatusOn){
			if(onConnect_Calisti==false){
				if(onConnect_Tanimlandi==true){
					(*on_tannection_connect)();
				}
				onConnect_Calisti=true;
				onDisconnect_Calisti=1;
			}
		}else{
			if(setNetConfig_Calisti==0){
				setNetConfig_Calisti=1;
				(*set_internet_configuration)();
			}
		}
	}
	if(seriMesajOku() == true){ //gelen mesaj uygun ise oku
		komutKontrol();
		/*
		Serial.print(portCheckSum);
		Serial.print(" :" );
		Serial.println(yedBuffer);
		*/
	}
	
	if(okumaBitti==true){
		okumaBitti=false;
		okumaDiziSiraNo=0;
		memset(okunanStr, 0, sizeof(okunanStr)); //burada diziyi sıfırla
		//Serial.print("rdy:");
		//2 929 201 218
		//portCheckSum
	}
	zamanAsimKontrol();
	/*
	byte cihazDurum = _tanNodeStatusUnknown; //#define _tanNodeStatusActive 125 		//#define _tanNodeStatusUnknown 100
	byte netDurum = _tanNetStatusUnknown;  //0 - bilinmiyor, 1-online, 2-offline
	*/
}
	
void Tannection::cevapVer(char gelenMesaj[]){
	char yedDizi[50];
	strcpy(yedDizi, okunanStr);
	memset(okunanStr, 0, sizeof(okunanStr)); //burada diziyi sıfırla
	okumaBitti=false;
	okumaDiziSiraNo=0;
	Serial.println("CEVAP VER fonksiyonu : ");
	Serial.println(yedDizi);
}

//Tannection tannection=Tannection();
