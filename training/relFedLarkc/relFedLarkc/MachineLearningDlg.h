#pragma once

#include "KernelRegression.h"

// MachineLearningDlg dialog

class MachineLearningDlg : public CDialog
{
	DECLARE_DYNAMIC(MachineLearningDlg)

public:
	MachineLearningDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~MachineLearningDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_MACHINE_LEARNING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckClusterRegression();
	afx_msg void OnBnClickedButtonResults();
	CString m_Results;
	CString m_PredictedAttribute;
	BOOL m_ClusteringRegression;
private:
	KernelRegression *kernel;
public:
	CString m_trainingFeatures;
	CString m_predictedFeature;
//	afx_msg void OnEnChangeEditTraining();
//	afx_msg void OnEnChangeEditPredictedFeature();
	afx_msg void OnEnKillfocusEditTraining();
	afx_msg void OnEnKillfocusEditPredictedFeature();
	afx_msg void OnEnSetfocusEditTraining();
	afx_msg void OnEnSetfocusEditPredictedFeature();
};
