//
// Module Name:	StateClass.hpp
//
// Description:	class for statechart engine.  This class represents
//				any class that would actually contain and use the engine.
//


#if !defined(CCtateClassHeader)
#define CCtateClassHeader


#include "stdafx.h"
#include "stdtype.h"

#include "RelaisController.h"
#include "HygrosenseIF.h"
#include "GrowBoxDefines.h"
#include "DBInterface.h"
#include "CoolingController.h"

class CGrowBoxDlg;
class CGrowBoxController;

#include "../commonSoap/GrowBoxSoapServer.h"



enum eEventTypes
{
	eReset,
	eValueAssignement,
	eVentilationStartTimer,
	eVentilationStopTimer,
	eVentilationButtonPressed,
	eVentilationStopButtonPressed
};


// class to held event data for the statechart
class CGrowBoxEvent {
public:
	CGrowBoxEvent();
	virtual ~CGrowBoxEvent();
	int evType;
	double temperature;
	double humidity;
	
	const CGrowBoxEvent& operator=(const CGrowBoxEvent& src);
};

// statechart implementation, see RationalRoseDescription.pdf
// and codeproject.com/samples/statechart.asp
class CHumidityStateClass
{
	public:
		CString HumidifyerStateString();
		CHumidityStateClass();
//		CStateClass();
		~CHumidityStateClass();

		void setOwner(CGrowBoxController* own);
		void start();

		bool	processEvent(CGrowBoxEvent& ev);


		// State transition/handling methods
//		void	defEntryStateGrowBoxKeepingHumidity();

		void	entryStateGrowBoxKeepingHumidity();
		void	entryStateHumidifying();
		void	entryStateIdle();
		void	entryStateNonVentilating();
		void	entryStateVentilating();
		void	entryStateDrying();

		void	exitStateGrowBoxKeepingHumidity();
		void	exitStateHumidifying();
		void	exitStateIdle();
		void	exitStateNonVentilating();
		void	exitStateVentilating();
		void	exitStateDrying();

		uint32  evStateGrowBoxKeepingHumidity();
		uint32  evStateHumidifying();
		uint32  evStateIdle();
		uint32  evStateNonVentilating();
		uint32  evStateVentilating();
		uint32  evStateDrying();


	private:
		bool	inState(const uint32 u32State);
		void	*engine;
		CGrowBoxEvent event;
		CGrowBoxController* owner;


};




// statechart implementation for temperature Control
// 
class CTemperatureStateClass
{
	public:
		CString HeaterStateString();
		CTemperatureStateClass();
//		CStateClass();
		~CTemperatureStateClass();

		void setOwner(CGrowBoxController* own);
		void start();

		bool	processEvent(CGrowBoxEvent& ev);

		// State transition/handling methods

		void	entryStateGrowBoxTemperature();
		void	entryStateHeating();
		void    entryStateCooling();
		void	entryStateTemperatureIdle();

		void	exitStateGrowBoxTemperature();
		void	exitStateHeating();
		void    exitStateCooling();
		void	exitStateTemperatureIdle();

		uint32  evStateGrowBoxTemperature();
		uint32  evStateHeating();
		uint32  evStateCooling();
		uint32  evStateTemperatureIdle();

	private:
		bool	inState(const uint32 u32State);
		void	*engine;
		CGrowBoxEvent event;
		CGrowBoxController* owner;
};


// class to aggregate all the different helper objects
// needed to implement the growbox control
class CGrowBoxController
{
public:
	void onVentilationButtonPressed();
	void onStopVentilationButtonPressed();
	void onExitIdleNotVentilating();
	void onEntryIdleNotVentilating();
	void onExitIdleVentilating();
	void onEntryIdleVentilating();
	bool isDBInterfaceOn();
	void OnDBUpdateTimer();
	// creation, intialization and stop/start methods
	CGrowBoxController();
	~CGrowBoxController();
	void setOwner(CGrowBoxDlg* own);
	void stop();
	void start();

	// interface for Dialog to get values
	void getMeasure(double& hum, double& temp, DWORD& cnt);
	BOOL isRelaisSet( INT pos);
	CString HumidifyerStateString();  // currently not used
	CString HeaterStateString();      // currently not used
	BOOL isRelaisControllerRunning();
	BOOL isHygrosensSensorRunning();

	// interface for OS to notify timer events
	void OnVentilationStartTimer();
	void OnVentilationStopTimer();
	void OnCheckValuesTimer();
	void Reset(); // resets state charts, needs to be done, when behaviour values
					// should be changeable during application run

	//  interface for the statechart to request Timers and Relais Actions
	void stopVentilatingStopTimer();
	void startVentilatingStopTimer(double minutes);
	void stopVentilatingStartTimer();
	void startVentilatingStartTimer(double minutes);
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

private:
	RelaisController* relais;
	CHygrosenseIF HygroIF;
	CHumidityStateClass growBoxHumidityChart;
	CTemperatureStateClass growBoxTemperatureChart;
	CGrowBoxDlg* owner;
	GrowBoxSoapServer soapServer;
	DBInterface DB_IF;
	CoolingController cooler;
};


#endif
