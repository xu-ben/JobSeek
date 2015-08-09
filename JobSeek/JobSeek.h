// JobSeek.h : main header file for the JOBSEEK application
//

#if !defined(AFX_JOBSEEK_H__63DC1655_6457_4CAA_A9AB_506DECB59AE7__INCLUDED_)
#define AFX_JOBSEEK_H__63DC1655_6457_4CAA_A9AB_506DECB59AE7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CJobSeekApp:
// See JobSeek.cpp for the implementation of this class
//

class CJobSeekApp : public CWinApp
{
public:
	//数据库连接字符串
	CString strSRC;

	_ConnectionPtr m_pConnection;
	_CommandPtr pCommand;

	CJobSeekApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJobSeekApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CJobSeekApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JOBSEEK_H__63DC1655_6457_4CAA_A9AB_506DECB59AE7__INCLUDED_)
