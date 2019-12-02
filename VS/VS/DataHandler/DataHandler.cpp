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

std::vector<std::vector<float>> DataHandler::dataProcessing(std::vector<float> sample)
{
	if (vectorProb.empty()) {
		out << "Создаем потоки проб № ";
		for (int i = 0; i < sample.size()+1; i++) {
			std::vector<float> proba;
			vectorProb.push_back(proba);
			out << i << ' ';
		}
		out << std::endl << "Вектора созданы." << std::endl;
	}
	//if (ccccc == 0) {
	//	int qq = -GetTickCount();
	//}
	//if (ccccc == 200) {
	//	qq += GetTickCount();
	//	ccccc = 0;
	//	//out << "200 пакетов получены за (время):" << qq << "\n"; 
	//}

	vectorProb[0].push_back(STATUS);
	for (int i = 0; i < sample.size(); i++) 
	{
		vectorProb[i + 1].push_back(sample[i]);
	}
	//for (std::vector<float> proba : vectorProb) {
	//	out << "Проба\n";
	//	for (float data : proba) {
	//		out << data << ' ';
	//	}
	//}

	if ((vectorProb[0].size() > SIZE_PRESHOW*Hz) && (!CREATING_PROBA)) // если мы не накапливаем пробу и накопились старые пакеты удаляем старые пакеты
	{
		// Удаляем старые пакеты до iтого элемента включительно
		for (int j = 0; j < vectorProb.size(); j++) {
			//out << "Удаляем от 0 до "<< (int)(vectorProb[j].size() - SIZE_PRESHOW * Hz + 1) <<" элемента включительно\n";
			vectorProb[j].erase(vectorProb[j].begin(), vectorProb[j].begin() + (int) (vectorProb[j].size() - SIZE_PRESHOW * Hz + 1));
		}
	}

	//ccccc++;
	
	if (CREATING_PROBA) { // если мы создаем пробу
		if (vectorProb[0].size() >= SIZE_PROBA*Hz) {// и она создалась
			CREATING_PROBA = false;//отмечаем что мы теперь перестали наращивать пробу
			STATUS = 0; // обнуляем статус
			out << "Окончили записывать пробу.\n";// В пробе "<<vectorProb[0].size()<<" элементов.\n";
			return vectorProb;
		}
	}

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
		STATUS = status;
		startCreateVectorProba();         // начинаем записывать пробу
		out << "Начали накапливать пробу..." << std::endl;
	}
}
