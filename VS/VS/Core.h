#pragma once

#include <thread>

namespace Core
{
	void main(std::thread*);
	void stopCore();

	//std::thread* CoreThread = NULL;
};
