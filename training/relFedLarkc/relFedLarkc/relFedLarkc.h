// relFedLarkc.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "relFedLarkcDlg.h"


// CrelFedLarkcApp:
// See relFedLarkc.cpp for the implementation of this class
//

class CrelFedLarkcApp : public CWinApp
{
public:
	CrelFedLarkcApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CrelFedLarkcApp theApp;
