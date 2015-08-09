// SearchJobDialog.cpp : implementation file
//

#include "stdafx.h"
#include "SearchJobDialog.h"
#include <fstream>
#include <iostream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSearchJobDialog dialog


CSearchJobDialog::CSearchJobDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSearchJobDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSearchJobDialog)
	m_sUserName = _T("");
	m_sEmail = _T("");
	m_sJobName = _T("");
	m_sCompanyName = _T("");
	m_fuzzyCareer = FALSE;
	m_fuzzyCompany = FALSE;
	//}}AFX_DATA_INIT
}


void CSearchJobDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSearchJobDialog)
	DDX_Control(pDX, IDC_COMBO_REGION, m_cbRegion);
	DDX_Control(pDX, IDC_COMBO_PROVINCE, m_cbProvince);
	DDX_Text(pDX, IDC_USERNAME_STATIC, m_sUserName);
	DDX_Text(pDX, IDC_EMAIL_STATIC, m_sEmail);
	DDX_Text(pDX, IDC_JOB_EDIT, m_sJobName);
	DDX_Text(pDX, IDC_COMPANYNAME_EDIT, m_sCompanyName);
	DDX_Check(pDX, IDC_CHECKCAREER, m_fuzzyCareer);
	DDX_Check(pDX, IDC_CHECKCOMPANY, m_fuzzyCompany);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSearchJobDialog, CDialog)
	//{{AFX_MSG_MAP(CSearchJobDialog)
	ON_BN_CLICKED(IDC_CLOSE, OnClose)
	ON_BN_CLICKED(ID_INPORT_DATA, OnInportData)
	ON_BN_CLICKED(IDC_BUTTON_UPDATA, OnButtonUpdata)
	ON_BN_CLICKED(ID_SEARCH_JOB, OnSearchJob)
	ON_CBN_SELCHANGE(IDC_COMBO_PROVINCE, OnSelchangeProvince)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSearchJobDialog message handlers

BOOL CSearchJobDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	initProvinceComb();
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSearchJobDialog::OnClose() 
{
	// TODO: Add your control notification handler code here
	AfxMessageBox(_T("即将关闭系统!"));
	exit(0);
}

/*//从Province表中查询
void SelectFromProvince(CString *ProvinceID, CString *ProvinceName)
{
	CString strSQL = _T("select * from Province");
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

	int i=0;
	while(!pPtr->GetadoEOF())
	{
		_variant_t varNo;
		_variant_t varNa;
		
		varNo=pPtr->GetCollect ("PID");//选择省编号
		varNa=pPtr->GetCollect("Name");//选择省名字
		
		CString strNo=(char *)_bstr_t(varNo);
		CString strNa=(char *)_bstr_t(varNa);
		
//		AfxMessageBox(strNa);
		
		ProvinceID[i] = strNo;//插入省编号到数组
		ProvinceName[i] = strNa;//插入省名字到数组
		i++;
		
		pPtr->MoveNext();
	}	
	pPtr->Close();
	pPtr.Release();
}


//更新Region表中的数据
void UpdateRegion()
{
	CString ProvinceID[32];
	CString ProvinceName[32];
	SelectFromProvince(ProvinceID, ProvinceName);

	_ConnectionPtr pConn;
	if (FAILED(pConn.CreateInstance("ADODB.Connection")))
	{
		AfxMessageBox(_T("Create Instance failed!"));
		return;
	}
	
	CString strSRC = _T("Driver=SQL Server;Server=(local);Database=JobSeek;UID=sa;PWD=123");
	
	_variant_t varSRC(strSRC);
	//	_variant_t varSQL(strSQL);
	_bstr_t bstrSRC(strSRC);
	
	if (FAILED(pConn->Open(bstrSRC,"","",-1)))
	{
		AfxMessageBox(_T("Can not open Database!"));
		pConn.Release();
		return;
	}
	
	COleVariant vtOptional((long)DISP_E_PARAMNOTFOUND,VT_ERROR);
		
	CString strSQL;	
	CString f,m,e;

	for(int i=0;i<31;i++)
	{
		strSQL.Format(_T("update Region set PID=\'%s\' where(PName=\'%s\')"), ProvinceID[i], ProvinceName[i]);
		pConn->Execute(_bstr_t(strSQL),&vtOptional,-1);
	}	
	pConn.Release();
	
	AfxMessageBox(_T("更新Region表成功!"));
}
*/

