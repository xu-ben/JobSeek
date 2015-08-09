#if !defined(AFX_SEARCHJOBDIALOG_H__6DE0DCE8_831A_4F0B_BAB3_9E5A100373E5__INCLUDED_)
#define AFX_SEARCHJOBDIALOG_H__6DE0DCE8_831A_4F0B_BAB3_9E5A100373E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SearchJobDialog.h : header file
//

#include "JobInfoDlg.h"

extern CJobSeekApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CSearchJobDialog dialog

class CSearchJobDialog : public CDialog
{
// Construction
public:
	CSearchJobDialog(CWnd* pParent = NULL);   // standard constructor
	CString GetFileText(int index);
	CString getCareerName(CString fileText);
	CString getWorkDuration(CString fileText);
	CString getNeedWorker(CString fileText);
	CString getReleaseDate(CString fileText);
	int getNeedEID(CString fileText);
	CString getAreaCodeFromAreaName(CString areaName);
	CString getAreaCode(CString fileText);
	CString getEmail(CString fileText);
	CString getPID(CString fileText);
	CString getPhoneNumber(CString fileText);
	CString getContactName(CString fileText);
	CString getWebSite(CString fileText);
	int getCompanyIDFromName(CString companyName);
	CString getCompanyName(CString fileText);
	void insertACompany(CString fileText);
	CString getCompanyIndustry(CString fileText);
	CString getPopulation(CString fileText);
	CString getIndustryIDFromName(CString indName);
	void InsertANewIndustry(CString indName);
	int getCurFileIndex();
	void updateCurFileIndex(int index);
	void initProvinceComb();
	CString getProvinceID(CString proName);
	void updateRegion(CString PID);
	CString getAreaCodeFromName(CString areaName);
	inline bool ExcuteSQL(CString strSQL);
	void SearchJobInDB(CString strSQL);
	void InitInfoDlg(CJobInfoDlg &dlg, CString user);
	// Dialog Data
	//{{AFX_DATA(CSearchJobDialog)
	enum { IDD = IDD_SEARCH_DIALOG };
	CComboBox	m_cbRegion;
	CComboBox	m_cbProvince;
	CString	m_sUserName;
	CString	m_sEmail;
	CString	m_sJobName;
	CString	m_sCompanyName;
	BOOL	m_fuzzyCareer;
	BOOL	m_fuzzyCompany;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSearchJobDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSearchJobDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnInportData();
	afx_msg void OnButtonUpdata();
	afx_msg void OnSearchJob();
	afx_msg void OnSelchangeProvince();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEARCHJOBDIALOG_H__6DE0DCE8_831A_4F0B_BAB3_9E5A100373E5__INCLUDED_)
