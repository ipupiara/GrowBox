#include "stdafx.h"
#include "USBDLL.H"



//
// PN 7. Nov 2005
//
// copyied most from AN21XX.DLL sample "USB_Interface.cpp"  
// for Visual C++ by Karsten Böhme, 
//

USBBoardIF::USBBoardIF()
{
	m_hLibInstance = NULL;
	DeviceNumber = InvalidDeviceNumber;
	openLib();
	openDevice();
}



USBBoardIF::~USBBoardIF()
{
	CloseAllDevices();
	DeviceNumber = InvalidDeviceNumber;
	closeLib();
}


void USBBoardIF::openLib()
{
	if ((m_hLibInstance = LoadLibrary(libName)) == NULL)
	{ MessageBox(NULL,"Error: Loading Library failed",libName, MB_OK);
	}
	else
    { 
		TRACE2("successfully loaded %s id %i\n",libName,m_hLibInstance);
	}
}

void USBBoardIF::closeLib()
{
	if(m_hLibInstance != NULL) {
		if (FreeLibrary(m_hLibInstance)) {
			TRACE0("Library successfully closed\n");
		} else {
			TRACE0("Library close error\n");
		}		
	}
}


int USBBoardIF::GetDeviceError (BYTE DevNumber)
{
	DWORD (_stdcall *function)(BYTE);

	if (!(function = (DWORD (_stdcall *)(BYTE))GetProcAddress(m_hLibInstance, "GetDeviceError")))
	{
		m_LastSystemError = GetLastError();
		TRACE1("USBBoardIF::GetDeviceError last system error %i \n",m_LastSystemError);
		return 0xFFFFFFFF;
	}

	DWORD retval=(*function)(DevNumber);

	m_LastSystemError = 0;
	return (retval);
}


char * USBBoardIF::ConvertStringToPascalDLL(LPCSTR s)
{
	UINT len = strlen(s);
	char *s1 = new char[len+5];
	
	memset(s1, '\0', len+5);
	strcpy(s1+4, s);
	*((DWORD *)s1) = len;

	return s1;
}



int USBBoardIF::OpenDevice(BYTE DevNumber, char* DriverName, char* ID, char* KeyWord)
{

	WORD (_stdcall *function)(BYTE, char *, char *, char *);

	if (!(function = (WORD (_stdcall *)(BYTE, char *, char *, char *))GetProcAddress(m_hLibInstance, "OpenDevice")))
	{
		m_LastSystemError = GetLastError();
		TRACE1("USBBoardIF::OpenDevice m_LastSystemError %X \n",m_LastSystemError);
		return FALSE;
	}

	char *s0 = ConvertStringToPascalDLL(DriverName);
	char *s1 = ConvertStringToPascalDLL(ID);
	char *s2 = ConvertStringToPascalDLL(KeyWord);

	WORD retval = (*function)(DevNumber, PascalString(s0), PascalString(s1), PascalString(s2));
	
	delete [] s0;
	delete [] s1;
	delete [] s2;
	
	return (m_LastSystemError == 0);
}


void USBBoardIF::openDevice()
{
	DeviceNumber = InvalidDeviceNumber;			// ungültige DeviceNumber als Initialisierung
	int devError;

	for(BYTE DeviceNr = 0; DeviceNr <= 31; ++ DeviceNr)  {
		OpenDevice(DeviceNr,"EZUSB","","");
		devError = GetDeviceError(DeviceNr);
		if (devError == Err_No) {
			DeviceNumber = DeviceNr;
			break;
		} else
		{
//			TRACE2("USBBoardIF::openDevice trying DeviceNumber = %i result: %i \n",DeviceNr, devError);
			CloseDevice(DeviceNr);
		}
	}
	TRACE1("USBBoardIF::openDevice set DeviceNumber = %i \n",DeviceNumber);

	SetDeviceEnabled ( DeviceNumber, IMode_1);			// Firmeware für Controllermodus 1 laden
	if (Err_No != (devError =GetDeviceError( DeviceNumber ))) {	// auf Fehler prüfen
		CloseDevice ( DeviceNumber );
		DeviceNumber = InvalidDeviceNumber;
		TRACE1("USBBoardIF::openDevice last error %i \n",devError);
	} else
	{
	}
}


int USBBoardIF::GetDeviceError()
{
	return GetDeviceError(DeviceNumber);
}


int  USBBoardIF::CloseDevice(BYTE DevNumber)
{
	void (_stdcall *function)(BYTE);

	if (!(function = (void (_stdcall *)(BYTE))GetProcAddress(m_hLibInstance, "CloseDevice")))
	{
		m_LastSystemError = GetLastError();
		TRACE1("USBBoardIF::CloseDevice m_LastSystemError %X \n",m_LastSystemError);
		return FALSE;
	}

	(*function)(DevNumber);

	m_LastSystemError = 0;
	return (TRUE);
}

