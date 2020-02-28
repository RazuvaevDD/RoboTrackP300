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

	double SIZE_PRESHOW = 0.3;//sec
	double SIZE_PROBA   = 1.5;
	int Hz = 250;

	std::ofstream out;
	std::ofstream dataOut, eventOut;
	void setStatusPicture(int status);
private:
	
	std::vector< std::vector<float>> vectorProb;
	
	int STATUS = 0;
	int DataTick = 0;

	bool CREATING_PROBA = false;
};

