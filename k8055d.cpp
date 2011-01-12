#include "StdAfx.h"
#include "k8055d.h"

k8055d::k8055d(void)
{
}

k8055d::~k8055d(void)
{
}

long k8055d::init()
{
	long alldigital = 0;
	hDLL = LoadLibrary("k8055d");
	if (hDLL != NULL)
	{
		pOpenDevice = (LONG2LONG)GetProcAddress(hDLL,"OpenDevice");
		if (!pOpenDevice)
		{
			// handle the error
			FreeLibrary(hDLL);       
			return -1;
		}
		pCloseDevice = (VOID2VOID)GetProcAddress(hDLL,"CloseDevice");
		if (!pCloseDevice)
		{
			// handle the error
			FreeLibrary(hDLL);       
			return -1;
		}
		pReadAnalogChannel = (LONG2LONG)GetProcAddress(hDLL,"ReadAnalogChannel");
		if (!pReadAnalogChannel)
		{
			// handle the error
			FreeLibrary(hDLL);       
			return -1;
		}
		pReadAllAnalog = (LONGPLONGP2VOID)GetProcAddress(hDLL,"ReadAllAnalog");
		if (!pReadAllAnalog)
		{
			// handle the error
			FreeLibrary(hDLL);       
			return -1;
		}
		pOutputAnalogChannel = (LONGLONG2VOID)GetProcAddress(hDLL,"OutputAnalogChannel");
		if (!pOutputAnalogChannel)
		{
			// handle the error
			FreeLibrary(hDLL);       
			return -1;
		}
		pOutputAllAnalog = (LONGLONG2VOID)GetProcAddress(hDLL,"OutputAllAnalog");
		if (!pOutputAllAnalog)
		{
			// handle the error
			FreeLibrary(hDLL);       
			return -1;
		}
		pClearAnalogChannel = (LONG2VOID)GetProcAddress(hDLL,"ClearAnalogChannel");
		if (!pClearAnalogChannel)
		{
			// handle the error
			FreeLibrary(hDLL);       
			return -1;
		}
		pClearAllAnalog = (VOID2VOID)GetProcAddress(hDLL,"ClearAllAnalog");
		if (!pClearAllAnalog)
		{
			// handle the error
			FreeLibrary(hDLL);       
			return -1;
		}
		pSetAnalogChannel = (LONG2VOID)GetProcAddress(hDLL,"SetAnalogChannel");
		if (!pSetAnalogChannel)
		{
			// handle the error
			FreeLibrary(hDLL);       
			return -1;
		}
		pSetAllAnalog = (VOID2VOID)GetProcAddress(hDLL,"SetAllAnalog");
		if (!pSetAllAnalog)
		{
			// handle the error
			FreeLibrary(hDLL);       
			return -1;
		}
		pWriteAllDigital = (LONG2VOID)GetProcAddress(hDLL,"WriteAllDigital");
		if (!pWriteAllDigital)
		{
			// handle the error
			FreeLibrary(hDLL);       
			return -1;
		}
		pClearDigitalChannel = (LONG2VOID)GetProcAddress(hDLL,"ClearDigitalChannel");
		if (!pClearDigitalChannel)
		{
			// handle the error
			FreeLibrary(hDLL);       
			return -1;
		}
		pClearAllDigital = (VOID2VOID)GetProcAddress(hDLL,"ClearAllDigital");
		if (!pClearAllDigital)
		{
			// handle the error
			FreeLibrary(hDLL);       
			return -1;
		}
		pSetDigitalChannel = (LONG2VOID)GetProcAddress(hDLL,"SetDigitalChannel");
		if (!pSetDigitalChannel)
		{
			// handle the error
			FreeLibrary(hDLL);       
			return -1;
		}
		pSetAllDigital = (VOID2VOID)GetProcAddress(hDLL,"SetAllDigital");
		if (!pSetAllDigital)
		{
			// handle the error
			FreeLibrary(hDLL);       
			return -1;
		}
		pReadDigitalChannel = (LONG2BOOL)GetProcAddress(hDLL,"ReadDigitalChannel");
		if (!pReadDigitalChannel)
		{
			// handle the error
			FreeLibrary(hDLL);       
			return -1;
		}
		pReadAllDigital = (VOID2LONG)GetProcAddress(hDLL,"ReadAllDigital");
		if (!pReadAllDigital)
		{
			// handle the error
			FreeLibrary(hDLL);       
			return -1;
		}
		pReadCounter = (LONG2LONG)GetProcAddress(hDLL,"ReadCounter");
		if (!pReadCounter)
		{
			// handle the error
			FreeLibrary(hDLL);       
			return -1;
		}
		pResetCounter = (LONG2VOID)GetProcAddress(hDLL,"ResetCounter");
		if (!pResetCounter)
		{
			// handle the error
			FreeLibrary(hDLL);       
			return -1;
		}
		pSetCounterDebounceTime = (LONGLONG2VOID)GetProcAddress(hDLL,"SetCounterDebounceTime");
		if (!pSetCounterDebounceTime)
		{
			// handle the error
			FreeLibrary(hDLL);       
			return -1;
		}
		initialized = 1;

		dout1 = 0;
		dout2 = 0;
		dout3 = 0;
		dout4 = 0;
		dout5 = 0;
		dout6 = 0;
		dout7 = 0;
		dout8 = 0;
		
		din1  = 0;
		din2  = 0;
		din3  = 0;
		din4  = 0;
		din5  = 0;

		ct1   = 0;
		ct2   = 0;
		dbct1 = 0;
		dbct2 = 0;

		da1   = 0;
		da2   = 0;
		ad1   = 0;
		ad2   = 0;
	}
	return -1;
}

