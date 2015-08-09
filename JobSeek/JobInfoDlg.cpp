// JobInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "jobseek.h"
#include "JobInfoDlg.h"
#include "Tools.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJobInfoDlg dialog


CJobInfoDlg::CJobInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CJobInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CJobInfoDlg)
	m_sCareerName = _T("");
	m_sCompanyName = _T("");
	m_sContactEmail = _T("");
	m_sContactNumber = _T("");
	m_sContactor = _T("");
	m_sEducation = _T("");
	m_iHireNumber = _T("");
	m_sReleaseDate = _T("");
	m_sWorkDuration = _T("");
	m_sWorkPlace = _T("");
	//}}AFX_DATA_INIT
}


void CJobInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJobInfoDlg)
	DDX_Text(pDX, IDC_JOBNAME_EDIT, m_sCareerName);
	DDX_Text(pDX, IDC_COMPANYNAME_EDIT, m_sCompanyName);
	DDX_Text(pDX, IDC_CONTACTEMAIL_EDIT, m_sContactEmail);
	DDX_Text(pDX, IDC_CONTACTNUMBER_EDIT, m_sContactNumber);
	DDX_Text(pDX, IDC_CONTACTOR_EDIT, m_sContactor);
	DDX_Text(pDX, IDC_EDUCATION_EDIT, m_sEducation);
	DDX_Text(pDX, IDC_HIRENUMBER_EDIT, m_iHireNumber);
	DDX_Text(pDX, IDC_ReleaseDate_EDIT, m_sReleaseDate);
	DDX_Text(pDX, IDC_WORKDURATION_EDIT, m_sWorkDuration);
	DDX_Text(pDX, IDC_WORKPLACE_EDIT, m_sWorkPlace);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CJobInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CJobInfoDlg)
	ON_BN_CLICKED(IDC_SENDMAIL, OnSendmail)
	ON_BN_CLICKED(IDC_PRERECORD, OnPrerecord)
	ON_BN_CLICKED(IDC_NEXTRECORD, OnNextrecord)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJobInfoDlg message handlers

BOOL CJobInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	_variant_t vtSQL(strSQL);
	
	try
	{
		m_pRecordset.CreateInstance(__uuidof(Recordset));
		m_pRecordset->Open(vtSQL, theApp.m_pConnection.GetInterfacePtr(),adOpenDynamic, adLockOptimistic, adCmdText);
	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
		return FALSE;
	}

/*
	while(!m_pRecordset->GetadoEOF())
	{
		_variant_t varID;
		varID  = m_pRecordset->GetCollect ("CarName");
		CString strID  = (char *)_bstr_t(varID);
		strID.TrimRight();
		AfxMessageBox(strID);
		m_pRecordset->MoveNext();
	}
	m_pRecordset->Close();
	m_pRecordset.Release();
*/
	DispRecord();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CJobInfoDlg::OnOK() 
{
	// TODO: Add extra validation here
	m_pRecordset->Close();
	m_pRecordset.Release();
	CDialog::OnOK();
}

bool shouldSend(CString mailto)
{
	CString strSQL;
	strSQL.Format(_T("select * from EmailInf where Email = '%s'"), mailto);
	_RecordsetPtr theRecord;
	theRecord.CreateInstance(__uuidof(Recordset));

	char *temp;
	time_t nowtime = time(NULL);
	tm nt = *localtime(&nowtime); 

	try
	{
		BSTR bstrSQL = strSQL.AllocSysString();
		theRecord->Open(bstrSQL, theApp.m_pConnection.GetInterfacePtr(),
			adOpenDynamic, adLockOptimistic, adCmdText);

		if(theRecord->adoEOF)
		{
			theRecord->AddNew();
			theRecord->Fields->GetItem(("Email"))->Value = _bstr_t(mailto);
			theRecord->Fields->GetItem(("LastSendTime"))->Value = _bstr_t(CString(stroftime(&nt)));
			theRecord->Update();
			theRecord->Close();
			theRecord.Release();
			return true;
		}
		else
		{
			theRecord->Fields->GetItem(("LastSendTime"))->Value = _bstr_t(CString(stroftime(&nt)));
			theRecord->Update();
			theRecord->Close();
			theRecord.Release();
			return false;
/*			tm lt;
			_variant_t vt;
			vt = theRecord->GetCollect("LastSendTime");
			CString ttemp = (char *)_bstr_t(vt);
			AfxMessageBox(ttemp);
*/
		}
	}
	catch(_com_error e)
	{
		throw e;
		return false;
	}

}

