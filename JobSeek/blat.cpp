// blat.cpp : implementation file
//

#include "stdafx.h"
#include "blat.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
#endif


BOOL IsFileExist(LPCTSTR szFullPath)
{
	BOOL bExist = FALSE;

#ifdef _UNICODE
	if (_waccess(szFullPath, 0) == 0)
#else
	if (_access(szFullPath, 0) == 0)
#endif
	{
		bExist = TRUE;
	}

	return bExist;
}

CString GetExePath2()
{
	TCHAR strExePath[MAX_PATH];     
	GetModuleFileName(NULL,strExePath,MAX_PATH);   

	CString strRet;
	strRet.Format(_T("%s"), strExePath);

	return strRet;
}


CString GetAppPath2()
{
	CString strPath = GetExePath2();
	int nIndex = strPath.ReverseFind('\\');
	
	strPath = strPath.Left(nIndex+1);

	return strPath;
}

CString Blat_GetExePath()
{
	CString strPath;
	strPath.Format(_T("\"%sblat.exe\""), GetAppPath2());

	return strPath;
}

BOOL IsFileContentUnicode(LPCTSTR szFile)
{
	BOOL bUnicode = FALSE;

	CFile file;
	if (file.Open(szFile, CFile::modeRead))
	{
		int nLen = file.GetLength();
		if (nLen>=2)
		{
			WORD uUnicodeMagic;
			file.Read(&uUnicodeMagic, sizeof(WORD)/sizeof(byte));
			
			if (uUnicodeMagic==0xFEFF || uUnicodeMagic==0xFFFE)
				bUnicode = TRUE;
		}

		file.Close();
	}
	
	return bUnicode;	
}

BOOL CreateChildProcess(LPCTSTR szCommandLine, DWORD dwWaitMillSecs) 
{ 
	PROCESS_INFORMATION piProcInfo; 
	STARTUPINFO siStartInfo; 
	
	// Set up members of the PROCESS_INFORMATION structure. 
	
	ZeroMemory( &piProcInfo, sizeof(PROCESS_INFORMATION) );
	
	// Set up members of the STARTUPINFO structure. 
	
	ZeroMemory( &siStartInfo, sizeof(STARTUPINFO) );
	siStartInfo.cb = sizeof(STARTUPINFO); 
	
	// Create the child process. 
	
	BOOL bRet = CreateProcess(NULL, 
		(LPTSTR)szCommandLine, // command line 
		NULL,          // process security attributes 
		NULL,          // primary thread security attributes 
		FALSE,         // handles are inherited 
		CREATE_NO_WINDOW,             // creation flags 
		NULL,          // use parent's environment 
		NULL,          // use parent's current directory 
		&siStartInfo,  // STARTUPINFO pointer 
		&piProcInfo);  // receives PROCESS_INFORMATION 

//	DWORD dw = GetLastError();

	if (dwWaitMillSecs != 0)
	{
		WaitForSingleObject(piProcInfo.hProcess, dwWaitMillSecs);
	}

	CloseHandle(piProcInfo.hThread);
	CloseHandle(piProcInfo.hProcess);

	return bRet;
}

