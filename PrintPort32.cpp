
#include "stdafx.h"
#include "conio.h"
#include "stdlib.h" 
#include "PrintPort32.h"
#include "resource.h"
#include <WINSVC.H>

//
// PN 7. Nov 2005:
//
// This code is more or less copied from the source code of inpout32.dll
// not my favorite style of code but it works
// 

PrintPort32::PrintPort32() 
{
	sysver = SystemVersion();

/*	RemoveService();  // just for debug and testing purpose
	sysver = 0;
	runningOK = FALSE;
	return;
*/
	TRACE1("\nconstuctor PrintPort32 version: %i\n",sysver);
	runningOK = TRUE;
	
	if(sysver==2)
	{
		Opendriver();
		TRACE0("\nconstuctor PrintPort32 Driver opened\n");
	}
}


PrintPort32::~PrintPort32()
{
	TRACE0("\ndestructor Printport32\n");
	if(sysver==2)
	{
		CloseHandle(hdriver);
		TRACE0("\ndestructor handle closed\n");
	}
	runningOK = FALSE;
}

int PrintPort32::SystemVersion()

{   
	
	   OSVERSIONINFOEX osvi;
       BOOL bOsVersionInfoEx;

	   ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
       osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	   if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
		{
          osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
         
		  if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) )
			  return 0;  
		}

	      switch (osvi.dwPlatformId)
			  {      
				case VER_PLATFORM_WIN32_NT:

					return 2;		//WINNT

					break;

				case VER_PLATFORM_WIN32_WINDOWS:
						
					return 1;		//WIN9X

					break;

					}   
		  return 0; 
}


int PrintPort32::Opendriver()
{
    hdriver = CreateFile("\\\\.\\inpoutx64", 
                                 GENERIC_READ | GENERIC_WRITE, 
                                 0, 
                                 NULL,
                                 OPEN_EXISTING, 
                                 FILE_ATTRIBUTE_NORMAL, 
                                 NULL);
	
	if(hdriver == INVALID_HANDLE_VALUE) {
		
		if(start())
		{
			inst();
			start();

			 hdriver = CreateFile("\\\\.\\inpoutx64", 
                                 GENERIC_READ | GENERIC_WRITE, 
                                 0, 
                                 NULL,
                                 OPEN_EXISTING, 
                                 FILE_ATTRIBUTE_NORMAL, 
                                 NULL);

		}
	}


	if(hdriver == INVALID_HANDLE_VALUE) {		
		TRACE0("Opendriver return 1");
		runningOK = FALSE;
		return 1;
	} else {

		TRACE0("Opendriver return 0");
		runningOK = TRUE;
		return 0;
	}
}


int PrintPort32::inst()
{

    SC_HANDLE  Mgr;
    SC_HANDLE  Ser;


	GetSystemDirectory(path , sizeof(path));
	HRSRC hResource = FindResource(NULL, MAKEINTRESOURCE(IDR_BIN3), "BIN");
	if(hResource)
	{
		HGLOBAL binGlob = LoadResource(NULL, hResource);
	
		if(binGlob)
		{
			void *binData = LockResource(binGlob);
		
			if(binData)
			{
				HANDLE file;
				strcat(path,"\\Drivers\\inpoutx64.sys");
				
				file = CreateFile(path,
								  GENERIC_WRITE,
								  0,
								  NULL,
								  CREATE_ALWAYS,
								  0,
								  NULL);

				if(file)
				{
					DWORD size, written;

					size = SizeofResource(NULL, hResource);
					WriteFile(file, binData, size, &written, NULL);
					CloseHandle(file);

				}
			}
		}
	}


	Mgr = OpenSCManager (NULL, NULL,SC_MANAGER_ALL_ACCESS);
	    if (Mgr == NULL)
		{							//No permission to create service
			if (GetLastError() == ERROR_ACCESS_DENIED) 
			{
				return 5;  // error access denied
			}
		}	
		else
		{
		   Ser = CreateService (Mgr,                      
                                "inpoutx64",                        
                                "inpoutx64",                        
                                SERVICE_ALL_ACCESS,                
                                SERVICE_KERNEL_DRIVER,             
                                SERVICE_SYSTEM_START,               
                                SERVICE_ERROR_NORMAL,               
                                "System32\\Drivers\\inpoutx64.sys",  
                                NULL,                               
                                NULL,                              
                                NULL,                               
                                NULL,                              
                                NULL                               
                                );




		}

CloseServiceHandle(Ser);
CloseServiceHandle(Mgr);

	return 0;
}
/**************************************************************************/
int PrintPort32::start(void)
{
    SC_HANDLE  Mgr;
    SC_HANDLE  Ser;

	Mgr = OpenSCManager (NULL, NULL,SC_MANAGER_ALL_ACCESS);

	    if (Mgr == NULL)
		{							//No permission to create service
			if (GetLastError() == ERROR_ACCESS_DENIED) 
			{
				Mgr = OpenSCManager (NULL, NULL,GENERIC_READ);
				Ser = OpenService(Mgr,"inpoutx64",GENERIC_EXECUTE);
				if (Ser)
				{    // we have permission to start the service
					if(!StartService(Ser,0,NULL))
					{
						CloseServiceHandle (Ser);
						return 4; // we could open the service but unable to start
					}
					
				}

			}
		}
		else
		{// Successfuly opened Service Manager with full access

				Ser = OpenService(Mgr,"inpoutx64",GENERIC_EXECUTE);
				if (Ser)
				{
					if(!StartService(Ser,0,NULL))
					{
						CloseServiceHandle (Ser);
						return 3; // opened the Service handle with full access permission, but unable to start
					}
					else
					{
						CloseServiceHandle (Ser);
						return 0;
					}

				}

		}

return 1;
}



