// grep.cpp

#include "stdafx.h"
#include <atlconv.h>

typedef BOOL (*FINDPATTERN)(LPCWSTR szInput, LPCWSTR szPattern, int* pMatchCount, LPWSTR* pMatchResult);
typedef BOOL (*FREESTRING)(LPCWSTR* pString);

HINSTANCE		s_hGrepInst = NULL;

FINDPATTERN		pfnFindPattern = NULL;
FREESTRING		pfnFreeString = NULL;

CString GetExePath()
{
	TCHAR strExePath[MAX_PATH];     
	GetModuleFileName(NULL,strExePath,MAX_PATH);   

	CString strRet;
	strRet.Format(_T("%s"), strExePath);

	return strRet;
}


CString GetAppPath()
{
	CString strPath = GetExePath();
	int nIndex = strPath.ReverseFind('\\');
	
	strPath = strPath.Left(nIndex+1);

	return strPath;
}

BOOL Grep_Initialize()
{
	CString strPath;
	strPath.Format(_T("%sgrep.dll"), GetAppPath());

	s_hGrepInst = LoadLibrary(strPath);
	if (!s_hGrepInst)
	{
		return FALSE; 
	}

	pfnFindPattern = (FINDPATTERN)GetProcAddress(s_hGrepInst, "FindPattern");
	if (!pfnFindPattern)
	{
		return FALSE;
	}

	pfnFreeString = (FREESTRING)GetProcAddress(s_hGrepInst, "FreeString");
	if (!pfnFreeString)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL Grep_UnInitialize()
{
	BOOL bRet = FALSE;

	if (s_hGrepInst)
	{
		FreeLibrary(s_hGrepInst);	
		s_hGrepInst = NULL;

		bRet = TRUE;
	}

	return bRet;
}

BOOL FindPattern(LPCWSTR szIn, LPCWSTR szPattern, CStringArray& aryRet)
{
	BOOL bOK = FALSE;

	aryRet.RemoveAll();

	if (NULL==pfnFindPattern || NULL==pfnFreeString)
		return FALSE;

	int nMatchCount = 0;
	LPWSTR wResult = NULL;
	bOK = pfnFindPattern(szIn, szPattern, &nMatchCount, &wResult);
	if (bOK)
	{
		CString strResult = wResult;
		CString strLen;
		CString str;
		int len = 0;
		for (int i=0;i<nMatchCount;i++)
		{
			if (strResult.GetLength()>=8)
			{
				strLen = strResult.Left(8);
				strResult = strResult.Right(strResult.GetLength()-8);
				len = _ttol(strLen);
				if (len>0)
				{
					str = strResult.Left(len);
					strResult = strResult.Right(strResult.GetLength()-len);
					
					aryRet.Add(str);
				}
			}
		}
	}

	if (wResult)
	{
		pfnFreeString((LPCWSTR*)(&wResult));
	}

	bOK = (aryRet.GetSize())>0;

	return bOK;
}

