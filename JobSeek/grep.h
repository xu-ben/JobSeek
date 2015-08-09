// grep.h

#ifndef _GREP_H_
#define _GREP_H_

#include "stdafx.h"

BOOL Grep_Initialize();
BOOL Grep_UnInitialize();

BOOL FindPattern(LPCWSTR szIn, LPCWSTR szPattern, CStringArray& aryRet);


#endif // _GREP_H_
