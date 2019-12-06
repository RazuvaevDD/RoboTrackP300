#include "stdafx.h"
#include "Core.h"
#include <thread>
#include <iostream>
#include "LSLConnector/LSLConnector.h"
#include "DataHandler/DataHandler.h"
#include "tcpSender.h"

using namespace Core;

bool runCore = true;

LSLConnector LSLInput;
DataHandler dataHandler;
tcpSender tcpSender;

void Core::main(std::thread* coreThread)
{
	while (runCore)
	{
		std::vector<std::vector<float>> vectorProb = dataHandler.dataProcessing(LSLInput.getData());
		
		if (!vectorProb.empty()) 
		{//если проба готова
			tcpSender.sendDataByTCP(vectorProb);
			dataHandler.out << "Итоговые вектора:" << std::endl;
			for (std::vector<float> proba : vectorProb) 
			{
				dataHandler.out << "Вектор:" << std::endl;
				for (float data : proba) 
				{
					dataHandler.out << data << ' ';
				}
				dataHandler.out << std::endl;
				if(proba.size()!=200)
					dataHandler.out <<"ОШИБКА:"<< proba.size() <<" элементов"<< std::endl;
			}
		}
	}
}

void Core::stopCore()
{
	runCore = false;
	//Тут действия для завершения работы бэкенда
	//в данном случае - завершение работы цикла
}

void Core::askStatus(int stat) {
	//dataHandler.out << "СТАТА = " << stat << "\n";
	dataHandler.setStatusPicture(stat);
}