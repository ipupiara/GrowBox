#include "stdafx.h"
#include "HygrosenseIF.h"
#include "GrowBoxDefines.h"


HANDLE CHygrosenseIF::hygroStopEvent;
HANDLE CHygrosenseIF::hygroMutex;

double CHygrosenseIF::temperature;
double CHygrosenseIF::humidity;
DWORD CHygrosenseIF::measureCount;



//  PN 8. Nov 2005
//
//  standard way of method access synchronisation
//
//  let the constructor/destructor access the semaphore in the
//   frame of a method
//
//  do not remove the method local Mutex m variables, even they seem 
//  not explicitely be used.
//

CHygrosenseIF::Mutex::Mutex(HANDLE* lpMutex)
{
	mutex = * lpMutex;
	WaitForSingleObject(mutex,INFINITE);
}

CHygrosenseIF::Mutex::~Mutex()
{
	SetEvent(mutex);
}
	



CHygrosenseIF::CHygrosenseIF() 
{
	hygroStopEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	hygroMutex = CreateEvent(NULL,FALSE,TRUE,NULL);
	
	TRACE0("\nconstuctor CHygrosenseIF  %i\n");
	
	if (! start())  {
		MessageBox(NULL,"error on HygroIF start","error", MB_OK);
	}
}


CHygrosenseIF::~CHygrosenseIF()
{
	TRACE0("\ndestructor CHygrosenseIF\n");
}


int CHygrosenseIF::start()
{
	TRACE0("\nstart CHygrosenseIF\n");
	commPort.Open_port();

	hygroThread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)hygroThreadMethod,this,0,NULL);

	return 1;
}

void CHygrosenseIF::stop()
{
	DWORD tmOut;
	TRACE0("\nstop CHygrosenseIF\n");
	commPort.Close_port();
	SetEvent(hygroStopEvent);
	tmOut = GBSerialReadTotalTimeoutConstant  \
			+ GBReadIntervalTimeout  \
			+ 2000;  //  max calculation time

	WaitForSingleObject(hygroThread,tmOut);
}



long WINAPI CHygrosenseIF::hygroThreadMethod(void* pParam)
{
	int step = 0;

	
	CHygrosenseIF* HSIF = (CHygrosenseIF*) pParam;
	TRACE0("\nHYGROTHREAD::hygro Thread Started\n");

	while (WaitForSingleObject(hygroStopEvent,100) != WAIT_OBJECT_0) {
		++ step;
//		TRACE1("HYGROTHREAD::hygro Thrad step %i\n",step);

		if (! HSIF->getSensorValues()) {
			TRACE0("\nHYGROTHREAD::hygro Thread error getting values");
		}
	}
	TRACE0("\nHYGROTHREAD::hygro Thrad Returning\n");
	return 0;
}


void CHygrosenseIF::incMeasure(double hum, double temp)
{
	Mutex m (&hygroMutex);

	measureCount ++;
	temperature = temp;
	humidity = hum;
	deviceRunning = TRUE;
}

void CHygrosenseIF::getMeasure(double& hum, double& temp, DWORD& cnt)
{
	Mutex m (&hygroMutex);

	cnt = measureCount;
	temp = temperature;
	hum = humidity;
}


BOOL CHygrosenseIF::getSensorValues()
{
	char buffer [300];
	DWORD amtRcv;

	memset(&buffer, 0, sizeof(buffer));

	commPort.Read_port(&buffer, sizeof(buffer), &amtRcv);

	if (amtRcv == 68) {

		char* tempS;
		char* hydS;

		tempS = strstr(buffer,"V01")+3;
		hydS = strstr(buffer,"V02")+3;

		*(hydS+4) = 0;
		*(tempS+4) = 0;

		double temp = strtoul(tempS ,&tempS+4,0x10) ;
		temp = temp / 100;
		double hyd = strtoul (hydS, &hydS + 4, 0x10);
		hyd = hyd / 200;

//		TRACE2("\nreceived: %i  %s\n",amtRcv, buffer);

//		TRACE2("\nV01: %s %f\n",tempS, temp);
//		TRACE2("\nV02: %s %f\n",hydS, hyd);
	
		incMeasure(hyd,temp);

	} else
	{
		setDeviceRunning(FALSE);
		TRACE1("CHygro IF wrong amount of bytes: %i\n",amtRcv);
	}
	return  TRUE;
}

BOOL CHygrosenseIF::isDeviceRunning()
{
	Mutex m (&hygroMutex);
	return deviceRunning;
}

void CHygrosenseIF::setDeviceRunning(BOOL runOK)
{
	Mutex m (&hygroMutex);
	deviceRunning = runOK;
}
