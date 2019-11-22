#include "stdafx.h"
#include "Core.h"
#include <thread>
#include <iostream>
#include "LSLConnector/LSLConnector.h"
#include "DataHandler/DataHandler.h"
#include "tcpSender.h"

using namespace Core;

bool runCore = true;

void Core::main(std::thread* coreThread)
{
	LSLConnector LSLInput;
	DataHandler dataHandler;
	tcpSender tcpSender;

	//тут связываем LSLInput с dataHandler (сигнал sendData в слот dataProcessing)
	//LSLInput.sendData.connect(boost::bind(&DataHandler::dataProcessing, &dataHandler));
	int kk = 0;
	while (runCore)
	{
		std::vector<std::vector<float>> vectorProb = dataHandler.dataProcessing(LSLInput.getData());
		
		if (!vectorProb.empty()) {//если проба готова
			tcpSender.sendDataByTCP(vectorProb);
			dataHandler.out << "Итоговые вектора:" << std::endl;
			for (std::vector<float> proba : vectorProb) {
				for (float data : proba) {
					dataHandler.out << data << ' ';
				}
				dataHandler.out << std::endl;
			}
			
			kk = 0;
		}

		if (kk == 500)
			dataHandler.startCreateVectorProba();
		kk++;
	}
}

void Core::stopCore()
{
	runCore = false;
	//Тут действия для завершения работы бэкенда
	//в данном случае - завершение работы цикла
}
