// RegisterDialog.cpp : implementation file
//

#include "stdafx.h"
#include "jobseek.h"
#include "RegisterDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRegisterDialog dialog


CRegisterDialog::CRegisterDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CRegisterDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRegisterDialog)
	m_username = _T("");
	m_password = _T("");
	m_repassword = _T("");
	m_email = _T("");
	m_age = _T("");
	m_nSex = 0;
	m_sMajor = _T("");
	m_sExperience = _T("");
	m_sIndustryName = _T("");
	m_sSalary = _T("");
	m_sEmailAddress = _T("");
	m_sEmailAttachment = _T("");
	m_sEmailCode = _T("");
	m_sEmailContent = _T("");
	m_EmailName = _T("");
	m_nPort = 0;
	m_sSmtp = _T("");
	//}}AFX_DATA_INIT
}


void CRegisterDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRegisterDialog)
	DDX_Control(pDX, IDC_REGION_COMBO, m_cbRegion);
	DDX_Control(pDX, IDC_PROVINCE_COMBO, m_cbProvince);
	DDX_Control(pDX, IDC_EDUCATION_COMBO, m_cbEducation);
	DDX_Text(pDX, IDC_USERNAME_EDIT, m_username);
	DDX_Text(pDX, IDC_PASSWORD_EDIT, m_password);
	DDX_Text(pDX, IDC_REPASSWORD_EDIT, m_repassword);
	DDX_Text(pDX, IDC_EMAIL_EDIT, m_email);
	DDX_Text(pDX, IDC_AGE_EDIT, m_age);
	DDX_Radio(pDX, IDC_MALE_RADIO, m_nSex);
	DDX_Text(pDX, IDC_MAJOR_EDIT, m_sMajor);
	DDX_Text(pDX, IDC_EXPERENCE_EDIT, m_sExperience);
	DDX_Text(pDX, IDC_INDUSTRY_EDIT, m_sIndustryName);
	DDX_Text(pDX, IDC_SALARY_EDIT, m_sSalary);
	DDX_Text(pDX, IDC_EMAILAddress_EDIT, m_sEmailAddress);
	DDX_Text(pDX, IDC_EMAILAttachment_EDIT, m_sEmailAttachment);
	DDX_Text(pDX, IDC_EMAILCode_EDIT, m_sEmailCode);
	DDX_Text(pDX, IDC_EMAILContent_EDIT, m_sEmailContent);
	DDX_Text(pDX, IDC_EMAILName_EDIT, m_EmailName);
	DDX_Text(pDX, IDC_EMAILPORT_EDIT, m_nPort);
	DDX_Text(pDX, IDC_EMAILSMTP_EDIT, m_sSmtp);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRegisterDialog, CDialog)
	//{{AFX_MSG_MAP(CRegisterDialog)
	ON_BN_CLICKED(ID_REGISTER, OnRegister)
	ON_BN_CLICKED(IDC_CHECK_USERNAME, OnCheckUsername)
	ON_CBN_SELCHANGE(IDC_PROVINCE_COMBO, OnSelchangeProvinceCombo)
	ON_BN_CLICKED(ID_REGISTER_CLOSE, OnRegisterClose)
	ON_WM_CREATE()
	ON_EN_CHANGE(IDC_EMAIL_EDIT, OnChangeEmailcodeEdit)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////
//初始化省份组合框
void CRegisterDialog::initProvinceComb()
{
	CString strSQL;
	strSQL.Format(_T("select * from Province"));
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
		return ;
	}
	
	m_cbProvince.AddString(_T("不限"));
	while(!pPtr->GetadoEOF())
	{
		_variant_t varName;
		varName = pPtr->GetCollect("Name");
		CString strName = (char *)_bstr_t(varName);
		strName.TrimRight();
//		AfxMessageBox(strName);
		m_cbProvince.AddString(strName);
		pPtr->MoveNext();
	}

	m_cbProvince.SetCurSel(0);
	m_cbRegion.AddString(_T("不限"));
	m_cbRegion.SetCurSel(0);
	pPtr->Close();
	pPtr.Release();
}

//初始化学历组合框
void CRegisterDialog::initEducationComb()
{
	CString strSQL;
	strSQL.Format(_T("select * from Education"));
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
		return ;
	}
	
	int index = -1;
	
	while(!pPtr->GetadoEOF())
	{
		_variant_t varName;
		varName = pPtr->GetCollect("Name");
		CString strName = (char *)_bstr_t(varName);
		strName.TrimRight();
		m_cbEducation.AddString(strName);
		pPtr->MoveNext();
		index = 0;
	}

	m_cbEducation.SetCurSel(index);
	pPtr->Close();
	pPtr.Release();
}

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CRegisterDialog message handlers

