// MachineLearningDlg.cpp : implementation file
//

#include "stdafx.h"
#include "relFedLarkc.h"
#include "MachineLearningDlg.h"

// MachineLearningDlg dialog

IMPLEMENT_DYNAMIC(MachineLearningDlg, CDialog)

MachineLearningDlg::MachineLearningDlg(CWnd* pParent /*=NULL*/)
	: CDialog(MachineLearningDlg::IDD, pParent)
	, m_Results(_T(""))
	, m_PredictedAttribute(_T(""))
	, m_ClusteringRegression(FALSE)
	, m_trainingFeatures(_T(""))
	, m_predictedFeature(_T(""))
{
	kernel = NULL;
}

MachineLearningDlg::~MachineLearningDlg()
{
	delete kernel;
}

void MachineLearningDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_RESULTS, m_Results);
	DDX_Text(pDX, IDC_STATIC_ATTRIBUTE, m_PredictedAttribute);
	DDX_Check(pDX, IDC_CHECK_CLUSTER_REGRESSION, m_ClusteringRegression);
	DDX_Text(pDX, IDC_EDIT_TRAINING, m_trainingFeatures);
	DDX_Text(pDX, IDC_EDIT_PREDICTED_FEATURE, m_predictedFeature);
}


BEGIN_MESSAGE_MAP(MachineLearningDlg, CDialog)
	ON_BN_CLICKED(IDC_CHECK_CLUSTER_REGRESSION, &MachineLearningDlg::OnBnClickedCheckClusterRegression)
	ON_BN_CLICKED(IDC_BUTTON_RESULTS, &MachineLearningDlg::OnBnClickedButtonResults)
//	ON_EN_CHANGE(IDC_EDIT_TRAINING, &MachineLearningDlg::OnEnChangeEditTraining)
//	ON_EN_CHANGE(IDC_EDIT_PREDICTED_FEATURE, &MachineLearningDlg::OnEnChangeEditPredictedFeature)
ON_EN_KILLFOCUS(IDC_EDIT_TRAINING, &MachineLearningDlg::OnEnKillfocusEditTraining)
ON_EN_KILLFOCUS(IDC_EDIT_PREDICTED_FEATURE, &MachineLearningDlg::OnEnKillfocusEditPredictedFeature)
ON_EN_SETFOCUS(IDC_EDIT_TRAINING, &MachineLearningDlg::OnEnSetfocusEditTraining)
ON_EN_SETFOCUS(IDC_EDIT_PREDICTED_FEATURE, &MachineLearningDlg::OnEnSetfocusEditPredictedFeature)
END_MESSAGE_MAP()


// MachineLearningDlg message handlers

void MachineLearningDlg::OnBnClickedCheckClusterRegression()
{
	
	UpdateData(TRUE);

	CrelFedLarkcDlg* mainDlg = (CrelFedLarkcDlg*)AfxGetApp()->GetMainWnd();

	if (m_ClusteringRegression)
	{		
		//first check if any features are selected or if any data is loaded
		if (!mainDlg->m_dLoadDlg.dlc->loadStatus)
		{
			AfxMessageBox(CString("Please load the training data first!"));
			m_ClusteringRegression=!m_ClusteringRegression;
		}
		
		else
		{
			DataLoadCSVParser *inputData = mainDlg->m_dLoadDlg.dlc->dlcsvp;
			
			int f[100];
			
			bool validFeaturesIdxs=true;

			int curPos = 0;
			int idx=0;
			CString tbuf=m_trainingFeatures;
			CString resToken= tbuf.Tokenize(_T(",; "),curPos);
			while (resToken != _T(""))
			{
				f[idx++]=atoi((LPCSTR)resToken);
				if (f[idx-1]<0 || f[idx-1]>=(int)(inputData->features.size()))
					validFeaturesIdxs=false;
				resToken = tbuf.Tokenize(_T(",; "), curPos);
			}; 

			int noFeatures=idx;

			int classFeature=atoi((LPCSTR)m_predictedFeature);

			if (classFeature<0 || classFeature>=(int)(inputData->features.size()))
				validFeaturesIdxs=false;

			/*	KernelRegression k("D:/data.txt",6,3);
				k.LoadInputData();
				k.ComputeKernelAndWeights();
				k.PredictTestInstances("D:/ti.txt","D:/out.txt");
			*/
			
			if (noFeatures >0 && m_predictedFeature.GetLength()>0 && validFeaturesIdxs)
			{
				kernel = new KernelRegression(inputData,f,noFeatures,classFeature);
				m_PredictedAttribute=kernel->className;
				kernel->ComputeKernelAndWeights();

				CButton *pb=(CButton*)GetDlgItem(IDC_BUTTON_RESULTS);
				pb->EnableWindow(TRUE);	
			}
			else
			{
				CString msg;
				msg.Format("Please input valid indexes (0 to %d) for training features and class feature!",inputData->features.size()-1);
				AfxMessageBox(msg);
				m_ClusteringRegression=false;
			}
			
		}
	}
	else
	{
		CButton *pb=(CButton*)GetDlgItem(IDC_BUTTON_RESULTS);
		pb->EnableWindow(FALSE);	
		m_Results="";
		m_PredictedAttribute="";
		delete kernel;
		kernel=NULL;
	}
	
	UpdateData(FALSE);
}


void MachineLearningDlg::OnBnClickedButtonResults()
{
	CFileDialog dlg(TRUE, _T("*.csv"), NULL, OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY, _T("CSV files (*.csv; ) |*.csv;|All Files (*.*)|*.*||"),NULL); 
	//CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_FILEMUSTEXIST, "Text Files(*.txt)|*.txt||",this);
	dlg.m_ofn.lpstrTitle= _T("Open CSV File "); 
	CString fileName ;
	if (dlg.DoModal() == IDOK) { 
		fileName= dlg.GetPathName();  // contain the selected filename		

		DataLoadController testDataController;
		testDataController.setCSVFileName(fileName);
		testDataController.parseFile(fileName); // we have the feature names and their values; we should put them in the features list box
		CString result=kernel->PredictTestInstances(testDataController.dlcsvp);
		m_Results=result;
		UpdateData(FALSE);
	}
}

BOOL MachineLearningDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CButton *pb=(CButton*)GetDlgItem(IDC_BUTTON_RESULTS);
	pb->EnableWindow(FALSE);	
	return TRUE;
}



void MachineLearningDlg::OnEnKillfocusEditTraining()
{
}

void MachineLearningDlg::OnEnKillfocusEditPredictedFeature()
{
}

void MachineLearningDlg::OnEnSetfocusEditTraining()
{
	UpdateData(TRUE);
	m_ClusteringRegression=false;
	UpdateData(FALSE);
	OnBnClickedCheckClusterRegression();
}

void MachineLearningDlg::OnEnSetfocusEditPredictedFeature()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_ClusteringRegression=false;
	UpdateData(FALSE);
	OnBnClickedCheckClusterRegression();
}
