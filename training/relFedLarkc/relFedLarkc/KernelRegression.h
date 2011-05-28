#pragma once

#include "cv.h"
#include <cxcore.h>
#include "dataLoadProcessing/dataLoadCSVParser.h"

class KernelRegression
{
private:
	char inputDataFile[256];
	int rows;
	int xVars;
	int *trainFeatures;
	int noFeatures;
	int classFeature;
	CvMat* X;
	CvMat* Y;
	CvMat* alfa;

	double VectorsDistance(CvMat *x1, CvMat* x2);
	double ComputeEstimatedValue(CvMat* testInstance);
public:
	CString className;
	KernelRegression(char * fileName, int rows, int xVars);
	KernelRegression(DataLoadCSVParser * inputData, int *v, int noFeatures, int classFeature);
	~KernelRegression(void);
	void LoadInputData();
	void ComputeKernelAndWeights();
	void PredictTestInstances(char* inputFileName, char* outputFileName);
	CString PredictTestInstances(DataLoadCSVParser * testData);
};