void PrintPort32::RemoveService()
{
// just a test and sample code, actually not used
	
    SC_HANDLE  Mgr;
    SC_HANDLE  Ser;

	Mgr = OpenSCManager (NULL, NULL,SC_MANAGER_ALL_ACCESS);

	if (Mgr != NULL)
	{

		Ser = OpenService( Mgr,"inpoutx64", DELETE);           

		if (Ser == NULL)
		{ 
			TRACE0("removeService Ser not found\n");
		}

		if (! DeleteService(Ser) ) 
		{
			TRACE0("service not deleted\n");
			MessageBox(NULL,"Service remove error","Remove Service",MB_OK);
		}
		else   {
			TRACE0("service deleted\n"); 
			MessageBox(NULL,"remove ok", "Remove Service", MB_OK);
		}

		CloseServiceHandle(Ser); 
		CloseServiceHandle(Mgr);
	}
}




void PrintPort32::Out32(short PortAddress, short data)
{

	switch(sysver)
	{
	case 1:
//			_outp( PortAddress,data);
			runningOK = TRUE;
	break;

	case 2:
			unsigned int error;
			DWORD BytesReturned;        
			BYTE Buffer[3];
			unsigned short * pBuffer;
			pBuffer = (unsigned short *)&Buffer[0];
			*pBuffer = LOWORD(PortAddress);
			Buffer[2] = LOBYTE(data);

			error = DeviceIoControl(hdriver,
                            IOCTL_WRITE_PORT_UCHAR,
                            &Buffer,
                            3,
                            NULL,
                            0,
                            &BytesReturned,
								NULL);
			if (error != 0) {  // PN 7. NOv 2005 "error" = success accoring to MSDN
				runningOK = TRUE;
			} else  {
				runningOK = FALSE;
			}


	break;
	}

	
}

/*********************************************************************/

short PrintPort32::Inp32(short PortAddress)
{
	BYTE retval;
	switch(sysver)
	{

	case 1:
//		retval = _inp(PortAddress);
		runningOK = TRUE;
	return retval;
	break;
	case 2:
		unsigned int error;
		DWORD BytesReturned;
		unsigned char Buffer[3];
		unsigned short * pBuffer;
		pBuffer = (unsigned short *)&Buffer;
		*pBuffer = LOWORD(PortAddress);
		Buffer[2] = 0;
		error = DeviceIoControl(hdriver,
                            IOCTL_READ_PORT_UCHAR,
                            &Buffer,
                            2,
                            &Buffer,
                            1,
                            &BytesReturned,
                            NULL);
		if (error != 0) {  // PN 7. NOv 2005 "error" = success accoring to MSDN
			runningOK = TRUE;
		} else  {
			runningOK = FALSE;
		}

		return((int)Buffer[0]);

	break;
	}
return 0;
}


BOOL PrintPort32::isRunningOK()
{
	return runningOK;
}
