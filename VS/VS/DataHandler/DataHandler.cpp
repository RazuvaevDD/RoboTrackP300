#include "stdafx.h"
#include "DataHandler.h"


DataHandler::DataHandler()
{
	out.open("log.log");        // окрываем файл для записи
	if (out.is_open())
	{
		out << "Log started!" << std::endl;
	}
}


DataHandler::~DataHandler()
{
	out.close();
}

void DataHandler::dataProcessing(std::vector<float> sample)
{
	//----------------------------------------------------------
	// поток для записи
	for (std::size_t i = 0; i < sample.size(); ++i) {
		out << sample[i] << ' ';
	}
	out << std::endl;
	//----------------------------------------------------------

	//printf("123");
	//тут мой (Данин) обработчик
}
