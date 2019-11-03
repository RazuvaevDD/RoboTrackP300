#include "stdafx.h"
#include "Core.h"
#include <thread>
#include <boost/signals2.hpp>
#include <iostream>
#include "LSLConnector/LSLConnector.h"
#include "DataHandler/DataHandler.h"
#include "boost/bind.hpp"

std::thread* CoreThread;
bool run = true;

void Core::main(std::thread* coreThread)
{
	LSLConnector LSLInput;
	DataHandler dataHandler;
	//тут связываем LSLInput с dataHandler (сигнал sendData в слот dataProcessing)
	LSLInput.sendData.connect(boost::bind(&DataHandler::dataProcessing, &dataHandler));
	while (run)
	{
		LSLInput.getData();
	}
}

void Core::stopCore()
{
	run = false;
	//Тут действия для завершения работы бэкенда
	//в данном случае - завершение работы цикла
}
