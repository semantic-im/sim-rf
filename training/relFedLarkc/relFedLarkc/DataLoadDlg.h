#pragma once
#include "afxwin.h"
#include "DataLoadController.h"
#include "FeatureDistribution.h"

// DataLoadDlg dialog

class DataLoadDlg : public CDialog
{
	DECLARE_DYNAMIC(DataLoadDlg)

public:
	DataLoadDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~DataLoadDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_DATA_LOAD };

protected:
	virtual void DoDataExchange (CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:
	int nbInstancesLoaded;
	int nbFeaturesInstance;
	int indexOffsetFeatureList; //contains the offset for the indices of the features in the list box; counts how many additional rows we have put in the list box before displaying the actual features
	afx_msg void OnBnClickedButtonChooseDataSrc();
	CListBox featuresLoadedListBox;
	CString m_strSelection;
	afx_msg void OnLbnSelchangeListboxFeaturesLoaded();

	DataLoadController *dlc;
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	void displayFeaturesInListBox();
	void setNbFeatures();
	void setNbInstaces();
	CString featureMeanValue;
	CString featureVarianceValue;
	CString featureMinValue;
	CString featureMaxValue;
	FeatureDistribution featureDistributionPlot;
};
