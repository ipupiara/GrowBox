#include "stdafx.h"
#include "CSerial.h"
#include "GrowBoxDefines.h"


CSerial::CSerial(){
};


CSerial::~CSerial(){
};



BOOL CSerial::Open_port(void)
{
 CString portname ="com1" ; 
//	portname = "//./"+ portname; 

	hComm = CreateFile(portname,
					  GENERIC_READ | GENERIC_WRITE,
					  0,
					  0,
					  OPEN_EXISTING,
					  0,
					  0);
	if(hComm==INVALID_HANDLE_VALUE)	{
		MessageBox(NULL,"unable to open the port ",portname,MB_OK+MB_ICONERROR);
		return false;
	}
	else
	return Configure_port();
}
//*****************************************************

BOOL CSerial::Configure_port(void)
{
	GetCommState(hComm, &m_dcb);

 m_dcb.BaudRate= 4800 ; 
 m_dcb.ByteSize = 8; 
 m_dcb.fParity =0 ; 
 m_dcb.Parity = NOPARITY ;
 m_dcb.StopBits = ONESTOPBIT ;

 m_dcb.fBinary=TRUE;
 m_dcb.fDsrSensitivity=false;
 m_dcb.fOutX=false;
 m_dcb.fInX=false;
 m_dcb.fNull=false;
 m_dcb.fAbortOnError=TRUE;
 m_dcb.fOutxCtsFlow=FALSE;
 m_dcb.fOutxDsrFlow=false;
 m_dcb.fDtrControl=DTR_CONTROL_DISABLE;
 m_dcb.fDsrSensitivity=false;
 m_dcb.fRtsControl=RTS_CONTROL_DISABLE;
 m_dcb.fOutxCtsFlow=false;
 m_dcb.fOutxCtsFlow=false;

	m_bPortReady = SetCommState(hComm, &m_dcb);
	if(m_bPortReady ==0)	{
		MessageBox(NULL,"SetCommState Error","Error",MB_OK+MB_ICONERROR);
		CloseHandle(hComm);
		return false;
	}

	COMMTIMEOUTS touts;

    if (! GetCommTimeouts(hComm, &touts) )	{
		MessageBox(NULL,"GetCommTimeout Error","Error",MB_OK+MB_ICONERROR);
		CloseHandle(hComm);
		return false;
	}

	touts.ReadTotalTimeoutConstant = GBSerialReadTotalTimeoutConstant;
	touts.ReadIntervalTimeout = GBReadIntervalTimeout;
    if (! SetCommTimeouts(hComm, &touts) )	{
		MessageBox(NULL,"SetCommTimeout Error","Error",MB_OK+MB_ICONERROR);
		CloseHandle(hComm);
		return false;
	}

 return true;
}
//**************************************************
/*
BOOL CSerial::Write_port(void)
{
 iBytesWritten=0;
 if(WriteFile(hComm,&Byte2Write,1,&iBytesWritten,NULL)==0)
 return false;
 else return true;
}
*/
//***************************************************

BOOL CSerial::Read_port( void* lpBuffer, DWORD len, DWORD* dwBytesTransferred)
{
	return ReadFile (hComm, lpBuffer, len, dwBytesTransferred, 0);
 // return value will not actually be used and does not make much sense anyhow  ????
}

//**************************************************

BOOL CSerial::Close_port(void)
{
 CloseHandle(hComm);
 return true;
}
//*************************************************
// main function 