void CJobInfoDlg::OnSendmail() 
{
	if(!shouldSend(m_sContactEmail))
	{
		AfxMessageBox(_T("该地址最近投递过邮件，不能频繁发送!"));
		return ;
	}
	LPCTSTR szEmailTo = m_sContactEmail;
//	AfxMessageBox(szEmailTo);
//	szEmailTo = _T("wangqianlan66@126.com");
	LPCTSTR szEmailTitle = _T("应聘邮件");
	LPCTSTR szContentFile = ContentFileName;
	CStringArray aryAttachmentFile;
	aryAttachmentFile.Add(AttachmentFileName);
	

	LPCTSTR szSmtpServer = SmtpServer;
	UINT uSmtpPort = port;
	LPCTSTR szSmtpServerEmail = Email;
	LPCTSTR szReplyToEmail = Email;
	LPCTSTR szUserName = EUserName;
	LPCTSTR szPassword = EPassWord;

	LPCTSTR szSinatureFile=_T("");
	EmailType emailType=EmailType_HTML;
	LPCTSTR szHtmlAlternativeFilePath=_T("");
	BOOL bRequireReceipt=FALSE;
	BOOL bMIME=FALSE;
	LPCTSTR szEncoding=_T("utf-8");
	BOOL bHighPriority=FALSE;
	LPCTSTR szOtherParameters=_T("");

/*
	LPCTSTR szSmtpServer = _T("202.204.112.13");
	UINT uSmtpPort = 25;
	LPCTSTR szSmtpServerEmail = _T("xuben@bjfu.edu.cn");
	LPCTSTR szReplyToEmail = _T("xuben@bjfu.edu.cn");
	LPCTSTR szUserName = _T("xuben");
	LPCTSTR szPassword = _T("jsj082");

	AfxMessageBox(EUserName);
	AfxMessageBox(EPassWord);
	AfxMessageBox(Email);
	AfxMessageBox(SmtpServer);
	AfxMessageBox(ContentFileName);
	AfxMessageBox(AttachmentFileName);
*/

	SendEmail(szEmailTo, 
			  szSmtpServer, uSmtpPort, 
			  szSmtpServerEmail, szReplyToEmail,
			  szUserName, szPassword,
			  szEmailTitle, szContentFile, aryAttachmentFile,
			  szSinatureFile,
			  emailType, szHtmlAlternativeFilePath,
			  bRequireReceipt, bMIME, szEncoding, bHighPriority,
			  szOtherParameters);
	AfxMessageBox(_T("邮件发送成功!"));
}

