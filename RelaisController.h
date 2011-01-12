
#include "stdafx.h"
#include "GrowBoxDefines.h"
#include "PrintPort32.h"
#include "USBDLL.H"


// PN 7. Nov 2005
//
// RelaisController implements a baseclass for the USB and Parallel Relais Controller
//
//
// The Parallel controller keeps its relais states at the Parallel Port. Data is written 
// to the Port, whenever they change and read from the port, whenever needed. It does not
// use the timer.
//
// The USB Port keeps its state in a variable because the relais state can not be queried
// from the device.
// It writes out the variable, whenever
// the value changes and on each timerEvent (each second), because of the watchdog
// mechanism on the device
//
//
//


class RelaisController
{
public:

	// methods for UI to get relais states
	BOOL isRelaisSet( INT pos);
	virtual BOOL isRelaisControllerRunning();

	// action methods used by the statecharts
	void stopVentilating();
	void startVentilating();
	void stopHumidifying();
	void startHumidifying();
	void stopHeating();
	void startHeating();
	void stopCooling();
	void startCooling();
	void stopDrying();
	void startDrying();

	// methods for os to communicate timer event each second
	virtual void onSecondTimer();

public:

	RelaisController();	// standard constructor
	virtual ~RelaisController();

protected:
	void setOffAllRelais();
private:
	void setRelais(BOOL on, int pos);
	void setOffRelaisPos(int pos);
	void setOnRelaisPos(int pos);

	BYTE bitMaskOnAt(INT pos );
	BYTE bitMaskOffAt(int pos);

private:
	virtual BYTE getRelaisByte();
	virtual void setRelaisByte(BYTE bits);
};

class ParallelRelaisController:    public RelaisController
{
public:
	ParallelRelaisController();	// standard constructor
	virtual ~ParallelRelaisController();

	virtual BOOL isRelaisControllerRunning();
private:
	PrintPort32 pp32;
	virtual BYTE getRelaisByte();
	virtual void setRelaisByte(BYTE bits);
};


class USBRelaisController:    public RelaisController
{

public:
	USBRelaisController();	// standard constructor
	virtual ~USBRelaisController();

	virtual void onSecondTimer();

	virtual BOOL isRelaisControllerRunning();
private:
	BYTE m_bits;
	virtual BYTE getRelaisByte();
	virtual void setRelaisByte(BYTE bits);
	void writeByte();
	USBBoardIF USBBoard;
};