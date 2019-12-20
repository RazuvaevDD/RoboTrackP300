#pragma once

#include <vector>
#include <fstream>
#include <chrono>
#include <windows.h>

class DataHandler
{
public:
	DataHandler();
	~DataHandler();
	std::vector<std::vector<float>> dataProcessing(std::vector<float>);
	void startCreateVectorProba();

	double SIZE_PRESHOW = 0.250;
	double SIZE_PROBA   = 2.000;
	int Hz = 250;

	std::ofstream out;
	void setStatusPicture(int status);
private:
	
	std::vector< std::vector<float>> vectorProb;

	int STATUS = 0;
	//int ccccc, qq = 0;

	bool CREATING_PROBA = false;
};

