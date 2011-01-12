//
// Module Name:	StateClass.cpp
//
// Description:	Test class for statechart engine.  This class exemplifies
//				any class that would actually contain and use the engine.
//
// Copyright (c) 1994-2005 Grant D. Schultz.	All rights reserved.
//

#include "stdafx.h"

#include <iostream.h>
#include <assert.h>

#include "TStatechart.h"
#include "StateClass.h"
#include "GrowBoxDlg.h"

// This defines and names the states the class has.
enum eStates
{
	eStateGrowBoxKeepingHumidity,
	eStartState = eStateGrowBoxKeepingHumidity,
	eStateHumidifying,
	eStateIdle,
	eStateNonVentilating,
	eStateVentilating,
	eStateDrying,
	eNumberOfStates
};


TStatechart<CHumidityStateClass>::xStateType xaStates[eNumberOfStates] = {
/* name						*/	{eStateGrowBoxKeepingHumidity,
/* parent					*/	-1,
/* default_substate			*/	eStateIdle,
/* event-checking func		*/	CHumidityStateClass::evStateGrowBoxKeepingHumidity,
/* default state entry func	*/	NULL,  //CHumidityStateClass::defEntryStateGrowBoxKeepingHumidity,
/* entering state func		*/	CHumidityStateClass::entryStateGrowBoxKeepingHumidity,
/* exiting state func		*/	CHumidityStateClass::exitStateGrowBoxKeepingHumidity},

/* name						*/	{eStateHumidifying,
/* parent					*/	eStateGrowBoxKeepingHumidity,
/* default_substate			*/	-1,
/* event-checking func		*/	CHumidityStateClass::evStateHumidifying,
/* default state entry func	*/	NULL,
/* entering state func		*/	CHumidityStateClass::entryStateHumidifying,
/* exiting state func		*/	CHumidityStateClass::exitStateHumidifying},

/* name						*/	{eStateIdle,
/* parent					*/	eStateGrowBoxKeepingHumidity,
/* default_substate			*/	eStateNonVentilating,
/* event-checking func		*/	CHumidityStateClass::evStateIdle,
/* default state entry func	*/	NULL,
/* entering state func		*/	CHumidityStateClass::entryStateIdle,
/* exiting state func		*/	CHumidityStateClass::exitStateIdle},

/* name						*/	{eStateNonVentilating,
/* parent					*/	eStateIdle,
/* default_substate			*/	-1,
/* event-checking func		*/	CHumidityStateClass::evStateNonVentilating,
/* default state entry func	*/	NULL,
/* entering state func		*/	CHumidityStateClass::entryStateNonVentilating,
/* exiting state func		*/	CHumidityStateClass::exitStateNonVentilating},

/* name						*/	{eStateVentilating,
/* parent					*/	eStateIdle,
/* default_substate			*/	-1,
/* event-checking func		*/	CHumidityStateClass::evStateVentilating,
/* default state entry func	*/	NULL,
/* entering state func		*/	CHumidityStateClass::entryStateVentilating,
/* exiting state func		*/	CHumidityStateClass::exitStateVentilating},

/* name						*/	{eStateDrying,
/* parent					*/	eStateGrowBoxKeepingHumidity,
/* default_substate			*/	-1,
/* event-checking func		*/	CHumidityStateClass::evStateDrying,
/* default state entry func	*/	NULL,
/* entering state func		*/	CHumidityStateClass::entryStateDrying,
/* exiting state func		*/	CHumidityStateClass::exitStateDrying}
};


/*
CHumidityStateClass::CHumidityStateClass(void)
{
	clearFlags();

	CREATE_ENGINE(CHumidityStateClass, engine, xaStates, eNumberOfStates, eStartState);

	// Test initial state entry and defaulting.
	assertInitial();
}
*/

CHumidityStateClass::~CHumidityStateClass(void)
{
	DESTROY_ENGINE(CHumidityStateClass, engine);
}


CHumidityStateClass::CHumidityStateClass()
{
}

void CHumidityStateClass::setOwner(CGrowBoxController* own)
{
	owner = own;
}

void CHumidityStateClass::start()
{
	CREATE_ENGINE(CHumidityStateClass, engine, xaStates, eNumberOfStates, eStartState);
}