int CSearchJobDialog::getCompanyIDFromName(CString companyName)
{
	CString strSQL;
	strSQL.Format(_T("select * from Company where CN = \'%s\'"), companyName);
	_RecordsetPtr pPtr;
	_variant_t varSQL(strSQL);

	int value = -1;

	try
	{
		pPtr.CreateInstance(__uuidof(Recordset));
		pPtr->Open(varSQL,theApp.m_pConnection.GetInterfacePtr(),adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error e)
	{
		throw e;
		return value;
	}

	if(!pPtr->GetadoEOF())
	{
		_variant_t varCID;
		
		varCID = pPtr->GetCollect ("CID");

		value =varCID.iVal;

		pPtr->MoveNext();
	}
	
	pPtr->Close();
	pPtr.Release();
	return value;
}

void CSearchJobDialog::insertACompany(CString fileText)
{
	CString companyName = getCompanyName(fileText);
	CString pid = getPID(fileText);
	CString areaCode = getAreaCode(fileText);
	CString industry = getCompanyIndustry(fileText);
	CString industryID = getIndustryIDFromName(industry);
	CString population = getPopulation(fileText);
	CString website = getWebSite(fileText);


	CString strSQL;
	strSQL.Format(_T("select * from Company where CN = '%s'"), companyName);
	_RecordsetPtr theRecord;
	theRecord.CreateInstance(__uuidof(Recordset));

	char *temp;

	try
	{
		BSTR bstrSQL = strSQL.AllocSysString();
		theRecord->Open(bstrSQL, theApp.m_pConnection.GetInterfacePtr(),
			adOpenDynamic, adLockOptimistic, adCmdText);

		if(theRecord->adoEOF)
		{
			theRecord->AddNew();
		}
		theRecord->Fields->GetItem(("CN"))->Value = _bstr_t(companyName);

		if(industryID != _T(""))
		{
			temp = (char *)industryID.GetBuffer(industryID.GetLength());
			theRecord->Fields->GetItem("IndustryID")->Value = (short)atoi(temp);
		}
		if(pid != _T(""))
		{
			temp = (char *)pid.GetBuffer(pid.GetLength());
			theRecord->Fields->GetItem("PID")->Value = (short)atoi(temp);
		}
		if(areaCode != _T(""))
		{
			theRecord->Fields->GetItem("AreaCode")->Value = _bstr_t(areaCode);
		}
		if(population != _T(""))
		{
			theRecord->Fields->GetItem("Population")->Value = _bstr_t(population);
		}
		if(!website.IsEmpty())
		{
			theRecord->Fields->GetItem("WebSite")->Value = _bstr_t(website);
		}

		theRecord->Update();
		theRecord->Close();
	}
	catch(_com_error e)
	{
		throw e;
	}
//	strSQL.Format(_T("insert into Company(CN, IndustryID, PID, Population, AreaCode, WebSite) values (\'%s\', %s, %s, \'%s\', \'%s\', \'%s\')"), companyName, industryID, pid, population, areaCode, website);	
	
//	AfxMessageBox(strSQL);
//	ExcuteSQL(strSQL);
}

//执行SQL更新语句，如插入、更新、删除
inline bool CSearchJobDialog::ExcuteSQL(CString strSQL)
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

int CSearchJobDialog::getCurFileIndex()
{
	fstream fin;
	fin.open("CurFileIndex", ios::in);
	if(!fin)
	{
//		AfxMessageBox(_T("打开文件失败!"));
		return -1;
	}
	int ans;
	fin>>ans;
	fin.close();
	return ans;
}

void CSearchJobDialog::updateCurFileIndex(int index)
{
	fstream fout;
	fout.open("CurFileIndex", ios::out);
	if(!fout)
	{
		AfxMessageBox(_T("打开文件失败!"));
		return ;
	}
	fout<<index<<endl;
	fout.flush();
	fout.close();
}

//分析数据入库
void CSearchJobDialog::OnInportData() 
{
	//得到当前分析到的文件的序号
	int index = getCurFileIndex();
	if(index < 0)
	{
		AfxMessageBox(_T("没有发现新的数据文件，请检查是否有新的文件加入，并且放在正确的目录中!"));
		return ;
	}

	CString fileText = GetFileText(index);

	CString careerName = getCareerName(fileText);
	CString companyName = getCompanyName(fileText);
	int cid = getCompanyIDFromName(companyName);
	if(cid < 0)
	{
		//AfxMessageBox(_T("there"));
		insertACompany(fileText);
		cid = getCompanyIDFromName(companyName);
	}

	CString releaseDate = getReleaseDate(fileText);
	CString needWorker = getNeedWorker(fileText);
	CString workDuration = getWorkDuration(fileText);
	int eid = getNeedEID(fileText);

	CString areaCode = getAreaCode(fileText);
	CString email = getEmail(fileText);
	CString phonenumber = getPhoneNumber(fileText);
	CString cname = getContactName(fileText);

	CString strSQL;
	strSQL.Format(_T("select * from Career"));
	_RecordsetPtr theRecord;
	theRecord.CreateInstance(__uuidof(Recordset));


	char *temp;
	try
	{
		BSTR bstrSQL = strSQL.AllocSysString();
		theRecord->Open(bstrSQL, theApp.m_pConnection.GetInterfacePtr(),
			adOpenDynamic, adLockOptimistic, adCmdText);

		theRecord->AddNew();

		theRecord->Fields->GetItem("CarName")->Value = _bstr_t(careerName);
		//AfxMessageBox(careerName);
		theRecord->Fields->GetItem("CID")->Value = (short)cid;


		if(releaseDate != _T(""))
		{
			theRecord->Fields->GetItem("ReleaseDate")->Value = _bstr_t(releaseDate);
		}

		if(areaCode != _T(""))
		{
			theRecord->Fields->GetItem("AreaCode")->Value = _bstr_t(areaCode);
		}

		if(needWorker != _T(""))
		{
			temp = (char *)needWorker.GetBuffer(needWorker.GetLength());
			theRecord->Fields->GetItem("NeedWorker")->Value = (short)atoi(temp);
		}

		if(workDuration != _T(""))
		{
			theRecord->Fields->GetItem("WorkDuration")->Value = _bstr_t(workDuration);
		}

		if(eid > 0)
		{
			theRecord->Fields->GetItem("EID")->Value = (short)eid;
		}

		if(email != _T(""))
		{
			int index = email.Find(_T(";"));
			if(index > 0)
				email.Delete(index, email.GetLength() - index);
			theRecord->Fields->GetItem("Email")->Value = _bstr_t(email);
		}

		if(phonenumber != _T(""))
		{
			theRecord->Fields->GetItem("PhoneNumber")->Value = _bstr_t(phonenumber);
		}

		if(cname != _T(""))
		{
			theRecord->Fields->GetItem("ContactName")->Value = _bstr_t(cname);
		}

		theRecord->Update();
		theRecord->Close();
//	AfxMessageBox(_T("here"));
//	exit(0);
	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
		return ;
	}
	updateCurFileIndex(index + 1);
//	AfxMessageBox(_T("成功插入一条职位记录!"));
}

//分析出公司规模
CString CSearchJobDialog::getPopulation(CString fileText)
{
	CStringArray aryResult;
	CString regex = _T(">公司规模：</strong>(&nbsp;&nbsp;{[^&nbsp]+})+</");
	BOOL bSuccess = FindPattern((LPCTSTR)fileText, (LPCTSTR)regex, aryResult);
	if (!bSuccess)
	{
		return "";
	}

	CString temp;
	temp = aryResult.GetAt(0);
	return temp;
}

//分析出公司所属行业
CString CSearchJobDialog::getCompanyIndustry(CString fileText)
{
	CStringArray aryResult;
	CString regex = _T(">公司行业：</strong>(&nbsp;&nbsp;{[^&nbsp]+})+</");
	BOOL bSuccess = FindPattern((LPCTSTR)fileText, (LPCTSTR)regex, aryResult);
	if (!bSuccess)
	{
		return "";
	}

	CString temp;
	temp = aryResult.GetAt(0);
	return temp;
}

//分析出公司名称
CString CSearchJobDialog::getCompanyName(CString fileText)
{
	CStringArray aryResult;
	CString regex = _T("【[^，]+，{[^，]+}】");
	BOOL bSuccess = FindPattern((LPCTSTR)fileText, (LPCTSTR)regex, aryResult);
	if (!bSuccess)
	{
		return "";
	}

	CString temp;
	temp = aryResult.GetAt(0);
	return temp;
}

//读取文件内容
CString CSearchJobDialog::GetFileText(int index)
{
	CFile file;
	
	CString strFileName;
	strFileName.Format(_T("JobInfoFiles\\%010d.htm"), index);
	
	// TODO: Add your control notification handler code here
	BOOL bOK=file.Open(strFileName, CFile::modeRead);
	
	CString strContent;
	if (bOK)
	{
		int len=file.GetLength();
		char* buff = (char*)malloc (len+1);
		if (buff)
		{
			file.Read(buff, len);
			buff[len] = '\0';
			
			USES_CONVERSION;
			strContent = A2W(buff);
			
			file.Close();
			//aryResult
		}
		return strContent;
	}

	AfxMessageBox(_T("没有找到指定的文件!"));

	return "";
}

CString CSearchJobDialog::getCareerName(CString fileText)
{
	CStringArray aryResult;
	CString regex = _T("【{[^，]+}，[^，]+】");
	BOOL bSuccess = FindPattern((LPCTSTR)fileText, (LPCTSTR)regex, aryResult);
	if (!bSuccess)
	{
		return "";
	}

	CString temp;
//	for (int i=0;i<aryResult.GetSize();i++)
//	{
		temp = aryResult.GetAt(0);
		return temp;
//		AfxMessageBox(temp);
//	}
}

//根据地区名得到其区号
CString CSearchJobDialog::getAreaCodeFromAreaName(CString areaName)
{
	int index = areaName.Find(_T("-"));
	if(index > 0)
	{
		int len = areaName.GetLength();
		areaName.Delete(index, len - index);
	}

//	index = areaName.Find(_T("市"));

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
		
		varAreaCode = pPtr->GetCollect ("AreaCode");

		strAreaCode =(char *)_bstr_t(varAreaCode);

		pPtr->MoveNext();
	}
	
	pPtr->Close();
	pPtr.Release();
	return strAreaCode;
}

