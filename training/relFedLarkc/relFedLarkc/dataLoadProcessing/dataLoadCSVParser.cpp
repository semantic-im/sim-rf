#include "StdAfx.h"
#include "DataLoadCSVParser.h"
#include "../include/csv_parser/csv_parser.hpp"

DataLoadCSVParser::DataLoadCSVParser(void)
{
	
}

DataLoadCSVParser::~DataLoadCSVParser(void)
{
	for (int i=0; i<features.size(); i++)
		delete features.at(i);
}

void DataLoadCSVParser::parseFile2(CString fn){
	//parses a file

	/* Declare the variables to be used */
	USES_CONVERSION;
	const char *filename = T2A(fn.GetBuffer());	

	Feature *f;
	vector<string> row;
	string line;
	ifstream in(filename);
	if (in.fail())  { cout << "File not found" <<endl; return ; }

	int crtLine = 0;
	while(getline(in, line)  && in.good() )
	{
		csvline_populate(row, line, ',');
		//if line is 0 we have the feature names
		if (row.size()>0 && crtLine == 0)
		{
			for(int i=0, leng=row.size(); i<leng; i++){			
				f = new Feature();
				f->setFeatureName(row[i].c_str());
				this->features.push_back(f);
			}
			crtLine ++;
		}

		//if line is 1 we have the feature types
		else if (row.size()>0 && crtLine == 1)
		{
			for (unsigned int i = 0; i < row.size(); i++)
			{
				CString crt = row[i].c_str();			
				f = features.at(i);
				if(crt.MakeLower() == "string")
					f->setFeatureType("string");
				if(crt.MakeLower() == "numeric")
					f->setFeatureType("numeric");
				if(crt.MakeLower() == "date")
					f->setFeatureType("date");
				if(crt.MakeLower() == "category")
					f->setFeatureType("category");
			}
			crtLine ++;
		}
		else {
			//we have read the first two linew, so crtLine is not interesting for us anymore
			this->instances.push_back(row);
		}
	}
	in.close();
}

void DataLoadCSVParser::csvline_populate(std::vector<std::string> &record, const std::string& line, char delimiter){
	int linepos=0;
	int inquotes=false;
	char c;
	char fieldTerm = '"';
	int i;
	int linemax=line.length();
	std::string curstring;
	record.clear();

	while(line[linepos]!=0 && linepos < linemax)
	{

		c = line[linepos];

		if (!inquotes && curstring.length()==0 && c==fieldTerm)
		{
			//beginquotechar
			inquotes=true;
		}
		else if (inquotes && c=='"')
		{
			//quotechar
			if ( (linepos+1 <linemax) && (line[linepos+1]==fieldTerm) ) 
			{
				//encountered 2 double quotes in a row (resolves to 1 double quote)
				curstring.push_back(c);
				linepos++;
			}
			else
			{
				//endquotechar
				inquotes=false; 
			}
		}
		else if (!inquotes && c==delimiter)
		{
			//end of field
			record.push_back( curstring );
			curstring="";
		}
		else if (!inquotes && (c=='\r' || c=='\n') )
		{
			record.push_back( curstring );
			return;
		}
		else
		{
			curstring.push_back(c);
		}
		linepos++;
	}
	record.push_back( curstring );
	return;
}

CString DataLoadCSVParser::getMeanValueForFeature(int index){
	CString result;
	double mean = 0;
	std::string crtVal;
	std::vector<std::string> crtInstance;
	if (features.at(index)->getFeatureType() == "numeric")
	{
		//iterate over the instances and get the mean value at index
		for (int i = 0; i < this->instances.size(); i++)
		{
			crtInstance = this->instances.at(i);
			crtVal = crtInstance.at(index);
			mean += atof(crtVal.c_str());
		}
		mean /= this->instances.size();
		result.Format("%lf", mean);
	}
	else 
		result.Format("NO");
	return result;
}

CString DataLoadCSVParser::getVarianceValueForFeature(int index){
	CString result;
	double mean = atof(this->getMeanValueForFeature(index));
	double variance = 0;
	std::string crtVal;
	std::vector<std::string> crtInstance;
	if (features.at(index)->getFeatureType() == "numeric")
	{
		//iterate over the instances and get the mean value at index
		for (int i = 0; i < this->instances.size(); i++)
		{
			crtInstance = this->instances.at(i);
			crtVal = crtInstance.at(index);
			variance += pow(atof(crtVal.c_str()) - mean, 2);			
		}
		variance = sqrt(variance);
		result.Format("%lf", variance);
	}
	else 
		result.Format("NO");
	return result;
}
CString DataLoadCSVParser::getMinValueForFeature(int index){
	CString result;
	double minVal = INT_MAX;
	std::string crtVal;
	std::vector<std::string> crtInstance;
	if (features.at(index)->getFeatureType() == "numeric")
	{
		//iterate over the instances and get the mean value at index
		for (int i = 0; i < this->instances.size(); i++)
		{
			crtInstance = this->instances.at(i);
			crtVal = crtInstance.at(index);
			if (minVal > atof(crtVal.c_str()))
				minVal = atof(crtVal.c_str());
		}
		
		result.Format("%lf", minVal);
	}
	else 
		result.Format("NO");
	return result;
}
CString DataLoadCSVParser::getMaxValueForFeature(int index){
	CString result;
	double maxVal = INT_MIN;
	std::string crtVal;
	std::vector<std::string> crtInstance;
	if (features.at(index)->getFeatureType() == "numeric")
	{
		//iterate over the instances and get the mean value at index
		for (int i = 0; i < this->instances.size(); i++)
		{
			crtInstance = this->instances.at(i);
			crtVal = crtInstance.at(index);
			if (maxVal < atof(crtVal.c_str()))
				maxVal = atof(crtVal.c_str());
		}

		result.Format("%lf", maxVal);
	}
	else 
		result.Format("NO");
	return result;
}

double *DataLoadCSVParser::computeDistributionForFeature(int index, int nbBins){	
	//divide the interval defined by minValue and MaxValue of the current feature into nbBins+1 intervals;
	//nbBins+1 because we want to put the values equal to max in the last interval !
	//the length of an interval will be (maxV-minV)/nbBins;
	//compute pdf only if data is numeric
	if (features.at(index)->getFeatureType() == "numeric")
	{
		std::string crtVal;
		std::vector<std::string> crtInstance;
		double *featurePDF = new double [nbBins+1];
		
		double mean = atof(this->getMeanValueForFeature(index));
		double minV = atof(this->getMinValueForFeature(index));
		double maxV = atof(this->getMaxValueForFeature(index));
		double crtV;
		double intervalDim = (maxV-minV)/nbBins;
		for (int k = 0; k <= nbBins; k++)
			featurePDF[k] = 0;

		//iterate over the instances and get the mean value at index
		for (int i = 0; i < this->instances.size(); i++)
		{
			crtInstance = this->instances.at(i);
			crtVal = crtInstance.at(index);
			crtV = atof(crtVal.c_str());
			for (int k = 0; k <= nbBins; k++)
			{
				if ( crtV >= k*intervalDim && crtV < (k+1)*intervalDim )
					featurePDF[k] += 1;
			}
		}		
		return featurePDF;
	}
	return NULL;
}
