#if !defined(AFX_LOGINDIALOG_H__E9A40B1E_8419_4108_82EE_19F95D8F05F4__INCLUDED_)
#define AFX_LOGINDIALOG_H__E9A40B1E_8419_4108_82EE_19F95D8F05F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LoginDialog.h : header file
//

extern CJobSeekApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CLoginDialog dialog

class CLoginDialog : public CDialog
{
// Construction
public:
	CLoginDialog(CWnd* pParent = NULL);   // standard constructor
	CString getEmailInDB(CString username);
	bool SearchUserInDB(CString username, CString password);

// Dialog Data
	//{{AFX_DATA(CLoginDialog)
	enum { IDD = IDD_LOGIN_DIALOG };
	CString	m_username;
	CString	m_password;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLoginDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLoginDialog)
	afx_msg void OnLogin();
	afx_msg void OnRegister();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGINDIALOG_H__E9A40B1E_8419_4108_82EE_19F95D8F05F4__INCLUDED_)