long  k8055d::OpenDevice(long CardAddress)
{
	if (!initialized) return -1;
	if ((CardAddress >= 0) && (CardAddress <= 3))
		return pOpenDevice(CardAddress);
	else 
		return -1;
}

void  k8055d::CloseDevice()
{
	pCloseDevice();
}

long  k8055d::ReadAnalogChannel(long Channel)
{
	if (!initialized) 
		return -1;
	if (Channel == 1)
		return ad1 = pReadAnalogChannel(Channel);
	else
		return ad2 = pReadAnalogChannel(Channel);
}

void  k8055d::ReadAllAnalog(long *Data1, long *Data2)
{
	if (initialized)
	{
		pReadAllAnalog(Data1, Data2);
		ad1 = *Data1;
		ad2 = *Data2;
	}
}

void  k8055d::OutputAnalogChannel(long Channel, long Data)
{
	if (initialized) 
	{
		if (Channel == 1)
		{
			if (da1 != Data)
			{
				pOutputAnalogChannel(Channel, Data);
				da1 = Data;
			}
		}
		else 
		{
			if (da2 != Data)
			{
				pOutputAnalogChannel(Channel, Data);
                da2 = Data;
			}
		}
	}
}

void  k8055d::OutputAllAnalog(long Data1, long Data2)
{
	if (initialized && ((Data1 != da1) || (Data2 != da2))) 
	{
		pOutputAllAnalog(Data1, Data2);
		da1 = Data1;
		da2 = Data2;
	}
}

void  k8055d::ClearAnalogChannel(long Channel)
{	
	if (initialized) 
	{
		pClearAnalogChannel(Channel);
		if (Channel == 1)
			da1 = 0;
		else 
			da2 = 0;
	}
}

void  k8055d::ClearAllAnalog()
{
	if (initialized)
	{
		pClearAllAnalog();
		da1 = 0;
		da2 = 0;
	}
}

void  k8055d::SetAnalogChannel(long Channel)
{
	if (initialized)
	{
		pSetAnalogChannel(Channel); 
		if (Channel == 1) 
			da1 = 255;
		else
			da2 = 255;
	}
}

void  k8055d::SetAllAnalog()
{	
	if (initialized) 
		pSetAllAnalog();
	da1 = 255;
	da2 = 255;
}

void  k8055d::WriteAllDigital(long Data)
{
	long old = 0;
	old = dout1+dout2*2+dout3*4+dout4*8+dout5*16+dout6*32+dout7*64+dout8*128;
	if (initialized && (old != Data)) 
	{
		pWriteAllDigital(Data);
		dout1 = (Data & 1)   && 1;
		dout2 = (Data & 2)   && 1;
		dout3 = (Data & 4)   && 1;
		dout4 = (Data & 8)   && 1;
		dout5 = (Data & 16)  && 1;
		dout6 = (Data & 32)  && 1;
		dout7 = (Data & 64)  && 1;
		dout8 = (Data & 128) && 1;
	}
}

