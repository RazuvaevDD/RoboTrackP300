#pragma once

#include <vector>
#include <fstream>

class DataHandler
{
public:
	DataHandler();
	~DataHandler();
	void dataProcessing(std::vector<float>);
private:
	std::ofstream out;
};