bool CHumidityStateClass::processEvent(CGrowBoxEvent& ev)
{
	event = ev;
	return (PROCESS_EVENT(CHumidityStateClass, engine));
}


bool CHumidityStateClass::inState(const uint32 u32State)
{
	return (IN_STATE(CHumidityStateClass, engine, u32State));
}


uint32 CHumidityStateClass::evStateGrowBoxKeepingHumidity(void)
{
//	TRACE0("check for event in State evStateGrowBoxKeepingHumidity\n");
	if (event.evType == eReset)
	{
		BEGIN_EVENT_HANDLER(CHumidityStateClass, engine, eStateGrowBoxKeepingHumidity );
			// No event action.
		END_EVENT_HANDLER(CHumidityStateClass, engine);
		
/*  left this as an original example for history states
 
		BEGIN_EVENT_HANDLER(CHumidityStateClass, engine, eStateGrowBoxKeepingHumidity | u32WithHistory);
			// No event action.
		END_EVENT_HANDLER(CHumidityStateClass, engine);
*/
		return (u32HandlingDone);
	}
	return (u32NoMatch);
}


uint32 CHumidityStateClass::evStateHumidifying(void)
{
//	TRACE0("check for event in State evStateHumidifying\n");
	if ((event.evType == eValueAssignement) && (CGrowBoxDefines::GetHumidifyingUpperLimit() < event.humidity))
	{
		BEGIN_EVENT_HANDLER(CHumidityStateClass, engine, eStateIdle);
			// No event action.
		END_EVENT_HANDLER(CHumidityStateClass, engine);
		return (u32HandlingDone);
	}
	return (u32NoMatch);
}


uint32 CHumidityStateClass::evStateIdle(void)
{
//	TRACE0("check for event in State evStateIdle\n");

	if (event.evType == eValueAssignement) 
	{	if (CGrowBoxDefines::GetHumidifyingLowerLimit() > event.humidity)
		{
			BEGIN_EVENT_HANDLER(CHumidityStateClass, engine, eStateHumidifying);
				// No event action.
			END_EVENT_HANDLER(CHumidityStateClass, engine);
			return (u32HandlingDone);
		}
		if (CGrowBoxDefines::GetDryingUpperLimit() < event.humidity)
		{
			BEGIN_EVENT_HANDLER(CHumidityStateClass, engine, eStateDrying);
				// No event action.
			END_EVENT_HANDLER(CHumidityStateClass, engine);
			return (u32HandlingDone);
		}
	}
	return (u32NoMatch);
}


uint32 CHumidityStateClass::evStateNonVentilating(void)
{
//	TRACE0("check for event in State evStateNonVentilating\n");

	if ((event.evType == eVentilationStartTimer) || (event.evType ==  eVentilationButtonPressed))
	{
		BEGIN_EVENT_HANDLER(CHumidityStateClass, engine, eStateVentilating);
			// No event action.
		END_EVENT_HANDLER(CHumidityStateClass, engine);
		return (u32HandlingDone);
	
	}
	return (u32NoMatch);
}


uint32 CHumidityStateClass::evStateVentilating(void)
{
//	TRACE0("check for event in State evStateVentilating\n");

	if ((event.evType == eVentilationStopTimer)  || (event.evType ==  eVentilationStopButtonPressed))
	{
		BEGIN_EVENT_HANDLER(CHumidityStateClass, engine, eStateNonVentilating);
			// No event action.
		END_EVENT_HANDLER(CHumidityStateClass, engine);
		return (u32HandlingDone);
	
	}
	return (u32NoMatch);
}

uint32 CHumidityStateClass::evStateDrying(void)
{
//	TRACE0("check for event in State evStateDrying\n");
	if ((event.evType == eValueAssignement) && (CGrowBoxDefines::GetDryingLowerLimit() > event.humidity))
	{
		BEGIN_EVENT_HANDLER(CHumidityStateClass, engine, eStateIdle);
			// No event action.
		END_EVENT_HANDLER(CHumidityStateClass, engine);
		return (u32HandlingDone);
	}
	return (u32NoMatch);
}

