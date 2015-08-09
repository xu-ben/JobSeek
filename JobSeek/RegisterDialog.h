#if !defined(AFX_REGISTERDIALOG_H__13791276_C1C3_448F_A091_DD5AA27F96A8__INCLUDED_)
#define AFX_REGISTERDIALOG_H__13791276_C1C3_448F_A091_DD5AA27F96A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RegisterDialog.h : header file
//

extern CJobSeekApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CRegisterDialog dialog

class CRegisterDialog : public CDialog
{
// Construction
public:
	CRegisterDialog(CWnd* pParent = NULL);   // standard constructor
	void initEducationComb();
	void insertJobSeekers(CString name, CString sex, CString age, CString EID, CString prof, CString workexp, CString areacode, CString industryid, CString salary, CString email, CString pwd, CString SmtpServer, int smtpPort, CString EUserName, CString EPassword, CString contentFileName, CString atFileName);
	bool FindUserNameInDB(CString username);
	CString getEID(CString education);
	CString getProvinceID(CString proName);
	void initProvinceComb();
	void updateRegion(CString PID);
	CString getAreaCodeFromName(CString areaName);
	CString getIndustryIDFromName(CString indName);
	void InsertANewIndustry(CString indName);
	inline bool ExcuteSQL(CString strSQL);
// Dialog Data
	//{{AFX_DATA(CRegisterDialog)
	enum { IDD = IDD_REGISTER_DIALOG };
	CComboBox	m_cbRegion;
	CComboBox	m_cbProvince;
	CComboBox m_cbEducation;
	CString	m_username;
	CString	m_password;
	CString	m_repassword;
	CString	m_email;
	CString	m_age;
	int		m_nSex;
	CString	m_sMajor;
	CString	m_sExperience;
	CString	m_sIndustryName;
	CString	m_sSalary;
	CString	m_sEmailAddress;
	CString	m_sEmailAttachment;
	CString	m_sEmailCode;
	CString	m_sEmailContent;
	CString	m_EmailName;
	int		m_nPort;
	CString	m_sSmtp;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegisterDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRegisterDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnRegister();
	afx_msg void OnCheckUsername();
	afx_msg void OnSelchangeProvinceCombo();
	afx_msg void OnRegisterClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnChangeEmailcodeEdit();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGISTERDIALOG_H__13791276_C1C3_448F_A091_DD5AA27F96A8__INCLUDED_)