//获取职位的工作地区区号
CString CSearchJobDialog::getAreaCode(CString fileText)
{
	CStringArray aryResult;
	CString regex = _T(">工作地点：</td><td class=\"txt_2\">{[^<]+}</");
	BOOL bSuccess = FindPattern((LPCTSTR)fileText, (LPCTSTR)regex, aryResult);
	if (!bSuccess)
	{
		return "";
	}

	CString temp;
	temp = aryResult.GetAt(0);
	return getAreaCodeFromAreaName(temp);

}

//职位工作年限要求
CString CSearchJobDialog::getWorkDuration(CString fileText)
{
	CStringArray aryResult;
	CString regex = _T(">工作年限：</td><td class=\"txt_2\">{[^<]+}</");
	BOOL bSuccess = FindPattern((LPCTSTR)fileText, (LPCTSTR)regex, aryResult);
	if (!bSuccess)
	{
		return "";
	}

	CString temp;
//	for (int i=0;i<aryResult.GetSize();i++)
//	{
		temp = aryResult.GetAt(0);
		return temp;
//		AfxMessageBox(temp);
//	}
}

//职位招聘人数
CString CSearchJobDialog::getNeedWorker(CString fileText)
{
	CStringArray aryResult;
	CString regex = _T(">招聘人数：</td><td class=\"txt_2\">{\\d+}</");
	BOOL bSuccess = FindPattern((LPCTSTR)fileText, (LPCTSTR)regex, aryResult);
	if (!bSuccess)
	{
		return "";
	}

	CString temp;
//	for (int i=0;i<aryResult.GetSize();i++)
//	{
		temp = aryResult.GetAt(0);
		return temp;
//		AfxMessageBox(temp);
//	}
}

