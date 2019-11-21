#pragma once

#include <vector>
#include <fstream>
#include <chrono>

class DataHandler
{
public:
	DataHandler();
	~DataHandler();
	std::vector<float> dataProcessing(std::vector<float>);
private:
	std::ofstream out;
	std::chrono::steady_clock::time_point t1; //время начала работы модуля
};