CString CJobInfoDlg::getCompanyNameFromID(CString cid)
{
	CString strSQL;
	strSQL.Format(_T("select CN from Company where CID = \'%s\'"), cid);
	_RecordsetPtr pPtr;
	_variant_t varSQL(strSQL);

	try
	{
		pPtr.CreateInstance(__uuidof(Recordset));
		pPtr->Open(varSQL,theApp.m_pConnection.GetInterfacePtr(),adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error e)
	{
		throw e;
		return "";
	}

	CString str;
	if(!pPtr->GetadoEOF())
	{
		_variant_t var;
		var = pPtr->GetCollect ("CN");
		str =(char *)_bstr_t(var);
		pPtr->MoveNext();
	}
	
	pPtr->Close();
	pPtr.Release();
	return str;
}

CString CJobInfoDlg::getAreaNameFromCode(CString areacode)
{
	CString strSQL;
	strSQL.Format(_T("select Name from Region where AreaCode = \'%s\'"), areacode);
	_RecordsetPtr pPtr;
	_variant_t varSQL(strSQL);

	try
	{
		pPtr.CreateInstance(__uuidof(Recordset));
		pPtr->Open(varSQL,theApp.m_pConnection.GetInterfacePtr(),adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error e)
	{
		throw e;
		return "";
	}

	CString str;
	if(!pPtr->GetadoEOF())
	{
		_variant_t var;
		var = pPtr->GetCollect ("Name");
		str =(char *)_bstr_t(var);
		pPtr->MoveNext();
	}
	pPtr->Close();
	pPtr.Release();
	return str;
}

CString CJobInfoDlg::getEducationFromEID(CString eid)
{
	CString strSQL;
	strSQL.Format(_T("select Name from Education where EID = \'%s\'"), eid);
	_RecordsetPtr pPtr;
	_variant_t varSQL(strSQL);

	try
	{
		pPtr.CreateInstance(__uuidof(Recordset));
		pPtr->Open(varSQL,theApp.m_pConnection.GetInterfacePtr(),adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error e)
	{
		throw e;
		return "";
	}

	CString str;
	if(!pPtr->GetadoEOF())
	{
		_variant_t var;
		var = pPtr->GetCollect ("Name");
		str =(char *)_bstr_t(var);
		pPtr->MoveNext();
	}
	pPtr->Close();
	pPtr.Release();
	return str;
}

//显示记录
void CJobInfoDlg::DispRecord()
{
	_variant_t theValue;
	if(!m_pRecordset->adoEOF)
	{
		//获取职位名称
		theValue = m_pRecordset->GetCollect("CarName");
		if(theValue.vt!=VT_NULL)
			m_sCareerName = (char *)_bstr_t(theValue);

		//公司名称
		theValue = m_pRecordset->GetCollect("CID");
		if(theValue.vt!=VT_NULL)
			m_sCompanyName = getCompanyNameFromID((char *)_bstr_t(theValue));

		//获取职位发布日期
		theValue = m_pRecordset->GetCollect("ReleaseDate");
		if(theValue.vt!=VT_NULL)
			m_sReleaseDate = (char *)_bstr_t(theValue);

		//获取工作地点
		theValue = m_pRecordset->GetCollect("AreaCode");
		if(theValue.vt!=VT_NULL)
			m_sWorkPlace = getAreaNameFromCode((char *)_bstr_t(theValue));

		//获取学历要求
		theValue = m_pRecordset->GetCollect("EID");
		if(theValue.vt!=VT_NULL)
			m_sEducation = getEducationFromEID((char *)_bstr_t(theValue));

		//获取工作年限
		theValue = m_pRecordset->GetCollect("WorkDuration");
		if(theValue.vt!=VT_NULL)
			m_sWorkDuration = (char *)_bstr_t(theValue);

		//获取招聘人数
		theValue = m_pRecordset->GetCollect("NeedWorker");
		if(theValue.vt!=VT_NULL)
			m_iHireNumber = (char *)_bstr_t(theValue);
		

		//获取联系人称呼
		theValue = m_pRecordset->GetCollect("ContactName");
		if(theValue.vt!=VT_NULL)
			m_sContactor = (char *)_bstr_t(theValue);

		//获取联系电话
		theValue = m_pRecordset->GetCollect("PhoneNumber");
		if(theValue.vt!=VT_NULL)
			m_sContactNumber = (char *)_bstr_t(theValue);

		//获取邮件地址
		theValue = m_pRecordset->GetCollect("Email");
		if(theValue.vt!=VT_NULL)
			m_sContactEmail = (char *)_bstr_t(theValue);
	}
	UpdateData(FALSE);
}

void CJobInfoDlg::OnPrerecord() 
{
	m_pRecordset->MoveNext();
	if(m_pRecordset->adoEOF)
	{
		m_pRecordset->MoveLast();
	}
	DispRecord();
}

void CJobInfoDlg::OnNextrecord() 
{
	m_pRecordset->MovePrevious();
	if(m_pRecordset->adoBOF)
	{
		m_pRecordset->MoveFirst();
	}
	DispRecord();
}

void CJobInfoDlg::OnPaint() 
{
//	CPaintDC dc(this); // device context for painting
	
	if (IsIconic())
	{}
	else
	{
		//CDialog::OnPaint();把这个注释掉，不调用基类的OnPaint()
		
		CPaintDC dc(this);
		
		CDC memdc;
		memdc.CreateCompatibleDC(&dc);//创建兼容DC
		
		CBitmap bkg;
		bkg.LoadBitmap(IDB_BITMAP_BGIN);//载入位图
		
		BITMAP bkginfo;
		bkg.GetBitmap(&bkginfo);//获取位图信息
		
		memdc.SelectObject(&bkg);
		
		RECT rect;
		GetWindowRect(&rect);//获取对话框信息
		
		dc.StretchBlt(0,0,rect.right-rect.left,rect.bottom-rect.top,&memdc,0,0,bkginfo.bmWidth,bkginfo.bmHeight,SRCCOPY);
 }	// Do not call CDialog::OnPaint() for painting messages
}