BOOL CRegisterDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	initEducationComb();
	initProvinceComb();
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//执行SQL更新语句，如插入、更新、删除
inline bool CRegisterDialog::ExcuteSQL(CString strSQL)
{
	try
	{
		COleVariant vtOptional((long)DISP_E_PARAMNOTFOUND,VT_ERROR);
		theApp.m_pConnection->Execute(_bstr_t(strSQL),&vtOptional,-1);
	}
	catch(_com_error e)
	{
//		AfxMessageBox(e->ErrorMessage());
		throw e;
		return false;
	}
	return true;
}

//插入一条求职者数据到数据库里
void CRegisterDialog::insertJobSeekers(CString name, CString sex, CString age, CString EID, CString prof, CString workexp, CString areacode, 
	CString industryid, CString salary, CString email, CString pwd,CString smtpserver,
	int smptport,CString eusername,CString epassword,CString contentfilename,CString attachmentfilen)
{
	CString strSQL;
	strSQL.Format(_T("insert into JobSeekers(Name,Sex,Age,EID,Professional,WorkExperience,AreaCode,IndustryID,Salary,Email,PWD,SmtpServer,SmtpPort,EUserName,EPassWord,ContentFileName,AttachmentFileName) values (\'%s\',\'%s\',%s,%s,\'%s\',%s,\'%s\',%s,%s,\'%s\',\'%s\',\'%s\',%d,\'%s\',\'%s\',\'%s\',\'%s\')"), name, sex, age, EID, prof, workexp,areacode,industryid,salary,email,pwd,smtpserver,smptport,eusername,epassword,contentfilename,attachmentfilen);
//	AfxMessageBox(strSQL);
	ExcuteSQL(strSQL);
}

void CRegisterDialog::OnRegister() 
{
	UpdateData(TRUE);
	if(m_password == "")
	{
		AfxMessageBox(_T("密码不能为空!"));
		return ;
	}

	if(m_password != m_repassword)
	{
		AfxMessageBox(_T("两次输入密码不一致!"));
		return ;
	}

	CString sex;
	if(m_nSex == 0)
	{
		sex = _T("男");
	}
	else
	{
		sex = _T("女");
	}

	CString eduName;
	m_cbEducation.GetWindowText(eduName);
	CString eid = getEID(eduName);

	CString areaName;
	m_cbRegion.GetWindowText(areaName);
	CString areaCode = getAreaCodeFromName(areaName);

	CString industryID = getIndustryIDFromName(m_sIndustryName);

	insertJobSeekers(m_username, sex, m_age, eid, m_sMajor, m_sExperience, areaCode, industryID, m_sSalary, 
		m_email, m_password,m_sSmtp,m_nPort,m_username,m_sEmailCode,m_sEmailContent,m_sEmailAttachment);
	AfxMessageBox(_T("注册成功!"));
//	this->CloseWindow();
}

CString CRegisterDialog::getAreaCodeFromName(CString areaName)
{
	CString strSQL;
	strSQL.Format(_T("select * from Region where Name = \'%s\'"), areaName);
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

	CString strAreaCode;

	if(!pPtr->GetadoEOF())
	{
		_variant_t varAreaCode;
		varAreaCode  = pPtr->GetCollect ("AreaCode");
		strAreaCode  = (char *)_bstr_t(varAreaCode);
		strAreaCode.TrimRight();
//		AfxMessageBox(strAreaCode);
		pPtr->MoveNext();
	}
	pPtr->Close();
	pPtr.Release();
	return strAreaCode;
}

//通过省份名称从数据库中查到省份的编号
CString CRegisterDialog::getProvinceID(CString proName)
{
	CString strSQL;
	strSQL.Format(_T("select * from Province where Name = \'%s\'"), proName);
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

	CString strPID;

	if(!pPtr->GetadoEOF())
	{
		_variant_t varPID;
		varPID  = pPtr->GetCollect ("PID");
		strPID  = (char *)_bstr_t(varPID);
		strPID.TrimRight();
//		AfxMessageBox(strPID);
		pPtr->MoveNext();
	}
	pPtr->Close();
	pPtr.Release();
	return strPID;
}


