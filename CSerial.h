#include "stdafx.h"
#include <iostream>


//  PN 8. Nov 2005
//
//  code to access the serial port, copied from a MSDN sample
//
//


class CSerial
{
public:
 CSerial();
 virtual ~CSerial();

 BOOL Open_port(void);
// BOOL Write_port(void);
 BOOL Read_port( void* lpBuffer, DWORD len,DWORD* dwBytesTransferred);
 BOOL Close_port(void);

private:

 HANDLE hComm;
 DCB      m_dcb;
 COMMTIMEOUTS m_CommTimeouts;
 BOOL     m_bPortReady;
 BOOL     bWriteRC;
 BOOL     bReadRC;
 DWORD iBytesWritten;
 DWORD iBytesRead;
 DWORD dwBytesRead;

 BOOL Configure_port(void);
} ;