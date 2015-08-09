#if !defined(AFX_JOBINFODLG_H__45E86BA7_3E9B_4B09_9349_238EB29DB755__INCLUDED_)
#define AFX_JOBINFODLG_H__45E86BA7_3E9B_4B09_9349_238EB29DB755__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// JobInfoDlg.h : header file
//

#include "blat.h"
#include "jobseek.h"
#include "grep.h"
#include  "ATLCONV.H"

extern CJobSeekApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CJobInfoDlg dialog

class CJobInfoDlg : public CDialog
{
// Construction
public:
	void DispRecord();
	CString strSQL;
	CJobInfoDlg(CWnd* pParent = NULL);   // standard constructor
	CString getCompanyNameFromID(CString cid);
	CString getAreaNameFromCode(CString areacode);
	CString getEducationFromEID(CString eid);

	CString SmtpServer;
	int port;
	CString EUserName;
	CString EPassWord;
	CString ContentFileName;
	CString AttachmentFileName;
	CString Email;

// Dialog Data
	//{{AFX_DATA(CJobInfoDlg)
	enum { IDD = IDD_JOBINFO_DIALOG };
	CString	m_sCareerName;
	CString	m_sCompanyName;
	CString	m_sContactEmail;
	CString	m_sContactNumber;
	CString	m_sContactor;
	CString	m_sEducation;
	CString	m_iHireNumber;
	CString	m_sReleaseDate;
	CString	m_sWorkDuration;
	CString	m_sWorkPlace;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJobInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	_RecordsetPtr m_pRecordset;
	// Generated message map functions
	//{{AFX_MSG(CJobInfoDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSendmail();
	afx_msg void OnPrerecord();
	afx_msg void OnNextrecord();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JOBINFODLG_H__45E86BA7_3E9B_4B09_9349_238EB29DB755__INCLUDED_)