/*
// State transition/handling methods
// left as an example of this defEntry method

void CHumidityStateClass::defEntryStateGrowBoxKeepingHumidity(void)
{

//	TRACE0("CHumidityStateClass::defEntryStateGrowBoxKeepingHumidity\n");
}
*/



void CHumidityStateClass::entryStateGrowBoxKeepingHumidity(void)
{
	TRACE0("CHumidityStateClass::entryStateGrowBoxKeepingHumidity\n");
}


void CHumidityStateClass::entryStateHumidifying(void)
{
	TRACE0("CHumidityStateClass::entryStateHumidifying\n");
	owner->startHumidifying();
}


void CHumidityStateClass::entryStateIdle(void)
{
	TRACE0("CHumidityStateClass::entryStateIdle\n");
}


void CHumidityStateClass::entryStateNonVentilating(void)
{
	TRACE0("CHumidityStateClass::entryStateNonVentilating\n");
	owner->startVentilatingStartTimer(CGrowBoxDefines::GetIdleVentilationDelayMinutes());
	owner->onEntryIdleNotVentilating();
}


void CHumidityStateClass::entryStateVentilating(void)
{
	TRACE0("CHumidityStateClass::entryStateVentilating\n");
	owner->startVentilating();
	owner->startVentilatingStopTimer(CGrowBoxDefines::GetIdleVentilationMinutes());
	owner->onEntryIdleVentilating();
}

void CHumidityStateClass::entryStateDrying(void)
{
	TRACE0("CHumidityStateClass::entryStateDrying\n");
	owner->startDrying();
}

void CHumidityStateClass::exitStateGrowBoxKeepingHumidity(void)
{
	TRACE0("CHumidityStateClass::exitStateGrowBoxKeepingHumidity\n");
}


void CHumidityStateClass::exitStateHumidifying(void)
{
	TRACE0("CHumidityStateClass::exitStateHumidifying\n");
	owner->stopHumidifying();
}


void CHumidityStateClass::exitStateIdle(void)
{
	TRACE0("CHumidityStateClass::exitStateIdle\n");
}


void CHumidityStateClass::exitStateNonVentilating(void)
{
	TRACE0("CHumidityStateClass::exitStateNonVentilating\n");
	owner->stopVentilatingStartTimer();
	owner->onExitIdleNotVentilating();
}


void CHumidityStateClass::exitStateVentilating(void)
{
	TRACE0("CHumidityStateClass::exitStateVentilating\n");
	owner->stopVentilating();
	owner->stopVentilatingStopTimer();
	owner->onExitIdleVentilating();
}


void CHumidityStateClass::exitStateDrying(void)
{
	TRACE0("CHumidityStateClass::exitStateDrying\n");
	owner->stopDrying();
}




/***   Temperature  ********/

// This defines and names the states the class has.
enum eTemperatureStates
{
	eStateGrowBoxTemperature,
	eTemperatureStartState = eStateGrowBoxTemperature,
	eStateTemperatureIdle,
	eStateHeating,
	eStateCooling,
	eNumberOfTemperatureStates
};


TStatechart<CTemperatureStateClass>::xStateType xTemperatureStates[eNumberOfTemperatureStates] = {
/* name						*/	{eStateGrowBoxTemperature,
/* parent					*/	-1,
/* default_substate			*/	eStateTemperatureIdle,
/* event-checking func		*/	CTemperatureStateClass::evStateGrowBoxTemperature,
/* default state entry func	*/	NULL,
/* entering state func		*/	CTemperatureStateClass::entryStateGrowBoxTemperature,
/* exiting state func		*/	CTemperatureStateClass::exitStateGrowBoxTemperature},

/* name						*/	{eStateTemperatureIdle,
/* parent					*/	eStateGrowBoxTemperature,
/* default_substate			*/	-1,
/* event-checking func		*/	CTemperatureStateClass::evStateTemperatureIdle,
/* default state entry func	*/	NULL,
/* entering state func		*/	CTemperatureStateClass::entryStateTemperatureIdle,
/* exiting state func		*/	CTemperatureStateClass::exitStateTemperatureIdle},

/* name						*/	{eStateHeating,
/* parent					*/	eStateGrowBoxTemperature,
/* default_substate			*/	-1,
/* event-checking func		*/	CTemperatureStateClass::evStateHeating,
/* default state entry func	*/	NULL,
/* entering state func		*/	CTemperatureStateClass::entryStateHeating,
/* exiting state func		*/	CTemperatureStateClass::exitStateHeating},

/* name						*/	{eStateCooling,
/* parent					*/	eStateGrowBoxTemperature,
/* default_substate			*/	-1,
/* event-checking func		*/	CTemperatureStateClass::evStateCooling,
/* default state entry func	*/	NULL,
/* entering state func		*/	CTemperatureStateClass::entryStateCooling,
/* exiting state func		*/	CTemperatureStateClass::exitStateCooling},

};