//职位信息发布日期
CString CSearchJobDialog::getReleaseDate(CString fileText)
{
	CStringArray aryResult;
	CString regex = _T(">发布日期：</td><td class=\"txt_2\">{[^<]+}</");
	BOOL bSuccess = FindPattern((LPCTSTR)fileText, (LPCTSTR)regex, aryResult);
	if (!bSuccess)
	{
		return "";
	}

	CString temp;
//	for (int i=0;i<aryResult.GetSize();i++)
//	{
		temp = aryResult.GetAt(0);
		return temp;
//		AfxMessageBox(temp);
//	}
}

//获取职位要求的学历编号
int CSearchJobDialog::getNeedEID(CString fileText)
{
	int eid = -1;

	CStringArray aryResult;
	CString regex = _T(">学(&nbsp;)*历：</td><td class=\"txt_2\">{[^<]+}</");
	BOOL bSuccess = FindPattern((LPCTSTR)fileText, (LPCTSTR)regex, aryResult);
	if (!bSuccess)
	{
		return eid;
	}

	CString temp;
	temp = aryResult.GetAt(0);
	CString strSQL;
	strSQL.Format(_T("select * from Education where Name = \'%s\'"), temp);
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
		return eid;
	}

	if(!pPtr->GetadoEOF())
	{
		_variant_t varEID;
		
		varEID = pPtr->GetCollect ("EID");

		eid =varEID.iVal;

		pPtr->MoveNext();
	}
	
	pPtr->Close();
	pPtr.Release();
	return eid;
}

