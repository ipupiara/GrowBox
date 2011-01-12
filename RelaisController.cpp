#include "stdafx.h"
#include "RelaisController.h"
#include "USBDLL.h"


// set bit at pos to 1, all other bits to 0
//
BYTE RelaisController::bitMaskOnAt(INT pos )
{
	BYTE v = 0x01;
	v = v << pos;
	return v;
}

// set the bit at pos to 0, all other to 1
//
BYTE RelaisController::bitMaskOffAt(int pos)
{
	return (bitMaskOnAt(pos) ^ 0xFF );
}


RelaisController::~RelaisController()
{
	TRACE0("\ndesstructur RelaisController");
}


RelaisController::RelaisController()
{
	TRACE0("\nconstructur RelaisController");
	setOffAllRelais();
}

BOOL RelaisController::isRelaisSet( INT pos)
{
	BYTE cmp = 0;
	BYTE bits = getRelaisByte();
	cmp = bits & bitMaskOnAt(pos);
	return (cmp != 0);
}


void RelaisController::startHeating()
{
	setOnRelaisPos(CGrowBoxDefines::GetHeatingRelais());
}

void RelaisController::stopHeating()
{
	setOffRelaisPos(CGrowBoxDefines::GetHeatingRelais());
}




void RelaisController::startHumidifying()
{
	setOnRelaisPos(CGrowBoxDefines::GetHumidifyingRelais());
}

void RelaisController::stopHumidifying()
{
	setOffRelaisPos(CGrowBoxDefines::GetHumidifyingRelais());
}

void RelaisController::startVentilating()
{
	setOnRelaisPos(CGrowBoxDefines::GetVentilatingRelais());
}

void RelaisController::stopVentilating()
{
	setOffRelaisPos(CGrowBoxDefines::GetVentilatingRelais());
}


void RelaisController::startCooling()
{
	setOnRelaisPos(CGrowBoxDefines::GetCoolingRelais());
}

void RelaisController::stopCooling()
{
	setOffRelaisPos(CGrowBoxDefines::GetCoolingRelais());
}


void RelaisController::startDrying()
{
	setOnRelaisPos(CGrowBoxDefines::GetDryingRelais());
}

void RelaisController::stopDrying()
{
	setOffRelaisPos(CGrowBoxDefines::GetDryingRelais());
}


void RelaisController::setOnRelaisPos(int pos)
{
	BYTE bits = getRelaisByte();
	bits = bits | bitMaskOnAt(pos) ;	
	setRelaisByte(bits);
}

void RelaisController::setOffRelaisPos(int pos)
{
	BYTE bits = getRelaisByte();
	bits = bits & bitMaskOffAt(pos)   ;	
	setRelaisByte(bits);
}

void RelaisController::setOffAllRelais()
{
	BYTE bits =0x00 ;	
	setRelaisByte(bits);
}

void RelaisController::setRelais(BOOL on, int pos)
{
	if (on) {
		setOnRelaisPos(pos);
	} else
	{
		setOffRelaisPos(pos);
	}
}



BYTE RelaisController::getRelaisByte()
{
	return 0;
}

void RelaisController::setRelaisByte(BYTE bits)
{
}

void RelaisController::onSecondTimer()
{
}


ParallelRelaisController::~ParallelRelaisController()
{
	TRACE0("\ndestructur parallelRelaisController");
	setOffAllRelais();
}


ParallelRelaisController::ParallelRelaisController() 
{
	TRACE0("\nconstructur parallelRelaisController");
}

BYTE ParallelRelaisController::getRelaisByte()
{
	return pp32.Inp32(lptPortNumber);
}

void ParallelRelaisController::setRelaisByte(BYTE bits)
{
	pp32.Out32(lptPortNumber,bits);
}



USBRelaisController::~USBRelaisController()
{
	setOffAllRelais();
	TRACE0("\ndestructur USBRelaisController\n");
}


USBRelaisController::USBRelaisController() 
{
	m_bits = 0;
	writeByte();
	TRACE0("\nconstructur USBRelaisController\n");
}

BYTE USBRelaisController::getRelaisByte()
{
	return m_bits;
}

void USBRelaisController::setRelaisByte(BYTE bits)
{
	m_bits = bits;
	writeByte();
}

void USBRelaisController::onSecondTimer()
{
	writeByte();
}

void USBRelaisController::writeByte()
{
	int devErr;
	if ( (devErr = USBBoard.GetDeviceError()) != Err_No)  {
		TRACE1(" USBRelaisController::onSecondTimer GetDeviceError %X\n",devErr);
		USBBoard.closeDevice();
		USBBoard.openDevice();
	}
	if ( (devErr = USBBoard.GetDeviceError()) == Err_No)  {
		USBBoard.readByte(m_bits);
	}
}

BOOL RelaisController::isRelaisControllerRunning()
{
	return FALSE;
}

BOOL USBRelaisController::isRelaisControllerRunning()
{
	return (USBBoard.GetDeviceError() == Err_No);
}

BOOL ParallelRelaisController::isRelaisControllerRunning()
{
	return pp32.isRunningOK();
}