/*
CTemperatureStateClass::CTemperatureStateClass(void)
{
	clearFlags();

	CREATE_ENGINE(CHumidityStateClass, engine, xaStates, eNumberOfStates, eStartState);

	// Test initial state entry and defaulting.
	assertInitial();
}
*/

CTemperatureStateClass::~CTemperatureStateClass(void)
{
	DESTROY_ENGINE(CTemperatureStateClass, engine);
}


CTemperatureStateClass::CTemperatureStateClass()
{
}

void CTemperatureStateClass::setOwner(CGrowBoxController* own)
{
	owner = own;
}

void CTemperatureStateClass::start()
{
	CREATE_ENGINE(CTemperatureStateClass, engine, xTemperatureStates, eNumberOfTemperatureStates, eTemperatureStartState);
}


bool CTemperatureStateClass::processEvent(CGrowBoxEvent& ev)
{
	event = ev;
	return (PROCESS_EVENT(CTemperatureStateClass, engine));
}


bool CTemperatureStateClass::inState(const uint32 u32State)
{
	return (IN_STATE(CTemperatureStateClass, engine, u32State));
}


uint32 CTemperatureStateClass::evStateGrowBoxTemperature(void)
{
//	TRACE0("check for event in State evStateGrowBoxKeepingHumidity\n");
	if (event.evType == eReset)
	{
		BEGIN_EVENT_HANDLER(CTemperatureStateClass, engine, eStateGrowBoxTemperature );
			// No event action.
		END_EVENT_HANDLER(CTemperatureStateClass, engine);
		
/*  left this as an original example for history states
 
		BEGIN_EVENT_HANDLER(CTemperatureStateClass, engine, eStateGrowBoxKeepingHumidity | u32WithHistory);
			// No event action.
		END_EVENT_HANDLER(CTemperatureStateClass, engine);
*/
		return (u32HandlingDone);
	}
	return (u32NoMatch);
}


uint32 CTemperatureStateClass::evStateTemperatureIdle(void)
{
//	TRACE0("check for event in State evStateHumidifying\n");
	if ((event.evType == eValueAssignement) && (CGrowBoxDefines::GetHeatingLowerLimit() > event.temperature))
	{
		BEGIN_EVENT_HANDLER(CTemperatureStateClass, engine, eStateHeating);
			// No event action.
		END_EVENT_HANDLER(CTemperatureStateClass, engine);
		return (u32HandlingDone);
	}
	if ((event.evType == eValueAssignement) && (CGrowBoxDefines::GetCoolingUpperLimit() < event.temperature))
	{
		BEGIN_EVENT_HANDLER(CTemperatureStateClass, engine, eStateCooling);
			// No event action.
		END_EVENT_HANDLER(CTemperatureStateClass, engine);
		return (u32HandlingDone);
	}
	return (u32NoMatch);
}


uint32 CTemperatureStateClass::evStateHeating(void)
{
//	TRACE0("check for event in State evStateIdle\n");

	if (event.evType == eValueAssignement) 
	{	if (CGrowBoxDefines::GetHeatingUpperLimit() < event.temperature)
		{
			BEGIN_EVENT_HANDLER(CTemperatureStateClass, engine, eStateTemperatureIdle);
				// No event action.
			END_EVENT_HANDLER(CTemperatureStateClass, engine);
			return (u32HandlingDone);
		}
	}
	return (u32NoMatch);
}

