#pragma once


// DataPreprocessingDlg dialog

class DataPreprocessingDlg : public CDialog
{
	DECLARE_DYNAMIC(DataPreprocessingDlg)

public:
	DataPreprocessingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~DataPreprocessingDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_DATA_PREPROCESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
