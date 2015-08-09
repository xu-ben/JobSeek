// LoginDialog.cpp : implementation file
//

#include "stdafx.h"
#include "jobseek.h"
#include "LoginDialog.h"
#include "RegisterDialog.h"
#include "SearchJobDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLoginDialog dialog


CLoginDialog::CLoginDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoginDialog)
	m_username = _T("");
	m_password = _T("");
	//}}AFX_DATA_INIT
}


void CLoginDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoginDialog)
	DDX_Text(pDX, IDC_USERNAME_EDIT, m_username);
	DDX_Text(pDX, IDC_PASSWORD_EDIT, m_password);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLoginDialog, CDialog)
	//{{AFX_MSG_MAP(CLoginDialog)
	ON_BN_CLICKED(ID_LOGIN, OnLogin)
	ON_BN_CLICKED(ID_REGISTER, OnRegister)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoginDialog message handlers

//在数据库中查找指定的用户名和密码是否存在并且配对
bool CLoginDialog::SearchUserInDB(CString username, CString password)
{
	_RecordsetPtr pPtr;
	CString strSQL;
	strSQL.Format(_T("select * from JobSeekers where Name = \'%s\' and PWD = \'%s\'"), username, password);
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
	
	if(!pPtr->adoEOF)
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


//在数据库中查得Email
CString CLoginDialog::getEmailInDB(CString username)
{
	_RecordsetPtr pPtr;
	CString strSQL;
	strSQL.Format(_T("select * from JobSeekers where Name = \'%s\'"), username);
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
	
	CString str("");
	if(!pPtr->GetadoEOF())
	{
		_variant_t var;
		var  = pPtr->GetCollect ("Email");
		str  = (char *)_bstr_t(var);
		str.TrimRight();
//		AfxMessageBox(strPID);
//		pPtr->MoveNext();
		pPtr->Close();
		pPtr.Release();
	}
	return str;
}

void CLoginDialog::OnLogin() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	bool find;
	try
	{
		find = SearchUserInDB(m_username, m_password);
	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
		return ;
	}

	if(!find)
	{
		AfxMessageBox(_T("输入的用户名或密码错误，如果您没有使用过本系统，请先注册!"));
		return ;
	}
	else
	{
		CString email;
		try
		{
			email = getEmailInDB(m_username);
		}
		catch(_com_error *e)
		{
			AfxMessageBox(e->ErrorMessage());
			return ;
		}

		CSearchJobDialog sjd;
		sjd.m_sUserName = m_username;
		sjd.m_sEmail = email;
		this->CloseWindow();
		sjd.DoModal();
		CLoginDialog::OnOK();
	}	
}

void CLoginDialog::OnRegister() 
{
	// TODO: Add your control notification handler code here
	CRegisterDialog dlg;
	dlg.DoModal();
}

BOOL CLoginDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_username = "";
	m_password = "";

//	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLoginDialog::OnPaint() 
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
		bkg.LoadBitmap(IDB_BITMAP_BGL);//载入位图
		
		BITMAP bkginfo;
		bkg.GetBitmap(&bkginfo);//获取位图信息
		
		memdc.SelectObject(&bkg);
		
		RECT rect;
		GetWindowRect(&rect);//获取对话框信息
		
		dc.StretchBlt(0,0,rect.right-rect.left,rect.bottom-rect.top,&memdc,0,0,bkginfo.bmWidth,bkginfo.bmHeight,SRCCOPY);
 }	// Do not call CDialog::OnPaint() for painting messages
}
