#include "StdAfx.h"
#include "KernelRegression.h"

#include "cv.h"
#include <cxcore.h>
#include <highgui.h>
#include <math.h>
#include "dataLoadProcessing/dataLoadCSVParser.h"

KernelRegression::KernelRegression(char * fileName, int rows, int xVars): rows(rows), xVars(xVars)
{
	strcpy(inputDataFile,fileName);
	X = cvCreateMat(rows,xVars,CV_32FC1);
	Y = cvCreateMat(rows,1,CV_32FC1);
	alfa = cvCreateMat(rows,1,CV_32FC1);
}


KernelRegression::KernelRegression(DataLoadCSVParser * inputData, int *trainFeatures, int noFeatures, int classFeature)
{
	xVars=noFeatures;
	this->noFeatures=noFeatures;
	rows=inputData->instances.size();

	this->trainFeatures=new int[noFeatures];
	memcpy(this->trainFeatures,trainFeatures,noFeatures*sizeof(int));

	this->classFeature=classFeature;
	className=inputData->features.at(classFeature)->getFeatureName();

	X = cvCreateMat(rows,xVars,CV_32FC1);
	Y = cvCreateMat(rows,1,CV_32FC1);
	alfa = cvCreateMat(rows,1,CV_32FC1);


	std::vector<std::string> crtInstance;
	
	for (int i = 0; i < rows; i++)
	{
		crtInstance = inputData->instances.at(i);
		for (int j = 0; j<noFeatures; j++)
		{
			std::string crtVal = crtInstance.at(trainFeatures[j]);
			double crtV = atof(crtVal.c_str());
			cvSetReal2D(X, i, j, crtV);
		}
		std::string crtVal = crtInstance.at(classFeature);
		double crtV = atof(crtVal.c_str());
		cvSetReal2D(Y, i, 0, crtV);
	}
}


KernelRegression::~KernelRegression(void)
{
	cvReleaseMat(&X);
	cvReleaseMat(&Y);
	cvReleaseMat(&alfa);
	delete [] trainFeatures;
}


void KernelRegression::LoadInputData()
{
	FILE * f = fopen(inputDataFile,"r");

	/*
	* Read the initial matrix from the file.
	*/
	//rows
	int i = 0;

	//array to save the current line
	char line[1000];
	int nLength = 0;

	//current value read
	double value = 0;

	while (i < rows)
	{
		//read the current line
		fgets(line,1000,f);
		nLength = (int)strlen(line);
		if (line[nLength-1] == '\n') 
			line[nLength-1] = '\0';
	
		int jX = 0;
		char * tok = strtok(line, ",");
		while (tok != NULL) 
		{	
			if (jX < xVars)
			{	
				cvSetReal2D(X, i, jX, atof(tok));
				jX++;
			}
			else
			{
				cvSetReal2D(Y, i, 0, atof(tok));
			}
			tok = strtok(NULL,",");
		}		
		i++;
	}
	
	fclose(f);
}

void KernelRegression::ComputeKernelAndWeights()
{
	/*
	* Compute the kernel matrix.
	*/
	CvMat* G = cvCreateMat(rows, rows, CV_32FC1);

	CvMat* arrX1 = cvCreateMat(1, X->cols, CV_32FC1);
	CvMat* arrX2 = cvCreateMat(1, X->cols, CV_32FC1);

	//G
	for (int i = 0; i < rows; i++)
	{
		for (int j = i; j < rows; j++) 
		{
			cvGetRow(X,arrX1,i);
			cvGetRow(X,arrX2,j);
			
			double dist=VectorsDistance(arrX1,arrX2);
			cvSetReal2D(G,i,j,dist);
			cvSetReal2D(G,j,i,dist);
		}
	}

	/*
	* Shift the diagonal of the kernel matrix.
	*/
	double k = 1.0/1000.0;
	for (int i = 0; i < G->rows; i++)
		cvSetReal2D(G,i,i,cvGetReal2D(G,i,i)+k);

	/*
	* Find the transpose of the shifted kernel matrix.
	*/
	CvMat* GInverse = cvCreateMat(rows,rows,CV_32FC1);

	cvInvert(G, GInverse, CV_SVD);

	/*
	* Compute alfa.
	*/

	cvMatMul(GInverse,Y,alfa);

	cvReleaseMat(&G);
	cvReleaseMat(&arrX1);
	cvReleaseMat(&arrX2);
	cvReleaseMat(&GInverse);
}


void KernelRegression::PredictTestInstances(char* inputFileName, char* outputFileName)
{
	/*
	* New test instance.
	*/
	FILE * f = fopen(inputFileName,"r");
	FILE * g = fopen(outputFileName,"w");

	CvMat* testInstance = cvCreateMat(1,xVars,CV_32FC1);

	//array to save the current line
	char line[1000];
	int nLength = 0;

	int zz = 1;
	while (fgets(line,1000,f))
	{
		nLength = (int) strlen(line);
		if (line[nLength-1] == '\n') 
			line[nLength-1] = '\0';

		int jX = 0;

		char * tok = strtok(line, ",");
		while (tok != NULL) 
		{	
			if (jX < xVars)
			{	
				cvSetReal2D(testInstance, 0, jX, atof(tok));
				jX++;
			}
			tok = strtok(NULL,",");
		}		
		double result = ComputeEstimatedValue(testInstance);
		
		fprintf(g,"\nResult for %d = %lg\n",zz,result);
		zz++;
	}

	fclose(f);
	fclose(g);
	cvReleaseMat(&testInstance);
}


CString KernelRegression::PredictTestInstances(DataLoadCSVParser * testData)
{
	/*
	* New test instance.
	*/
	
	CString result="";
	
	CvMat* testInstance = cvCreateMat(1,xVars,CV_32FC1);

	std::vector<std::string> crtInstance;

	for (int i = 0; i < rows; i++)
	{
		crtInstance = testData->instances.at(i);
		for (int j = 0; j<noFeatures; j++)
		{
			std::string crtVal = crtInstance.at(trainFeatures[j]);
			double crtV = atof(crtVal.c_str());
			cvSetReal2D(testInstance, 0, j, crtV);
		}

		double r = ComputeEstimatedValue(testInstance);

		std::string crtVal = crtInstance.at(classFeature);
		double crtV = atof(crtVal.c_str());

		result.AppendFormat("**Instance no %3d** Measured:%10.2f | Predicted:%10.2f\r\n",i,crtV,r);
	}

	cvReleaseMat(&testInstance);
	return result;
}


double KernelRegression::VectorsDistance(CvMat *x1, CvMat* x2)
{
	double norm = 0;
	for (int i = 0; i < x1->cols; i++)
	{
		double v1=cvGetReal2D(x1,0,i);
		double v2=cvGetReal2D(x2,0,i);
		norm += (v1 - v2)*(v1 - v2);
	}
	return exp((double)(-norm)/1000000);
}


double KernelRegression::ComputeEstimatedValue(CvMat* testInstance)
{
	double val = 0.0;

	//compute the similarity matrix between the new instance and each datapoint
	double* k = new double[rows];
	CvMat* arrX = cvCreateMat(1, X->cols, CV_32FC1);

	for (int i = 0; i < rows; i++)
	{
		cvGetRow(X,arrX,i);
		k[i] = VectorsDistance(testInstance,arrX);
	}

	for (int i = 0; i < rows; i++)
	{
		val = val + cvGetReal2D(alfa,i,0)*k[i];
	}
	
	delete [] k;
	
	return val;
}


