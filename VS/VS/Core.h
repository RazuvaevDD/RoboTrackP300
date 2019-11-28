#pragma once

#include <thread>

namespace Core
{
	void main(std::thread*);
	void stopCore();
	void askStatus(int);

	//std::thread* CoreThread;
	
};
