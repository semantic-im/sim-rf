// relFedLarkcDlg.h : header file
//

#pragma once

#include "DataLoadDlg.h"
#include "DataPreprocessingDlg.h"
#include "MachineLearningDlg.h"
#include "ResultsDlg.h"
#include "afxcmn.h"

// CrelFedLarkcDlg dialog
class CrelFedLarkcDlg : public CDialog
{
// Construction
public:
	CrelFedLarkcDlg(CWnd* pParent = NULL);	// standard constructor

	DataLoadDlg m_dLoadDlg;
	DataPreprocessingDlg m_dPreprocessDlg;
	MachineLearningDlg m_dMLDlg;
	ResultsDlg m_dResultsDlg;

	void ShowWindowNumber(int number);

// Dialog Data
	enum { IDD = IDD_RELFEDLARKC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	CRect m_rSettingsRect;
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	CTabCtrl m_cTab;
};
