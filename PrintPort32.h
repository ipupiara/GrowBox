//
//
//	PN 5. Nov 2005 
//
//  uses directly the source code of the widely used inpout32.dll
//
//  which you can find under www.logix4u.net/inpout32.htm
//
//
//


#include "stdafx.h"


#define IOCTL_READ_PORT_UCHAR	 -1673519100 //CTL_CODE(40000, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_WRITE_PORT_UCHAR	 -1673519096 //CTL_CODE(40000, 0x802, METHOD_BUFFERED, FILE_ANY_ACCESS)



class PrintPort32 {
public:
	BOOL isRunningOK();

	PrintPort32();
	virtual ~PrintPort32();
	void Out32(short PortAddress, short data);
	short Inp32(short PortAddress);
private:
	void RemoveService();
	HANDLE hdriver;
	char path[MAX_PATH];
	BOOL runningOK;

	int SystemVersion();
	int Opendriver();
	int sysver;
	int inst();
	int start(void);
};


