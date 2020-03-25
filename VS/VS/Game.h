#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>

namespace Game
{
	void main(std::thread*);
	void stopGame();
	void input();

	//std::thread* GameThread;
};