//获取求职联系邮箱
CString CSearchJobDialog::getEmail(CString fileText)
{
	CStringArray aryResult;
	CString regex = _T(">电子邮箱：</td>[\r\n\t \f]*<[^<>]+>[\r\n\t \f]*<([^<>]|[\r\n\t \f])+>{[^@<>]+@[^<]+}</");
	BOOL bSuccess = FindPattern((LPCTSTR)fileText, (LPCTSTR)regex, aryResult);
	if (!bSuccess)
	{
		return "";
	}

	CString temp;
	temp = aryResult.GetAt(0);
	return temp;
}

//获取公司性质编号
CString CSearchJobDialog::getPID(CString fileText)
{
	CStringArray aryResult;
	CString regex = _T(">公司性质：</strong>&nbsp;&nbsp;{[^<]+}<");
	BOOL bSuccess = FindPattern((LPCTSTR)fileText, (LPCTSTR)regex, aryResult);
	if (!bSuccess)
	{
		return "";
	}

	CString temp;
	temp = aryResult.GetAt(0);

	int index = temp.Find(_T("公司"));
	if(index > 0)
	{
		temp.Delete(index, temp.GetLength() - index);
	}

	CString strSQL;
	strSQL.Format(_T("select * from Property where Name = \'%s\'"), temp);
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
		
		varPID = pPtr->GetCollect ("PID");

		strPID =(char *)_bstr_t(varPID);

		pPtr->MoveNext();
	}
	
	pPtr->Close();
	pPtr.Release();
	return strPID;
}

//获取求职联系电话
CString CSearchJobDialog::getPhoneNumber(CString fileText)
{
	CStringArray aryResult;
	CString regex = _T(">电&nbsp;&nbsp;&nbsp;&nbsp;话：{[^<]+}+</p>");
	BOOL bSuccess = FindPattern((LPCTSTR)fileText, (LPCTSTR)regex, aryResult);
	if (!bSuccess)
	{
		return "";
	}

	CString temp;
	temp = aryResult.GetAt(0);
	return temp;
}