uint32 CTemperatureStateClass::evStateCooling(void)
{
//	TRACE0("check for event in State evStateIdle\n");

	if (event.evType == eValueAssignement) 
	{	if (CGrowBoxDefines::GetCoolingLowerLimit() > event.temperature)
		{
			BEGIN_EVENT_HANDLER(CTemperatureStateClass, engine, eStateTemperatureIdle);
				// No event action.
			END_EVENT_HANDLER(CTemperatureStateClass, engine);
			return (u32HandlingDone);
		}
	}
	return (u32NoMatch);
}


void CTemperatureStateClass::entryStateGrowBoxTemperature(void)
{
	TRACE0("CHumidityStateClass::entryStateGrowTemperature\n");
}


void CTemperatureStateClass::entryStateTemperatureIdle(void)
{
	TRACE0("CHumidityStateClass::entryStateTemperatureIdle\n");
}


void CTemperatureStateClass::entryStateHeating(void)
{
	TRACE0("CHumidityStateClass::entryStateHeating\n");
	owner->startHeating();
}

void CTemperatureStateClass::entryStateCooling(void)
{
	TRACE0("CHumidityStateClass::entryStateCooling\n");
	owner->startCooling();
}


void CTemperatureStateClass::exitStateGrowBoxTemperature(void)
{
	TRACE0("CHumidityStateClass::exitStateGrowBoxTemperature\n");
}


void CTemperatureStateClass::exitStateTemperatureIdle(void)
{
	TRACE0("CHumidityStateClass::exitStateTemperatureIdle\n");
}


void CTemperatureStateClass::exitStateHeating(void)
{
	TRACE0("CHumidityStateClass::exitStateHeating\n");
	owner->stopHeating();
}

void CTemperatureStateClass::exitStateCooling(void)
{
	TRACE0("CHumidityStateClass::exitStateCooling\n");
	owner->stopCooling();
}


/**** end Temperature *****/


const CGrowBoxEvent& CGrowBoxEvent::operator =( const CGrowBoxEvent& src )
{
	evType = src.evType;
	temperature = src.temperature;
	humidity = src.humidity;

	return *this;
}

CGrowBoxEvent::CGrowBoxEvent()
{
}

CGrowBoxEvent::~CGrowBoxEvent()
{
}


CGrowBoxController::~CGrowBoxController(void)
{
	delete relais;
	DB_IF.exitDBInterface() ;
}


CGrowBoxController::CGrowBoxController()
{
	if (CGrowBoxDefines::useUSBBoard())  {
		relais = new USBRelaisController();
	} else {
		relais = new ParallelRelaisController();
	}

	growBoxHumidityChart.setOwner(this);
	growBoxTemperatureChart.setOwner(this);
	DB_IF.initDBInterface();

}


void CGrowBoxController::getMeasure(double& hum, double& temp, DWORD& cnt)
{
	HygroIF.getMeasure(hum, temp, cnt);
}



void CGrowBoxController::OnVentilationStartTimer()
{
	CGrowBoxEvent ev;
	ev.evType = eVentilationStartTimer;
	growBoxHumidityChart.processEvent(ev);
}

void CGrowBoxController::OnVentilationStopTimer()
{
	CGrowBoxEvent ev;
	ev.evType = eVentilationStopTimer;
	growBoxHumidityChart.processEvent(ev);
}

void CGrowBoxController::OnCheckValuesTimer()
{
	double temp;
	double humid;
	DWORD cnt;
	HygroIF.getMeasure(humid,temp,cnt);

	if (cnt > 0) {
		CGrowBoxEvent ev;
		ev.evType = eValueAssignement;
		ev.humidity = humid;
		ev.temperature = temp;

		growBoxHumidityChart.processEvent(ev);
		growBoxTemperatureChart.processEvent(ev);

		float tf = (float) temp;
		float hf = (float) humid;

		CString state = CString("t: ")+ HeaterStateString()+ CString(" h: ")+  \
						HumidifyerStateString();

		soapServer.publishValues(cnt,tf,hf,state);
	}
	relais->onSecondTimer();
}

void CGrowBoxController::onEntryIdleNotVentilating()
{

	owner->onEntryIdleNotVentilating();
}

void CGrowBoxController::onExitIdleNotVentilating()
{
	owner->onExitIdleNotVentilating();
}

