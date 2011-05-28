#pragma once


// ResultsDlg dialog

class ResultsDlg : public CDialog
{
	DECLARE_DYNAMIC(ResultsDlg)

public:
	ResultsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~ResultsDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_RESULTS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
