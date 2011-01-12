#ifndef USBDLLHEADER
#define USBDLLHEADER

/* PN 6. Nov 2005 copied most code into this wrapper class from samples of

DLL zur Verwendung des Cypress USB-Controllers

Copyright .2001 - 2004 by Karsten Böhme
Karsten Böhme
Auf der Bismarckhöhe 8
01737 Tharandt
Tel : 035203 / 32758
Mobil : 0160 / 6763917
Fax : 069 / 1330 444 6109
Mail : karsten.boehme@arcor.de

which you can find under  http://www.braintechnology.de/braintechnology/usb1_1dll.html


and 

IO-Karten in eigenen Anwendungen.doc,
USB8IO_dcl.pas also by Karsten Böhme
*/


#include "stdafx.h" 


typedef struct

	{
    BYTE bLength; 
    BYTE bDescriptorType;
    LONG bcdUSB;
    BYTE bDeviceClass;
    BYTE bDeviceSubClass;
    BYTE bDeviceProtocol;
    BYTE bMaxPacketSize0;
    LONG idVendor;
    LONG idProduct;
    LONG bcdDevice;
    BYTE iManufacturer;
    BYTE iProduct;
    BYTE iSerialNumber;
    BYTE bNumConfigurations;
} TUSB_DEVICE_DESCRIPTOR;
  
 

 
#define		Pin0	(BYTE)0x01
#define		Pin1	(BYTE)0x02
#define		Pin2	(BYTE)0x04
#define		Pin3	(BYTE)0x08
#define		Pin4	(BYTE)0x10
#define		Pin5	(BYTE)0x20
#define		Pin6	(BYTE)0x40
#define		Pin7	(BYTE)0x80



// Konstanten für die EEPROM-Funktionen, Parameter: BYTE EEPType
// EEP_WriteByte(), EEP_WriteBlock(), EEP_ReadByte(), EEP_ReadBlock()
#define		C00 	(BYTE)0
#define		C01 	(BYTE)1
#define		C02 	(BYTE)2
#define		C04 	(BYTE)3
#define		C08 	(BYTE)4
#define		C16 	(BYTE)5
#define		C32 	(BYTE)6
#define		C64 	(BYTE)7
#define		C128	(BYTE)8
#define		C256	(BYTE)9

#define PortA 0
#define PortB 1
#define PortC 2



#define IMode_1 1
#define IMode_2 2
#define IMode_3 3 // Not possible with AN 2131
#define IMode_4 4 // Not possible with AN 2131 
#define IMode_5 5 
#define IMode_6 6 


#define libName "USB8IO.dll"

#define PascalString(s)	(s+4)

#define InvalidDeviceNumber 255




#define    Err_No              0x00000000    // kein Fehler aufgetreten
/*    Err_DeviceNotOpened   : DWord = $000001;    // kein gültiges Device-Handle (Device nicht geöffnet)
    Err_DeviceNumber      : DWord = $000002;    // unzulässige Device-Nummer
    Err_DriverName        : DWord = $000004;    // Es wurde bereits unter einem Treibernamen Enabled gesetzt
    Err_DeviceNotEnabled  : DWord = $000008;    // keine Firmeware geladen bzw. unbehebbarer Fehler aufgetreten (nach Err_DeviceIOCtl)
    Err_DeviceIOCtl       : DWord = $000010;    // Funktion DeviceI0Control hat 'False' zurückgeliefert
    Err_AI2CNotAck        : DWord = $000020;    // I2C-Bus NotAcknoledge nach Slaveadresse
    Err_DI2CNotAck        : DWord = $000040;    // I2C-Bus NotAcknoledge nach Datum
    Err_I2CBus            : DWord = $000080;    // allgemeiner I2C-Busfehler
    Err_Array             : DWord = $000100;    // ByteArray hat unerlaubte Dimension
    Err_McMode            : DWord = $000200;    // Methode im aktuellen Controllermodus nicht verfügbar
    Err_Pointer           : DWord = $000400;    //  Ungültige RAM-/EEP-Adresse übergeben
    Err_DownLoadFile      : DWord = $000800;    // Datei existiert nicht oder kann nicht gelesen werden
    Err_PinNumber         : DWord = $001000;    // unzulässige Pinnummer
    Err_EEPType           : DWord = $002000;    // unzulässiger EEPROM-Typ
    Err_ClockDelay        : DWord = $004000;    // unzulässige Taktverzögerung
    Err_InterruptNumber	  : DWord = $008000;	  // Interrupt-Nummer existiert nicht
    Err_InterruptHandling : DWord = $010000;    // Fehler bei Abfrage der Interrupt-Flags
    Err_SetBootEEP        : DWord = $020000;    // Unzulässige DeviceID an die Methode 'SetBootEEP' bzw. Array wurde nicht korrekt geschrieben

*/


class USBBoardIF 
{
public:
	USBBoardIF();
	~USBBoardIF();

public:
	BYTE readByte(BYTE outB);
	void closeDevice();
	int GetDeviceError();
	void openDevice();

private:
	HINSTANCE m_hLibInstance;
	DWORD m_LastSystemError;
	void openLib();
	void closeLib();
	char * ConvertStringToPascalDLL(LPCSTR s);
	BYTE DeviceNumber;


//  methods for the calls to the USB8IO.dll. Methods that are not used in
//  this application, were not tested
//
	int OpenDevice(BYTE DevNumber, char* DriverName, char* ID, char* KeyWord);
	int GetDeviceError (BYTE DevNumber);
	int CloseDevice(BYTE DevNumber);
	int SetDeviceEnabled(BYTE DevNumber, BYTE MC_Mode);
	int  CloseAllDevices(void);
	int  IsEnabled(BYTE DevNumber, BYTE *MC_Mode);
	int  IsOpened(BYTE DevNumber);
	int  SetBootEEP(BYTE DevNumber, BYTE EEPType, BYTE Addr, WORD DeviceID);
	int  GetDeviceDescriptor (BYTE DevNumber,TUSB_DEVICE_DESCRIPTOR *usbDD);
	int  SetClockDelay (BYTE DevNumber,BYTE Delay);
	BYTE PB_ReadByte(BYTE DevNumber,BYTE Addr);
};

#endif