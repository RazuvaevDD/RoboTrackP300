#include "stdafx.h"
#include "DataHandler.h"



DataHandler::DataHandler()
{
	out.open("log.log");        // окрываем файл для записи
	if (out.is_open())
	{
		out << "Log started!" << std::endl;
	}
	t1 = std::chrono::high_resolution_clock::now();
}

DataHandler::~DataHandler()
{
	out.close();
}

std::vector<float> DataHandler::dataProcessing(std::vector<float> sample)
{
	//----------------------------------------------------------
	// поток для записи

	
	std::chrono::steady_clock::time_point t2 = std::chrono::high_resolution_clock::now();

	// floating-point duration: no duration_cast needed
	std::chrono::duration<double, std::milli> fp_ms = t2 - t1;

	float ms = fp_ms.count();






	out << ms << ' ';
	for (std::size_t i = 0; i < sample.size(); ++i) {
		out << sample[i] << ' ';
	}
	out << std::endl;
	
	//тут мой (Данин) обработчик

	return sample;
}
