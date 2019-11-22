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

std::vector<std::vector<float>> DataHandler::dataProcessing(std::vector<float> sample)
{
	//----------------------------------------------------------
	// поток для записи

	
	std::chrono::steady_clock::time_point t2 = std::chrono::high_resolution_clock::now();

	// floating-point duration: no duration_cast needed
	std::chrono::duration<double, std::milli> fp_ms = t2 - t1;

	float timeNOW = fp_ms.count();






	//out << timeNOW << ' ';
	//for (std::size_t i = 0; i < sample.size(); ++i) {
	//	out << sample[i] << ' ';
	//}
	//out << std::endl;
	
	if (vectorProb.empty()) {
		out << "Создаем потоки проб № ";
		for (int i = 0; i < sample.size()+2; i++) {
			std::vector<float> proba;
			vectorProb.push_back(proba);
			out << i << ' ';
		}
		out << std::endl << "Вектора созданы." << std::endl;
	}
	

	
	vectorProb[0].push_back(timeNOW);
	vectorProb[1].push_back(STATUS);
	for (int i = 0; i < sample.size(); i++) 
	{
		vectorProb[i + 2].push_back(sample[i]);
	}

	float oldPackageTime = vectorProb[0][0];


	if ((timeNOW - SIZE_PRESHOW > oldPackageTime) && (!CREATING_PROBA)) // если мы не накапливаем пробу и накопились старые пакеты удаляем старые пакеты
	{
		int i = 0;
		while (i<vectorProb[0].size()) {
			if (timeNOW - SIZE_PRESHOW > vectorProb[0][i]) {
				i++;
			}
			else {

				break;
			}
		}

		// Удаляем старые пакеты до iтого элемента включительно
		for (int j = 0; j < vectorProb.size(); j++) {
			vectorProb[j].erase(vectorProb[j].begin(), vectorProb[j].begin() + i);
		}
	}

	if (CREATING_PROBA) { // если мы создаем пробу
		if (timeNOW - oldPackageTime >= SIZE_PROBA) {// и она создалась
			//удаляем последний элемент
			for (int j = 0; j < vectorProb.size(); j++) {
				vectorProb[j].erase(vectorProb[j].end()-1);
			}
			CREATING_PROBA = false;//отмечаем что мы теперь перестали наращивать пробу
			STATUS = 0; // обнуляем статус
			return vectorProb;
		}
	}



	//out << std::endl;
	//out << std::endl;
	
	std::vector<std::vector<float>> emptyVectProb;
	return emptyVectProb;
}

void DataHandler::startCreateVectorProba()
{
	CREATING_PROBA = true;
}

void DataHandler::setStatusPicture(int status)
{
	if ((STATUS == 0) && (status != 0)) { // если мы начали показывать любую картинку
		startCreateVectorProba();         // начинаем записывать пробу
		STATUS = status;
	}
}