//获取公司主页
CString CSearchJobDialog::getWebSite(CString fileText)
{
	CStringArray aryResult;
	CString regex = _T(">公司网站：<a href=\"{[^\"]+}+\"");
	BOOL bSuccess = FindPattern((LPCTSTR)fileText, (LPCTSTR)regex, aryResult);
	if (!bSuccess)
	{
		return "";
	}

	CString temp;
	temp = aryResult.GetAt(0);
	return temp;
}

//获取求职联系人称呼
CString CSearchJobDialog::getContactName(CString fileText)
{
	CStringArray aryResult;
	CString regex = _T(">联&nbsp;系&nbsp;人：{[^<]+}+</");
	BOOL bSuccess = FindPattern((LPCTSTR)fileText, (LPCTSTR)regex, aryResult);
	if (!bSuccess)
	{
		return "";
	}

	CString temp;
	temp = aryResult.GetAt(0);
	return temp;
}


void WorkResponsibilities (CString fileText)
{
	CStringArray aryResult;
	CString regex = _T("任职资格：([ ]*<BR>{\\d+、[^<>]+})+");
	BOOL bSuccess = FindPattern((LPCTSTR)fileText, (LPCTSTR)regex, aryResult);
	if (!bSuccess)
	{
		AfxMessageBox(_T("fail!"));
		return ;
	}

	CString temp;
	for (int i=0;i<aryResult.GetSize();i++)
	{
		temp = aryResult.GetAt(i);
		AfxMessageBox(temp);
	}
}

//通过行业名称得到行业编号
CString CSearchJobDialog::getIndustryIDFromName(CString indName)
{
	CString strID = _T("");
	indName.TrimRight();
	if(indName.IsEmpty())
		return strID;
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
		return strID;
	}

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

//插入一条新数据至行业表
void CSearchJobDialog::InsertANewIndustry(CString indName)
{
	CString strSQL;
	strSQL.Format(_T("insert into Industry values ('%s')"), indName);	
//	AfxMessageBox(strSQL);
	ExcuteSQL(strSQL);
}

//更新用户信息
void CSearchJobDialog::OnButtonUpdata() 
{
	// TODO: Add your control notification handler code here
	AfxMessageBox(_T("更新用户信息!"));
	return ;
}

//执行查询职位信息的语句，并将结果显示出来
void CSearchJobDialog::SearchJobInDB(CString strSQL)
{
	_RecordsetPtr pPtr;
	_bstr_t bstrSQL(strSQL);
	try
	{
		theApp.pCommand->ActiveConnection = theApp.m_pConnection;
		theApp.pCommand->CommandText=bstrSQL;
		theApp.pCommand->CommandType=adCmdText;
		theApp.pCommand->Parameters->Refresh();
		pPtr = theApp.pCommand->Execute(NULL,NULL,adCmdUnknown);
		if(pPtr->GetadoEOF())
		{
			AfxMessageBox(_T("系统中不存在满足条件的职位记录!"));
		}

		while(!pPtr->GetadoEOF())
		{
			_variant_t varID;
			varID  = pPtr->GetCollect ("CarName");
			CString strID  = (char *)_bstr_t(varID);
			strID.TrimRight();
			AfxMessageBox(strID);
			pPtr->MoveNext();
		}
		pPtr->Close();
		pPtr.Release();
	}
	catch(_com_error e)
	{
		pPtr->Close();
		pPtr.Release();
		throw e;
	}
}

