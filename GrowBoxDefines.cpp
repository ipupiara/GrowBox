
#include "stdafx.h"
#include "GrowBoxDefines.h"
//#include "IniFile.h"


double  CGrowBoxDefines::IdleVentilationDelayMinutes = DefaultIdleVentilationDelayMinutes;    
double  CGrowBoxDefines::IdleVentilationMinutes = DefaultIdleVentilationMinutes;    

double  CGrowBoxDefines::HumidifyingLowerLimit = DefaultHumidifyingLowerLimit;   
double  CGrowBoxDefines::HumidifyingUpperLimit = DefaultHumidifyingUpperLimit ;  
double  CGrowBoxDefines::DryingUpperLimit = DefaultDryingUpperLimit ;	
double  CGrowBoxDefines::DryingLowerLimit = DefaultDryingLowerLimit;	


double  CGrowBoxDefines::HeatingLowerLimit = DefaultHeatingLowerLimit;	
double  CGrowBoxDefines::HeatingUpperLimit = DefaultHeatingUpperLimit;	
double  CGrowBoxDefines::CoolingLowerLimit = DefaultCoolingLowerLimit;	
double  CGrowBoxDefines::CoolingUpperLimit = DefaultCoolingUpperLimit;	

int CGrowBoxDefines::UseUSBBoard = 1;

int CGrowBoxDefines::VentilatingRelais = DefaultVentilatingRelais;
int CGrowBoxDefines::HumidifyingRelais = DefaultHumidifyingRelais;
int CGrowBoxDefines::DryingRelais= DefaultDryingRelais;
int CGrowBoxDefines::HeatingRelais = DefaultHeatingRelais;
int CGrowBoxDefines::CoolingRelais = DefaultCoolingRelais;


BOOL CGrowBoxDefines::initialized = FALSE;

char CGrowBoxDefines::iniFilePath [3000];


void  CGrowBoxDefines::checkInitialization()
{
	if (initialized) return;
	setIniFilePath();

	IdleVentilationDelayMinutes = GetIniValue("IdleVentilationDelayMinutes",IdleVentilationDelayMinutes);
	IdleVentilationMinutes = GetIniValue("IdleVentilationMinutes",IdleVentilationMinutes);

	HumidifyingLowerLimit = GetIniValue("HumidifyingLowerLimit",HumidifyingLowerLimit);
	HumidifyingUpperLimit = GetIniValue("HumidifyingUpperLimit",HumidifyingUpperLimit);
	DryingLowerLimit = GetIniValue("DryingLowerLimit",DryingLowerLimit);
	DryingUpperLimit = GetIniValue("DryingUpperLimit",DryingUpperLimit);

	HeatingLowerLimit = GetIniValue("HeatingLowerLimit",HeatingLowerLimit);
	HeatingUpperLimit = GetIniValue("HeatingUpperLimit",HeatingUpperLimit);
	CoolingLowerLimit = GetIniValue("CoolingLowerLimit",CoolingLowerLimit);
	CoolingUpperLimit = GetIniValue("CoolingUpperLimit",CoolingUpperLimit);

	UseUSBBoard = GetIniInt("UseUSBBoard",1);

	VentilatingRelais = GetIniInt("VentilatingRelais",DefaultVentilatingRelais);
	HumidifyingRelais = GetIniInt("HumidifyingRelais",DefaultHumidifyingRelais);
	DryingRelais = GetIniInt("DryingRelais", DefaultDryingRelais);
	HeatingRelais = GetIniInt("HeatingRelais",DefaultHeatingRelais);
	CoolingRelais = GetIniInt("CoolingRelais",DefaultCoolingRelais);

	initialized = TRUE;
	return;	
}


double  CGrowBoxDefines::GetIdleVentilationDelayMinutes()
{
	checkInitialization();
	return IdleVentilationDelayMinutes;
}

double  CGrowBoxDefines::GetIdleVentilationMinutes()   
{
	checkInitialization();
	return IdleVentilationMinutes;
}

double  CGrowBoxDefines::GetHumidifyingLowerLimit()  
{
	checkInitialization();
	return HumidifyingLowerLimit;
}

double  CGrowBoxDefines::GetHumidifyingUpperLimit() 
{
	checkInitialization();
	return HumidifyingUpperLimit;
}

double  CGrowBoxDefines::GetDryingUpperLimit()
{
	checkInitialization();
	return DryingUpperLimit;
}
double  CGrowBoxDefines::GetDryingLowerLimit()	
{
	checkInitialization();
	return DryingLowerLimit;
}

double  CGrowBoxDefines::GetHeatingLowerLimit()
{
	checkInitialization();
	return HeatingLowerLimit;
}

double  CGrowBoxDefines::GetHeatingUpperLimit()
{
	checkInitialization();
	return HeatingUpperLimit;
}

double  CGrowBoxDefines::GetCoolingLowerLimit()
{
	checkInitialization();
	return CoolingLowerLimit;
}

double  CGrowBoxDefines::GetCoolingUpperLimit()
{
	checkInitialization();
	return CoolingUpperLimit;
}

BOOL CGrowBoxDefines::useUSBBoard()
{
	checkInitialization();
	return UseUSBBoard;
}

int CGrowBoxDefines::GetVentilatingRelais()
{	checkInitialization();
	return VentilatingRelais;
}

int CGrowBoxDefines::GetHumidifyingRelais()
{	checkInitialization();
	return HumidifyingRelais;
}

int CGrowBoxDefines::GetDryingRelais()
{	checkInitialization();
	return DryingRelais;
}

int CGrowBoxDefines::GetHeatingRelais()
{	checkInitialization();
	return HeatingRelais;
}

int CGrowBoxDefines::GetCoolingRelais()
{	checkInitialization();
	return CoolingRelais;
}


double CGrowBoxDefines::GetIniValue(char* id, double defaultVal)
{
	char buffer [1000];
	memset(buffer,0,sizeof(buffer));
	GetPrivateProfileString("Behaviour",id, "", buffer, sizeof(buffer), iniFilePath);
//	TRACE2("\n 1. %s %s\n",id,buffer);
	if (strlen(buffer) == 0)  {
		sprintf(buffer,"%.2f",defaultVal);
		WritePrivateProfileString("Behaviour",id, buffer, iniFilePath); 
//		TRACE1("\ndefValString %s\n",buffer);
	}
	return atof(buffer);
}

int CGrowBoxDefines::GetIniInt(char* id, int defaultVal)
{

	int res = GetPrivateProfileInt("Behaviour",id, 0xFF,  iniFilePath);
//	TRACE2("\n 1. %s %s\n",id,buffer);
	if (res == 0xFF)  {
		char buffer [1000];
		sprintf(buffer,"%i",defaultVal);
		res = defaultVal;
		WritePrivateProfileString("Behaviour",id, buffer, iniFilePath); 
//		TRACE1("\ndefValString %s\n",buffer);
	}
	return res;
}


void CGrowBoxDefines::setIniFilePath()
{
//	MessageBox(NULL,"must be specified","error",MB_OK);

	// setting the ini file path is a hack, who knows a better method ?
	// lets use the windows "help file interface"
	memset(&iniFilePath,0,sizeof(iniFilePath));
	strncpy(iniFilePath,AfxGetApp()->m_pszHelpFilePath,strlen(AfxGetApp()->m_pszHelpFilePath)-3);
	strcat(iniFilePath,"ini");
	//TRACE1("\nini File path %s\n",iniFilePath);
}


