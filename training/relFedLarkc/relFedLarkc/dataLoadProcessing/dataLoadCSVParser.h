#pragma once

#include "Feature.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

class DataLoadCSVParser
{

private:
	// Data can be stored in CSV format or 
	// we may give as data source a .txt file that contains db connection information: host, username, password

	CString dbConnectionFileName;
	CString CSVFileName;
	
public:
	std::vector<Feature*> features;
	std::vector<std::vector<std::string> > instances;

	void setCSVFileName(CString fn);
	CString getCSVFileName();
	void parseFile2(CString fn);
	void csvline_populate(std::vector<std::string> &record, const std::string& line, char delimiter);
	CString getMeanValueForFeature(int index);
	CString getVarianceValueForFeature(int index);
	CString getMinValueForFeature(int index);
	CString getMaxValueForFeature(int index);
	double *computeDistributionForFeature(int index, int nbBins);

	DataLoadCSVParser(void);
	~DataLoadCSVParser(void);
};