void CSearchJobDialog::InitInfoDlg(CJobInfoDlg &dlg, CString user)
{

	CString strSQL;
	strSQL.Format(_T("select * from JobSeekers where Name = \'%s\'"), user);
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

	if(!pPtr->GetadoEOF())
	{
		CString str;
		_variant_t var;	
		var  = pPtr->GetCollect ("SmtpServer");
		str = (char *)_bstr_t(var);
		str.TrimRight();
		dlg.SmtpServer = str;

		var = pPtr->GetCollect("EuserName");
		str = (char *)_bstr_t(var);
		str.TrimRight();
		dlg.EUserName = str;
		
		var = pPtr->GetCollect("EPassWord");
		str = (char *)_bstr_t(var);
		str.TrimRight();
		dlg.EPassWord = str;
		
		var = pPtr->GetCollect("ContentFileName");
		str = (char *)_bstr_t(var);
		str.TrimRight();
		dlg.ContentFileName = str;
		
		var = pPtr->GetCollect("AttachmentFileName");
		str = (char *)_bstr_t(var);
		str.TrimRight();
		dlg.AttachmentFileName = str;
	
		var = pPtr->GetCollect(_T("SmtpPort"));
		dlg.port = var.iVal;
	}	
	pPtr->Close();
	pPtr.Release();
}



//搜索职位
void CSearchJobDialog::OnSearchJob() 
{
	UpdateData(TRUE);
	CString strSQL = _T("select * from Career where ");
	CString areaName;
	CString temp;
	m_cbRegion.GetWindowText(areaName);
	if(areaName != _T("不限"))
	{
		temp.Format(_T("AreaCode = (select AreaCode from Region where Name = '%s') and "), areaName);
		strSQL += temp;
	}

	m_sJobName.TrimRight();
	if(m_sJobName != _T(""))
	{
//		if(m_fuzzyCareer)
//		{
//			temp.Format(_T("CarName like '%%%s%%' and "), m_sJobName);
//			AfxMessageBox(temp);
//			exit(0);
//		}
//		else
//		{
			temp.Format(_T("CarName = '%s' and "), m_sJobName);
//		}
		strSQL += temp;
	}

	m_sCompanyName.TrimRight();
	if(m_sCompanyName != _T(""))
	{
		if(m_fuzzyCompany)
		{
			temp.Format(_T("CID = (select CID from Company where CN like '%%%s%%') and "), m_sCompanyName);
		}
		else
		{
			temp.Format(_T("CID = (select CID from Company where CN = '%s') and "), m_sCompanyName);
		}
		strSQL += temp;
	}

	if(strSQL.Right(4) == _T("and "))
	{
		strSQL.Delete(strSQL.GetLength() - 4, 4);
	}
	else if(strSQL.Right(6) == _T("where "))
	{
		strSQL.Delete(strSQL.GetLength() - 6, 6);
	}

//	SearchJobInDB(strSQL);
	CJobInfoDlg dlg;
	dlg.strSQL = strSQL;
	dlg.Email = m_sEmail;
	InitInfoDlg(dlg, m_sUserName);
//	this->CloseWindow();
	dlg.DoModal();
//	CSearchJobDialog::OnOK();
}

//初始化省份组合框
void CSearchJobDialog::initProvinceComb()
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

//通过省份名称从数据库中查到省份的编号
CString CSearchJobDialog::getProvinceID(CString proName)
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

void CSearchJobDialog::updateRegion(CString PID)
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

void CSearchJobDialog::OnSelchangeProvince() 
{
	int index = m_cbProvince.GetCurSel();
	if(index == 0)
	{
		m_cbRegion.ResetContent();
		m_cbRegion.AddString(_T("不限"));
		m_cbRegion.SetCurSel(0);
		return ;
	}

	CString proName;
	CString PID;
	m_cbProvince.GetLBText(index, proName);
	PID = getProvinceID(proName);
	updateRegion(PID);	
}

CString CSearchJobDialog::getAreaCodeFromName(CString areaName)
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

void CSearchJobDialog::OnPaint() 
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
		bkg.LoadBitmap(IDB_BITMAP_BGS);//载入位图
		
		BITMAP bkginfo;
		bkg.GetBitmap(&bkginfo);//获取位图信息
		
		memdc.SelectObject(&bkg);
		
		RECT rect;
		GetWindowRect(&rect);//获取对话框信息
		
		dc.StretchBlt(0,0,rect.right-rect.left,rect.bottom-rect.top,&memdc,0,0,bkginfo.bmWidth,bkginfo.bmHeight,SRCCOPY);
 }	// Do not call CDialog::OnPaint() for painting messages
}
