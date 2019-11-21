#pragma once

#include <vector>
#include <fstream>
#include <chrono>

class DataHandler
{
public:
	DataHandler();
	~DataHandler();
	std::vector<std::vector<float>> dataProcessing(std::vector<float>);
	void startCreateVectorProba();

	int SIZE_PRESHOW = 25;
	int SIZE_PROBA   = 80;

	std::ofstream out;
private:
	
	std::chrono::steady_clock::time_point t1; //время начала работы модуля
	std::vector< std::vector<float>> vectorProb;

	bool CREATING_PROBA = false;
};

