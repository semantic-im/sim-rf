// FeatureDistribution.cpp : implementation file
//

#include "stdafx.h"
#include "relFedLarkc.h"
#include "FeatureDistribution.h"


// FeatureDistribution

IMPLEMENT_DYNAMIC(FeatureDistribution, CStatic)

FeatureDistribution::FeatureDistribution()
{

}

FeatureDistribution::~FeatureDistribution()
{
}


BEGIN_MESSAGE_MAP(FeatureDistribution, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// FeatureDistribution message handlers

void FeatureDistribution::allocPDF(int nbV){
	this->nbValues = nbV;
	pdf = new double[nbV];
	for (int i = 0; i < nbValues; i++)
	{
		pdf[i] = 0;
	}
}

void FeatureDistribution::resetPDF(){
	for (int i = 0; i < this->nbValues; i++)
	{
		pdf[i] = 0;
	}
}

void FeatureDistribution::deletePDF(){
	if (pdf)
		delete [] pdf;
}

void FeatureDistribution::OnPaint()
{
	// TODO: Add your message handler code here
	// Do not call CStatic::OnPaint() for painting messages

	CPaintDC dc(this); // device context-ul pentru afisare
	RECT rectf;
	GetClientRect(&rectf);
	CBrush brush;
	brush.Attach(GetStockObject(LTGRAY_BRUSH));
	dc.FillRect(&rectf, &brush);
	brush.Detach();
	CPen pen(PS_SOLID, 1, RGB(255,0,0)); // definirea pen-ului de desenare cu culoare rosie
	CPen *pTempPen=dc.SelectObject(&pen); // selectarea pen-ului de afisare
	CRect rect;
	GetClientRect(rect); // obtine zona de afisare dreptunghiulara disponibila
	int height=rect.Height(); // inaltimea zonei de afisare
	int width=rect.Width(); // latimea zonei de afisare
	int upperOffset = 10;

	// se determina maximul sirului values[256]
	int i;
	int maxV=0;
	for (i=0;i<nbValues;i++)
		if (pdf[i]>maxV) 
			maxV=pdf[i];

	// se verifica daca este nevoie de scalare

	double scaleFactor=1.0;	
	int heightOffset = 20;
	// facem intotdeauna scalarea; astfel incat la maxValue sa corespunda height;
	scaleFactor=(double)(height-heightOffset)/maxV;	
	int intervalDim = width/(this->nbValues)-1;
	//find the dimension of an interval, and draw rectangles not lines; 

	// se afiseaza pdf (eventual scalata) ca niste bare verticale
	for (i=0;i<nbValues;i++)
	{
		// determinarea lungimii liniei
		int lengthLine=(int)(scaleFactor*pdf[i]);	
		for (int j = i*intervalDim+2; j < i*intervalDim+intervalDim-1; j++)
		{
			//afisarea dreptunghiului
			dc.MoveTo(j,height-heightOffset);
			dc.LineTo(j,height-heightOffset-lengthLine);
		}		
	}
	dc.SelectObject(pTempPen); // restaurarea pen-ului de afisare
#if 0
	CPen pen2(PS_SOLID, 1, RGB(5,0,0)); // definirea pen-ului de desenare cu culoare rosie
	CPen *pTempPen2=dc.SelectObject(&pen2); // selectarea pen-ului de afisare

	dc.MoveTo(0, height-heightOffset);
	dc.LineTo(nbValues*intervalDim, height-heightOffset);
	dc.MoveTo(0, height-heightOffset);
	dc.LineTo(0, 0);

	CPoint pos ;
	int intervalDimReal = (maxValue - minValue)/nbValues;
	CString msg;

	for (i = 0; i < nbValues; i++)
	{
		pos.x = (i+1)*intervalDim - 3*intervalDim/4;
		pos.y = height - heightOffset;
		msg.Format("%d",(i+1)*intervalDimReal/2);
		dc.TextOut(pos.x, pos.y, msg);
		dc.MoveTo(0, height-heightOffset);
		dc.LineTo(nbValues*intervalDim, height-heightOffset);
	}


	dc.SelectObject(pTempPen2); // restaurarea pen-ului de afisare
#endif
}
