#include "stdafx.h"
#include "Game.h"

#include <thread>
#include <iostream>

using namespace Game;

bool runGame = true;

void Game::main(std::thread* gameThread)
{
	//инициализация игры
	while (runGame)
	{
		std::cout << "";
		 // Игровой цикл, который можно остановить
	}
	// Здесь удаление динамических объектов
}

void Game::stopGame() // Вызывается когда программа закрывается
{
	runGame = false;
	//Тут действия для завершения работы игры
	//в данном случае - завершение работы цикла
}