int USBBoardIF::SetDeviceEnabled(BYTE DevNumber, BYTE MC_Mode)
{
	void (_stdcall *function)(BYTE, BYTE);

	if (!(function = (void (_stdcall *)(BYTE, BYTE))GetProcAddress(m_hLibInstance, "SetDeviceEnabled")))
	{
		m_LastSystemError = GetLastError();
		TRACE1("USBBoardIF::SetDeviceEnabled m_LastSystemError %X \n",m_LastSystemError);
		return FALSE;
	}
	(*function)(DevNumber, MC_Mode);
	return (m_LastSystemError == 0);
}


int  USBBoardIF::CloseAllDevices(void)
{
	void (_stdcall *function)();

	if (!(function = (void (_stdcall *)())GetProcAddress(m_hLibInstance, "CloseAllDevices")))
	{
		m_LastSystemError = GetLastError();
		TRACE1("USBBoardIF::CloseAllDevices m_LastSystemError %X \n",m_LastSystemError);
		return FALSE;
	}

	(*function)();

	m_LastSystemError = 0;
	return TRUE;
}

int  USBBoardIF::IsEnabled(BYTE DevNumber, BYTE *MC_Mode)
{
	BYTE mode;
	BOOL (_stdcall *function)(BYTE, BYTE *);

	if (!(function = (BOOL (_stdcall *)(BYTE, BYTE *))GetProcAddress(m_hLibInstance, "IsEnabled")))
	{
		m_LastSystemError = GetLastError();
		TRACE1("USBBoardIF::IsEnabled m_LastSystemError %X \n",m_LastSystemError);
		return FALSE;
	}

	BOOL retval = (*function)(DevNumber, &mode);
    *MC_Mode = mode;

	m_LastSystemError = 0;
	return (retval);
}

int  USBBoardIF::IsOpened(BYTE DevNumber)
{
	BOOL (_stdcall *function)(BYTE);

	if (!(function = (BOOL (_stdcall *)(BYTE))GetProcAddress(m_hLibInstance, "IsOpened")))
	{
		m_LastSystemError = GetLastError();
		TRACE1("USBBoardIF::IsOpened m_LastSystemError %X \n",m_LastSystemError);
		return FALSE;
	}

	BOOL retval = (*function)(DevNumber);

	m_LastSystemError = 0;
	return (retval);
}

int  USBBoardIF::SetBootEEP(BYTE DevNumber, BYTE EEPType, BYTE Addr, WORD DeviceID)
{
	void (_stdcall *function)(BYTE, BYTE, BYTE, WORD);

	if (!(function = (void (_stdcall *)(BYTE, BYTE, BYTE, WORD))GetProcAddress(m_hLibInstance, "SetBootEEP")))
		{
		m_LastSystemError = GetLastError();
		TRACE1("USBBoardIF::SetBootEEP m_LastSystemError %X \n",m_LastSystemError);
		return FALSE;
		}


	(*function)(DevNumber, EEPType, Addr, DeviceID);

	return (m_LastSystemError == 0);
}

int  USBBoardIF::GetDeviceDescriptor (BYTE DevNumber,TUSB_DEVICE_DESCRIPTOR *usbDD)
{
	void (_stdcall *function)(BYTE, TUSB_DEVICE_DESCRIPTOR*);

	if (!(function = (void (_stdcall *)(BYTE,TUSB_DEVICE_DESCRIPTOR*))GetProcAddress(m_hLibInstance, "GetDeviceDescriptor")))
		{
		m_LastSystemError = GetLastError();
		TRACE1("USBBoardIF::GetDeviceDescriptor m_LastSystemError %X \n",m_LastSystemError);
		return FALSE;
		}

	(*function)(DevNumber, usbDD);

	m_LastSystemError = 0;
	return (TRUE);
}

int  USBBoardIF::SetClockDelay (BYTE DevNumber,BYTE Delay)
{
	void (_stdcall *function)(BYTE, BYTE);

	if (!(function = (void (_stdcall *)(BYTE,BYTE))GetProcAddress(m_hLibInstance, "SetClockDelay")))
		{
		m_LastSystemError = GetLastError();
		TRACE1("USBBoardIF::SetClockDelay m_LastSystemError %X \n",m_LastSystemError);
		return FALSE;
		}

	(*function)(DevNumber, Delay);

	m_LastSystemError = 0;
	return (TRUE);
}

BYTE USBBoardIF::PB_ReadByte(BYTE DevNumber,BYTE Addr)
{
	BYTE (_stdcall *function)(BYTE, BYTE);

	if (!(function = (BYTE (_stdcall *)(BYTE, BYTE))GetProcAddress(m_hLibInstance, "PB_ReadByte")))
		{
		m_LastSystemError = GetLastError();
		TRACE1("USBBoardIF::PB_ReadByte m_LastSystemError %X \n",m_LastSystemError);
		return 0;
		}

	BYTE retval = (*function)(DevNumber, Addr);

	m_LastSystemError = 0;
	return retval;
}

void USBBoardIF::closeDevice()
{
	CloseDevice(DeviceNumber);
	DeviceNumber = InvalidDeviceNumber;	
}

BYTE USBBoardIF::readByte(BYTE outB)
{
	return PB_ReadByte(DeviceNumber, outB);
}
