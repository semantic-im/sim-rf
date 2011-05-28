#pragma once


// FeatureDistribution

class FeatureDistribution : public CStatic
{
	DECLARE_DYNAMIC(FeatureDistribution)

public:
	FeatureDistribution();
	virtual ~FeatureDistribution();
	double *pdf;
	int nbValues;
	double maxValue, minValue;
	void allocPDF(int nbV);
	void resetPDF();
	void deletePDF();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};


