// DataLoadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "relFedLarkc.h"
#include "DataLoadDlg.h"


// DataLoadDlg dialog

IMPLEMENT_DYNAMIC(DataLoadDlg, CDialog)

DataLoadDlg::DataLoadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(DataLoadDlg::IDD, pParent)
	, nbInstancesLoaded(0)
	, nbFeaturesInstance(0)
	, m_strSelection(_T(""))
	, featureMeanValue(_T(""))
	, featureVarianceValue(_T(""))
	, featureMinValue(_T(""))
	, featureMaxValue(_T(""))
	, indexOffsetFeatureList(0)
{
	dlc = new DataLoadController();
}

DataLoadDlg::~DataLoadDlg()
{
	delete dlc;
}

void DataLoadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, nbInstancesLoaded);
	DDX_Text(pDX, IDC_EDIT2, nbFeaturesInstance);
	DDX_Control(pDX, IDC_LISTBOX_FEATURES_LOADED, featuresLoadedListBox);
	DDX_LBString(pDX, IDC_LISTBOX_FEATURES_LOADED, m_strSelection);
	DDX_Text(pDX, IDC_EDIT_MEAN, featureMeanValue);
	DDX_Text(pDX, IDC_EDIT_VARIANCE_VAL, featureVarianceValue);
	DDX_Text(pDX, IDC_EDIT_MIN_VAL, featureMinValue);
	DDX_Text(pDX, IDC_EDIT_MAX_VAL, featureMaxValue);
	DDX_Control(pDX, IDC_FEATURE_PLOT, featureDistributionPlot);
}


BEGIN_MESSAGE_MAP(DataLoadDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_CHOOSE_DATA_SRC, &DataLoadDlg::OnBnClickedButtonChooseDataSrc)
	ON_LBN_SELCHANGE(IDC_LISTBOX_FEATURES_LOADED, &DataLoadDlg::OnLbnSelchangeListboxFeaturesLoaded)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


void DataLoadDlg::setNbFeatures(){
	nbFeaturesInstance = dlc->dlcsvp->features.size();	
}

void DataLoadDlg::setNbInstaces(){
	nbInstancesLoaded = dlc->dlcsvp->instances.size();	
}

// DataLoadDlg message handlers
void DataLoadDlg::displayFeaturesInListBox(){
	CString strN;
	CString strT;
	CString str;

	str.Format("-----------------------------------------------");
	featuresLoadedListBox.AddString(str);
	this->indexOffsetFeatureList++;

	str.Format("FEATURE_NAME [FEATURE_TYPE]");
	featuresLoadedListBox.AddString(str);
	this->indexOffsetFeatureList++;
	
	str.Format("-----------------------------------------------");
	featuresLoadedListBox.AddString(str);
	this->indexOffsetFeatureList++;

	Feature *f;
	for (int i = 0; i < dlc->dlcsvp->features.size(); i++)
	{
		f = dlc->dlcsvp->features.at(i);
		strN = f->getFeatureName();

		f = dlc->dlcsvp->features.at(i);
		strT = f->getFeatureType();

		str.Format("%d> %s [%s]", i, strN, strT);
		featuresLoadedListBox.AddString(str);
	}	
}

void DataLoadDlg::OnBnClickedButtonChooseDataSrc()
{	
	//opens a file chooser dialog box; puts the name of the file in the list box, plus some test values

	CFileDialog dlg(TRUE, _T("*.csv"), NULL, OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY, _T("CSV files (*.csv; ) |*.csv;|All Files (*.*)|*.*||"),NULL); 
	//CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_FILEMUSTEXIST, "Text Files(*.txt)|*.txt||",this);
	dlg.m_ofn.lpstrTitle= _T("Open CSV File "); 
	CString fileName ;
	if (dlg.DoModal() == IDOK) { 
		fileName= dlg.GetPathName();  // contain the selected filename		
		this->UpdateWindow();
	}

	dlc->setCSVFileName(fileName);
	dlc->parseFile(fileName); // we have the feature names and their values; we should put them in the features list box
	displayFeaturesInListBox();
	setNbFeatures();
	setNbInstaces();	
	//put the values in the list box
	//featuresLoadedListBox.SetColumnWidth(2);

	UpdateData(FALSE);
}

void DataLoadDlg::OnLbnSelchangeListboxFeaturesLoaded()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CString m_strReport;
	int idx = featuresLoadedListBox.GetCurSel();

	m_strReport.Format( "The selection in List has changed to index %d containing %s", idx,  m_strSelection);
	if ((idx - this->indexOffsetFeatureList) >= 0)
	{
		//display the mean , variance, min value, max value for the selected featur
		this->featureMeanValue = dlc->dlcsvp->getMeanValueForFeature(idx - this->indexOffsetFeatureList);
		this->featureVarianceValue = dlc->dlcsvp->getVarianceValueForFeature(idx - this->indexOffsetFeatureList);
		this->featureMinValue = dlc->dlcsvp->getMinValueForFeature(idx - this->indexOffsetFeatureList);
		this->featureMaxValue = dlc->dlcsvp->getMaxValueForFeature(idx - this->indexOffsetFeatureList);
		this->featureDistributionPlot.resetPDF();
		//dispaly the distribution of the features - if these are numeric
		//dlc->dlcsvp->computeDistributionForFeature(idx - this->indexOffsetFeatureList, this->featureDistributionPlot.nbValues, this->featureDistributionPlot.pdf, this->featureDistributionPlot.minValue, this->featureDistributionPlot.maxValue);
	}	
	else{
		this->featureMeanValue = _T("NO");
		this->featureVarianceValue = _T("NO");
		this->featureMinValue = _T("NO");
		this->featureMaxValue =  _T("NO");
	}	

	UpdateData(FALSE);
	featureDistributionPlot.InvalidateRect(0);
}

BOOL DataLoadDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	int nbBins = 10;
	this->featureDistributionPlot.allocPDF(nbBins+1);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void DataLoadDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
	this->featureDistributionPlot.deletePDF();
}