// 利用Blat.exe发送邮件
int SendEmail(LPCTSTR szEmailTo, 
				   LPCTSTR szSmtpServer, UINT uSmtpPort,
				   LPCTSTR szSmtpServerEmail, LPCTSTR szReplyToEmail,  
				   LPCTSTR szUserName, LPCTSTR szPassword,
				   LPCTSTR szEmailTitle, LPCTSTR szContentFile, CStringArray& aryAttachmentFile,
				   LPCTSTR szSinatureFile,
				   EmailType emailType, LPCTSTR szHtmlAlternativeFilePath,
				   BOOL bRequireReceipt, BOOL bMIME, LPCTSTR szEncoding, BOOL bHighPriority,
				   LPCTSTR szOtherParameters)
{
	int ret = 1;

	// blat body.txt -to fu-xu@163.com -f abc@gmail.com -server 127.0.0.1 -port 25 -u textcapture -pw 790711

	CString strBasePara;
	strBasePara.Format(_T(" -to %s -f %s -i %s -server %s -port %u -u \"%s\" -pw \"%s\" "), 
		szEmailTo, szSmtpServerEmail, szSmtpServerEmail, szSmtpServer, uSmtpPort, szUserName, szPassword);

	CString strEmailTypePara;
	if (emailType == EmailType_PlainText)
	{
		strEmailTypePara = _T(" ");
	}
	else if (emailType == EmailType_HTML)
	{
		if (IsFileExist(szHtmlAlternativeFilePath))
		{
			strEmailTypePara.Format(_T(" -html -alttextf \"%s\" "), szHtmlAlternativeFilePath);
		}
		else
		{
			strEmailTypePara = _T(" -html -alttext \"The email content is in HTML format. Please switch to HTML view to see the content.\" ");
		}
	}

	CString strReceiptPara = _T(" ");
	if (bRequireReceipt)
	{
		strReceiptPara = _T(" -r ");
	}

	CString strMimePara = _T(" ");
	if (bMIME)
	{
		strMimePara = _T(" -mime ");
	}

	CString strCharsetPara = _T(" -charset \"ISO-8859-1\" ");
	if (_tcslen(szEncoding)>0)
	{
		strCharsetPara.Format(_T(" -charset \"%s\" "), szEncoding);
	}

	CString strPriorityPara;
	if (bHighPriority)
	{
		strPriorityPara.Format(_T(" -priority %u "), 1);
	}
	else
	{
		strPriorityPara.Format(_T(" -priority %u "), 0);
	}

	CString strReplyToPara;
	if (_tcslen(szReplyToEmail)==0)
	{
		strReplyToPara.Format(_T(" -replyto %s "), szSmtpServerEmail);
	}
	else
	{
		strReplyToPara.Format(_T(" -replyto %s "), szReplyToEmail);
	
	}

	CString strTitlePara = _T(" ");
	strTitlePara.Format(_T(" -subject \"%s\" "), szEmailTitle);

	CString strBodyPara = _T(" ");
	if (IsFileExist(szContentFile))
	{
		strBodyPara.Format(_T(" -bodyF \"%s\" "), szContentFile);

		if (IsFileContentUnicode(szContentFile))
			strBodyPara += _T(" -unicode ");
	}

	CString strSinaturePara = _T(" ");
	if (IsFileExist(szSinatureFile))
	{
		strSinaturePara.Format(_T(" -sig \"%s\" "), szSinatureFile);
	}

	CString strAttachmentPara = _T(" ");
	CString strTempAttach;
	int nFilesCount = aryAttachmentFile.GetSize();
	if (nFilesCount>0)
	{
		strAttachmentPara = _T(" \"");

		for (int i=0;i<nFilesCount;i++)
		{
			strTempAttach.Format(_T("%s"), aryAttachmentFile.GetAt(i));

			strAttachmentPara += strTempAttach;

			if (i<nFilesCount-1)
				strAttachmentPara += _T(",");
		}

		strAttachmentPara += _T("\" ");
	
		strTempAttach.Format(_T(" -attach %s "), strAttachmentPara);

		strAttachmentPara = strTempAttach;
	}

	CString strOtherPara = _T(" ");
	if (szOtherParameters)
	{
		strOtherPara.Format(_T(" %s "), szOtherParameters);
	}
	
	CString strParameter = strBasePara+strEmailTypePara+strReceiptPara+
		strMimePara+strCharsetPara+strPriorityPara+strReplyToPara+
		strTitlePara+strBodyPara+strSinaturePara+strAttachmentPara+strOtherPara;

	CString strCommand;
	strCommand.Format(_T("%s%s"), Blat_GetExePath(), strParameter);

#ifdef _DEBUG
	AfxMessageBox(strCommand);
#endif
	
	CreateChildProcess(strCommand, INFINITE);

	return ret;
}
