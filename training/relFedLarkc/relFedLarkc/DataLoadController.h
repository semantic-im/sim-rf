#pragma once
#include "dataLoadProcessing/dataLoadCSVParser.h"
class DataLoadController
{

private:
	// Data can be stored in CSV format or 
	// we may give as data source a .txt file that contains db connection information: host, username, password
	CString dbConnectionFileName;
	CString CSVFileName;
	int nbInstances;
	int nbFeatures;	

public:
	DataLoadCSVParser *dlcsvp;
	bool loadStatus;

	void setCSVFileName(CString fn);
	CString getCSVFileName();
	void parseFile(CString fn);
	int getNbInstances();
	void setNbInstances(int);
	int getNbFeatures();
	void setNbFeatures(int);

	DataLoadController(void);
	~DataLoadController(void);

};