//通过学历名称从数据库中查得学历的编号
CString CRegisterDialog::getEID(CString education)
{
	CString strSQL;
	strSQL.Format(_T("select * from Education where Name = \'%s\'"), education);
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

	CString strEID;

	if(!pPtr->GetadoEOF())
	{
		_variant_t varEID;

		varEID  = pPtr->GetCollect ("EID");
		
		strEID  = (char *)_bstr_t(varEID);
		strEID.TrimRight();
//		AfxMessageBox(strEID);
		pPtr->MoveNext();
	}
	pPtr->Close();
	pPtr.Release();
	return strEID;
}

//插入一条新数据至行业表
void CRegisterDialog::InsertANewIndustry(CString indName)
{
	CString strSQL;
	strSQL.Format(_T("insert into Industry values ('%s')"), indName);	
	ExcuteSQL(strSQL);
}

//通过行业名称得到行业编号
CString CRegisterDialog::getIndustryIDFromName(CString indName)
{
	CString strSQL;
	strSQL.Format(_T("select * from Industry where Name = \'%s\'"), indName);
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

	CString strID;

	if(!pPtr->GetadoEOF())
	{
		_variant_t varID;

		varID  = pPtr->GetCollect ("IndustryID");
		
		strID  = (char *)_bstr_t(varID);
		strID.TrimRight();
//		AfxMessageBox(strEID);
//		pPtr->MoveNext();
		pPtr->Close();
		pPtr.Release();
		return strID;
	}
	else
	{
		pPtr->Close();
		pPtr.Release();
		InsertANewIndustry(indName);
		return getIndustryIDFromName(indName);
	}

}

//在数据库中查找指定的用户名,返回该用户名是否存在
bool CRegisterDialog::FindUserNameInDB(CString username)
{
	CString strSQL;
	strSQL.Format(_T("select * from JobSeekers where Name = \'%s\'"), username);
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
		return false;
	}
	
	if(!pPtr->GetadoEOF())
	{
		pPtr->Close();
		pPtr.Release();
		return true;
	}
	else
	{
		pPtr->Close();
		pPtr.Release();
		return false;
	}
}

void CRegisterDialog::OnCheckUsername() 
{
	UpdateData(TRUE);
	bool exist;
	try
	{
		exist = FindUserNameInDB(m_username);
	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
		return ;
	}

	if(exist)
	{
		AfxMessageBox(_T("抱歉,指定的用户名已存在,请更换一个用户名!"));
	}
	else
	{
		AfxMessageBox(_T("恭喜,指定的用户名未被占用!"));
	}
}

//更新地区组合框中的内容
void CRegisterDialog::updateRegion(CString PID)
{
	CString strSQL;
	strSQL.Format(_T("select * from Region where PID = \'%s\'"), PID);
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
		return ;
	}

	int index = -1;
	m_cbRegion.ResetContent();
	
	while(!pPtr->GetadoEOF())
	{
		_variant_t varName;
		varName = pPtr->GetCollect("Name");
		CString strName = (char *)_bstr_t(varName);
		strName.TrimRight();
//		AfxMessageBox(strName);
		m_cbRegion.AddString(strName);
		pPtr->MoveNext();
		index = 0;
	}

	m_cbRegion.SetCurSel(index);
	pPtr->Close();
	pPtr.Release();
	UpdateData(FALSE);
}

void CRegisterDialog::OnSelchangeProvinceCombo() 
{
	UpdateData(TRUE);
	int index = m_cbProvince.GetCurSel();
	if(index == 0)
	{
		m_cbRegion.ResetContent();
		m_cbRegion.AddString(_T("不限"));
		m_cbRegion.SetCurSel(0);
		return ;
	}

	CString proName;
	m_cbProvince.GetLBText(index, proName);
	CString PID = getProvinceID(proName);
	updateRegion(PID);
//	AfxMessageBox(proName);
}

void CRegisterDialog::OnRegisterClose() 
{
	// TODO: Add your control notification handler code here
	//exit(0);
	CRegisterDialog::OnOK();
}

int CRegisterDialog::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

void CRegisterDialog::OnChangeEmailcodeEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here	
}



void CRegisterDialog::OnPaint() 
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
		bkg.LoadBitmap(IDB_BITMAP_BGR);//载入位图
		
		BITMAP bkginfo;
		bkg.GetBitmap(&bkginfo);//获取位图信息
		
		memdc.SelectObject(&bkg);
		
		RECT rect;
		GetWindowRect(&rect);//获取对话框信息
		
		dc.StretchBlt(0,0,rect.right-rect.left,rect.bottom-rect.top,&memdc,0,0,bkginfo.bmWidth,bkginfo.bmHeight,SRCCOPY);
 }	// Do not call CDialog::OnPaint() for painting messages
}