void CGrowBoxController::onEntryIdleVentilating()
{

	owner->onEntryIdleVentilating();
}

void CGrowBoxController::onExitIdleVentilating()
{
	owner->onExitIdleVentilating();
}



BOOL CGrowBoxController::isRelaisSet( INT pos)
{
	return relais->isRelaisSet(pos);
}


void CGrowBoxController::startHeating()
{
	relais->startHeating();
}

void CGrowBoxController::stopHeating()
{
	relais->stopHeating();
}

void CGrowBoxController::startHumidifying()
{
	relais->startHumidifying();
}

void CGrowBoxController::stopHumidifying()
{
	relais->stopHumidifying();
}

void CGrowBoxController::startVentilating()
{
	relais->startVentilating();
}

void CGrowBoxController::stopVentilating()
{
	relais->stopVentilating();
}


void CGrowBoxController::Reset()
{
	CGrowBoxEvent ev;
	ev.evType = eReset;
	growBoxHumidityChart.processEvent(ev);
	growBoxTemperatureChart.processEvent(ev);
}

void CGrowBoxController::stop()
{
	HygroIF.stop();
}

void CGrowBoxController::start()
{
	growBoxHumidityChart.start();
	growBoxTemperatureChart.start();
	soapServer.startServer();
}


void CGrowBoxController::setOwner(CGrowBoxDlg* own)
{
	owner = own;
}

void CGrowBoxController::startVentilatingStopTimer(double minutes)
{
	owner->startVentilatingStopTimer(minutes );
}

void CGrowBoxController::stopVentilatingStopTimer()
{
	owner->stopVentilatingStopTimer();
}

void CGrowBoxController::startVentilatingStartTimer(double minutes)
{
	owner->startVentilatingStartTimer(minutes );
}

void CGrowBoxController::stopVentilatingStartTimer()
{
	owner->stopVentilatingStartTimer();
}


CString CTemperatureStateClass::HeaterStateString()
{
	if (inState(eStateTemperatureIdle)) return CString("TemperatureIdle");
	if (inState(eStateHeating)) return CString("Heating");
	if (inState(eStateCooling)) return CString("Cooling");

	return CString("application error");
}


CString CHumidityStateClass::HumidifyerStateString()
{
	if (inState(eStateHumidifying)) return CString("Humidifying");
	if (inState(eStateNonVentilating)) return CString("Idle / Not Ventilating");
	if (inState(eStateVentilating)) return CString("Idle / Ventilating");
	if (inState(eStateDrying)) return CString("eStateDrying");

	return CString("application error");
}

CString CGrowBoxController::HeaterStateString()
{
	return growBoxTemperatureChart.HeaterStateString();
}

CString CGrowBoxController::HumidifyerStateString()
{
	return growBoxHumidityChart.HumidifyerStateString();
}


void CGrowBoxController::startDrying()
{
	relais->startDrying();
}

void CGrowBoxController::stopDrying()
{
	relais->stopDrying();
}

void CGrowBoxController::startCooling()
{
	relais->startCooling();
}

void CGrowBoxController::stopCooling()
{
	relais->stopCooling();
}

BOOL CGrowBoxController::isHygrosensSensorRunning()
{
	return HygroIF.isDeviceRunning();
}

BOOL CGrowBoxController::isRelaisControllerRunning()
{
	return relais->isRelaisControllerRunning();
}

void CGrowBoxController::OnDBUpdateTimer()
{
	double temp;
	double humid;
	DWORD cnt;
	HygroIF.getMeasure(humid,temp,cnt);

	if (cnt > 0) {
		DB_IF.logValues(temp,humid);	
	}
}

bool CGrowBoxController::isDBInterfaceOn()
{
	return DB_IF.isInterfaceOn();
}

void CGrowBoxController::onVentilationButtonPressed()
{
	CGrowBoxEvent ev;
	ev.evType = eVentilationButtonPressed;
	growBoxHumidityChart.processEvent(ev);
}

void CGrowBoxController::onStopVentilationButtonPressed()
{
	CGrowBoxEvent ev;
	ev.evType = eVentilationStopButtonPressed;
	growBoxHumidityChart.processEvent(ev);
}
