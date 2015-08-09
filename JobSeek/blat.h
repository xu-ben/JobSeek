// blat.h : header file
//

#if !defined(_Blat_H_)
#define _Blat_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Stdafx.h"			// IMPORTANT: Include this file in each file of project

typedef enum _tagEmailType
{
	EmailType_PlainText = 0, 
	EmailType_HTML = 1

}EmailType;

int SendEmail(LPCTSTR szEmailTo, 
			  LPCTSTR szSmtpServer, UINT uSmtpPort, 
			  LPCTSTR szSmtpServerEmail, LPCTSTR szReplyToEmail, 
			  LPCTSTR szUserName, LPCTSTR szPassword,
			  LPCTSTR szEmailTitle, LPCTSTR szContentFile, CStringArray& aryAttachmentFile,
			  LPCTSTR szSinatureFile=_T(""),
			  EmailType emailType=EmailType_HTML, LPCTSTR szHtmlAlternativeFilePath=_T(""),
			  BOOL bRequireReceipt=FALSE, BOOL bMIME=FALSE, LPCTSTR szEncoding=_T("utf-8"), BOOL bHighPriority=FALSE,
			  LPCTSTR szOtherParameters=_T(""));

BOOL CreateChildProcess(LPCTSTR szCommandLine, DWORD dwWaitMillSecs);

#endif // !defined(_Blat_H_)
