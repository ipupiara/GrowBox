// GrowBoxDlg.h : header file
//

#if !defined(AFX_GROWBOXDLG_H__F2FD9AA7_3A6F_11DA_BB67_00001B483E74__INCLUDED_)
#define AFX_GROWBOXDLG_H__F2FD9AA7_3A6F_11DA_BB67_00001B483E74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#include "StateClass.h"
#include "resource.h"
#include "GrowBoxDefines.h"



/////////////////////////////////////////////////////////////////////////////
// CGrowBoxDlg dialog

class CGrowBoxDlg : public CDialog
{
// Construction
public:
	void onExitIdleNotVentilating();
	void onEntryIdleNotVentilating();	
	void onExitIdleVentilating();
	void onEntryIdleVentilating();
	void setBoldText(CEdit& edt, CString& txt);
	void stopVentilatingStopTimer();
	void startVentilatingStopTimer(double minutes);
	void stopVentilatingStartTimer();
	void startVentilatingStartTimer(double minutes);

	CGrowBoxDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CGrowBoxDlg();

// Dialog Data
	//{{AFX_DATA(CGrowBoxDlg)
	enum { IDD = IDD_GROWBOX_DIALOG };
	CEdit	m_EditStateText;
	CEdit	m_EditTitle;
	DWORD	m_TimerCount;
	double	m_Humidity;
	DWORD	m_HygroCnt;
	double	m_Temperature;
	double	m_DryingLowerLimit;
	double	m_DryingUpperLimit;
	double	m_HeatingLowerLimit;
	double	m_HeatingUpperLimit;
	double	m_HumidifyingLowerLimit;
	double	m_HumidifyingUpperLimit;
	double	m_IdleVentilationDelayMinutes;
	double	m_IdleVentilationMinutes;
	double	m_CoolingLowerLimit;
	double	m_CoolingUpperLimit;
	BOOL	m_RelaisCool;
	BOOL	m_RelaisDry;
	BOOL	m_RelaisHeat;
	BOOL	m_RelaisHumidify;
	BOOL	m_RelaisVentilate;
	CString	m_RelaisHumidifyCaption;
	CString	m_CoolingRelaisCaption;
	CString	m_DryingRelaisCaption;
	CString	m_HeatingRelaisCaption;
	CString	m_VentilatingRelaisCaption;
	BOOL	m_DBStateOK;
	CString	m_HeaterCoolerStates;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGrowBoxDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CGrowBoxDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void OnOK();
	afx_msg void OnBUTTONMinimize();
	afx_msg void OnBUTTONVentilation();
	afx_msg void OnBUTTONStopVentilation();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void setStateText();
	CGrowBoxController growBoxController;
	void GetBehaviourValues();
	void GetRelaisValues();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GROWBOXDLG_H__F2FD9AA7_3A6F_11DA_BB67_00001B483E74__INCLUDED_)
