// relFedLarkcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "relFedLarkc.h"
#include "relFedLarkcDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CrelFedLarkcDlg dialog




CrelFedLarkcDlg::CrelFedLarkcDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CrelFedLarkcDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CrelFedLarkcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_cTab);
}

BEGIN_MESSAGE_MAP(CrelFedLarkcDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CrelFedLarkcDlg::OnTcnSelchangeTab1)
END_MESSAGE_MAP()


// CrelFedLarkcDlg message handlers

BOOL CrelFedLarkcDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);  // Set big icon
	SetIcon(m_hIcon, FALSE); // Set small icon

	CRect tabRect;

	m_cTab.GetWindowRect(tabRect);

	// Set the size and location of the child windows based on the tab control

	m_rSettingsRect.left = 13;
	m_rSettingsRect.top = 44;
	m_rSettingsRect.right = tabRect.Width() - 7;
	m_rSettingsRect.bottom = tabRect.Height() - 38;

	// Create the child windows for the main window class

	m_dLoadDlg.Create(IDD_DIALOG_DATA_LOAD, this);
	m_dPreprocessDlg.Create(IDD_DIALOG_DATA_PREPROCESS, this);
	m_dMLDlg.Create(IDD_DIALOG_MACHINE_LEARNING, this);
	m_dResultsDlg.Create(IDD_DIALOG_RESULTS, this);
	
	// This is redundant with the default value, considering what OnShowWindow does

	ShowWindowNumber(0);

	// Set the titles for each tab

	TCITEM tabItem;
	tabItem.mask = TCIF_TEXT;
	tabItem.pszText = _T("  Load   ");
	m_cTab.InsertItem(0, &tabItem);

	tabItem.pszText = _T("  Preprocess   ");
	m_cTab.InsertItem(1, &tabItem);

	tabItem.pszText = _T("  Machine Learning   ");
	m_cTab.InsertItem(2, &tabItem);


	tabItem.pszText = _T("  Results   ");
	m_cTab.InsertItem(3, &tabItem);

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	//add initialization for the list box dialog 
	m_dLoadDlg.featuresLoadedListBox.SetColumnWidth(100);
	
	//initialize the controller associated to each dialog	

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CrelFedLarkcDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CrelFedLarkcDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CrelFedLarkcDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CrelFedLarkcDlg::ShowWindowNumber(int number){
	// This example uses four windows
	int windowCount = 4;

	// Validate the parameter
	if ((number >= 0) && (number < windowCount))
	{
		// Create and assign pointers to each window
		CDialog *m_dPointer[4];

		m_dPointer[0] = &m_dLoadDlg;
		m_dPointer[1] = &m_dPreprocessDlg;
		m_dPointer[2] = &m_dMLDlg;
		m_dPointer[3] = &m_dResultsDlg;
		
		// Hide every window except for the chosen one
		for (int count = 0; count < windowCount; count++)
		{
			if (count != number)
			{
				m_dPointer[count]->ShowWindow(SW_HIDE);
			}
			else //if (count == number)
			{
				// Show the chosen window and set it's location
				m_dPointer[count]->SetWindowPos(&wndTop, m_rSettingsRect.left,
					m_rSettingsRect.top, m_rSettingsRect.right,
					m_rSettingsRect.bottom, SWP_SHOWWINDOW);

				m_cTab.SetCurSel(count);
			}
		}
	}
}

void CrelFedLarkcDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// When the dialog is shown, display the first window

	if (bShow)
	{
		ShowWindowNumber(0);
	}
}

void CrelFedLarkcDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// Get the number of the currently selected tab, and show its window

	ShowWindowNumber(m_cTab.GetCurFocus());

	// Do something with the "formal parameters" so 

	// the compiler is happy in warning level 4

	pNMHDR = NULL;
	pResult = NULL;
}
