// JobSeek.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "JobSeek.h"
#include "grep.h"
#include "LoginDialog.h"
#include "SearchJobDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJobSeekApp

BEGIN_MESSAGE_MAP(CJobSeekApp, CWinApp)
	//{{AFX_MSG_MAP(CJobSeekApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJobSeekApp construction

CJobSeekApp::CJobSeekApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CJobSeekApp object

CJobSeekApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CJobSeekApp initialization

BOOL CJobSeekApp::InitInstance()
{
	AfxEnableControlContainer();

	if(!AfxOleInit())//这就是初始化COM库
	{
		AfxMessageBox(_T("OLE初始化出错!"));
		return FALSE;
	} 

	Grep_Initialize();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	strSRC= _T("Driver=SQL Server;Server=");
	strSRC += _T("(local)");
	strSRC += _T(";Database=");
	strSRC += _T("JobSeek");
	strSRC += _T(";UID=sa;PWD=123");

	_variant_t varSRC(strSRC);
	_bstr_t bstrSRC(strSRC);
	try
	{
		m_pConnection.CreateInstance("ADODB.Connection");
		m_pConnection->Open(bstrSRC,"","",-1);

		pCommand.CreateInstance(__uuidof(Command));
	}
	catch(_com_error e)
	{
		AfxMessageBox(_T("数据库连接出错!"));
		pCommand.Release();
		m_pConnection.Release();
		return FALSE;
	}

	CLoginDialog dialog;
	m_pMainWnd = &dialog;
	dialog.DoModal();

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CJobSeekApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	Grep_UnInitialize();

	pCommand.Release();

	if(m_pConnection->State)
		m_pConnection->Close();
	m_pConnection.Release();

	return CWinApp::ExitInstance();
}
