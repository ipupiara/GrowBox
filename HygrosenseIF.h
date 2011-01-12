#include "stdafx.h"
#include "CSerial.h"



class CHygrosenseIF {
public:

	BOOL isDeviceRunning();

	CHygrosenseIF();
	virtual ~CHygrosenseIF();
	int start();
	void stop();
	void getMeasure(double& hum, double& temp, DWORD& cnt);

private:
	BOOL deviceRunning;
	void setDeviceRunning(BOOL runOK);

	class Mutex{
	public:
		Mutex(HANDLE* lpMutex);
		~Mutex();
		HANDLE mutex;
	};
	

	HANDLE hygroThread;  // Thread reading values from Sensor
	void incMeasure(double hum, double temp);
	BOOL getSensorValues();
	static HANDLE hygroStopEvent;  // event to communicate to the hygrothread
									// that it shall stop
	static HANDLE hygroMutex;		// thread concurrency synchronisation
	static double temperature;
	static double humidity;
	static DWORD measureCount;
	CSerial commPort;
	
	static long WINAPI CHygrosenseIF::hygroThreadMethod(void* pParam); 
};
