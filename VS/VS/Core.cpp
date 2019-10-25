#include "stdafx.h"
#include "Core.h"
#include <thread>
#include <boost/signals2.hpp>
#include <iostream>

std::thread* CoreThread;
bool run = true;


class Example
{
public:
	boost::signals2::signal<void()> onShit;
};

void Core::main(std::thread* coreThread)
{
	while (run)//т.к. второй поток, шраф интерфейс не виснит
	printf( "Ядро запущено! Этот поток отвечает за бэкенд." );	// проблема в выводе в консоль, аналог не нашел. Прога сюда доходит.
}

void Core::stopCore()
{
	run = false;
	std::cout << "Тут действия для завершения работы бэкенда";	// странно что в этом потоке не работет вывод в консоль, 
																//что делать - потом подумаю
}