void  k8055d::ClearDigitalChannel(long Channel)
{
	if (initialized) 
	{
		switch (Channel)
		{
		case 1:
			if (dout1) pClearDigitalChannel(Channel);
			dout1 = 0;
			break;
		case 2:
			if (dout2) pClearDigitalChannel(Channel);
			dout2 = 0;
			break;
		case 3:
			if (dout3) pClearDigitalChannel(Channel);
			dout3 = 0;
			break;
		case 4:
			if (dout4) pClearDigitalChannel(Channel);
			dout4 = 0;
			break;
		case 5:
			if (dout5) pClearDigitalChannel(Channel);
			dout5 = 0;
			break;
		case 6:
			if (dout6) pClearDigitalChannel(Channel);
			dout6 = 0;
			break;
		case 7:
			if (dout7) pClearDigitalChannel(Channel);
			dout7 = 0;
			break;
		case 8:
			if (dout8) pClearDigitalChannel(Channel);
			dout8 = 0;
			break;
		}
	}
}

void  k8055d::ClearAllDigital()
{
	if (initialized)
	{
		pClearAllDigital();
		dout1 = 0;
		dout2 = 0;
		dout3 = 0;
		dout4 = 0;
		dout5 = 0;
		dout6 = 0;
		dout7 = 0;
		dout8 = 0;
	}
}

void  k8055d::SetDigitalChannel(long Channel)
{
	if (initialized && ((Channel > 0) && (Channel <= 8)))
	{
		switch (Channel)
		{
		case 1:
			if (!dout1)	pSetDigitalChannel(Channel);
			dout1 = 1;
			break;
		case 2:
			if (!dout2)	pSetDigitalChannel(Channel);
			dout2 = 1;
			break;
		case 3:
			if (!dout3)	pSetDigitalChannel(Channel);
			dout3 = 1;
			break;
		case 4:
			if (!dout4)	pSetDigitalChannel(Channel);
			dout4 = 1;
			break;
		case 5:
			if (!dout5)	pSetDigitalChannel(Channel);
			dout5 = 1;
			break;
		case 6:
			if (!dout6)	pSetDigitalChannel(Channel);
			dout6 = 1;
			break;
		case 7:
			if (!dout7)	pSetDigitalChannel(Channel);
			dout7 = 1;
			break;
		case 8:
			if (!dout8)	pSetDigitalChannel(Channel);
			dout8 = 1;
			break;
		}
	}
}

void  k8055d::SetAllDigital()
{
	if (initialized) 
	{
		pSetAllDigital();
		dout1 = dout2 = dout3 = dout4 = dout5 = dout6 = dout7 = dout8 = true;
	}
}

bool  k8055d::ReadDigitalChannel(long Channel)
{	
	if (initialized && ((Channel > 0) && (Channel <= 5))) 
		switch (Channel)
	{
		case 1:
			return din1 = pReadDigitalChannel(Channel);
			break;
		case 2:
			return din2 = pReadDigitalChannel(Channel);
			break;
		case 3:
			return din3 = pReadDigitalChannel(Channel);
			break;
		case 4:
			return din4 = pReadDigitalChannel(Channel);
			break;
		case 5:
			return din5 = pReadDigitalChannel(Channel);
			break;
	}
	else return 0;
	return 0;
}

long  k8055d::ReadAllDigital()
{
	long ret = 0;
	if (initialized)
	{
		ret = pReadAllDigital();
		din1 = (ret & 1)  && 1;
		din1 = (ret & 2)  && 1;
		din1 = (ret & 4)  && 1;
		din1 = (ret & 8)  && 1;
		din1 = (ret & 16) && 1;
		return ret;
	}
	else return -1;
}

long  k8055d::ReadCounter(long CounterNr)
{
	if (initialized)
		if (CounterNr == 1)
			return ct1 = pReadCounter(CounterNr);
		else
			return ct2 = pReadCounter(CounterNr);
	else return -1;
}

void  k8055d::ResetCounter(long CounterNr)
{
	if (initialized && ((CounterNr == 1) || (CounterNr == 2)))
		pResetCounter(CounterNr);
	if (CounterNr == 1) 
		ct1 = 0;
	else
		ct2 = 0;
}
void  k8055d::SetCounterDebounceTime(long CounterNr, long DebounceTime)
{
	if (initialized && ((CounterNr == 1) || (CounterNr == 2)))
	{
    	pSetCounterDebounceTime(CounterNr, DebounceTime);
		if (CounterNr == 1)
			dbct1 = DebounceTime;
		else
			dbct2 = DebounceTime;
	}
}

void k8055d::exit()
{
	FreeLibrary(hDLL);
}
