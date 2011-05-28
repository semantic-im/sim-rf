#include "StdAfx.h"
#include "DataLoadController.h"

DataLoadController::DataLoadController(void)
{
	dlcsvp = new DataLoadCSVParser();
	loadStatus = false;
}

DataLoadController::~DataLoadController(void)
{
	delete dlcsvp;
}

void DataLoadController::setCSVFileName(CString fn){
	this->CSVFileName = fn;
}
CString DataLoadController::getCSVFileName(){
	return this->CSVFileName;
}
void DataLoadController::parseFile(CString fn){
	//initialize the parser and call the parse function :)	
	dlcsvp->parseFile2(fn);
	loadStatus=true;
}

int DataLoadController::getNbInstances(){
	return this->nbInstances;
}

void DataLoadController::setNbInstances(int nbi){
	this->nbInstances = nbi;
}

int DataLoadController::getNbFeatures(){
	return this->nbFeatures;
}

void DataLoadController::setNbFeatures(int nbf){
	this->nbFeatures = nbf;
}