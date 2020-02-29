#include "stdafx.h"
#include "Game.h"

#include <random>
#include <ctime>
#include <thread>
#include <iostream>
#include <string>
#include "Core.h"
/////////////////////////////////////////////////////////////////////////
unsigned short int mode = 1;
unsigned short int targetArrow = 3;
unsigned short int width = 680;
unsigned short int height = 680;

std::string dotTexture = "res/mode_3/dot.png";
std::string arrowTexture = "res/mode_3/arrow.png";

unsigned short int point_0 = 0;
unsigned short int point_1 = 300;
unsigned short int point_2 = 400;
unsigned short int point_3 = 1500;
unsigned short int point_generated = 300;
unsigned short int stats = 0;
/////////////////////////////////////////////////////////////////////////
using namespace Game;

bool runGame = true;

void chooseMode(int mode)
{
	switch (mode) {
	case 1:
		width = 400;
		height = 400;
		arrowTexture = "res/mode_1/arrow.png";
		dotTexture = "res/mode_1/dot.png";
		break;
	case 2:
		width = 400;
		height = 400;
		arrowTexture = "res/mode_2/arrow.png";
		dotTexture = "res/mode_2/dot.png";
		break;
	case 3:
		
		break;
	default:
		break;
	}
}

void Game::main(std::thread* gameThread)
{
	std::mt19937 randomValueGenerator(time(0));
	std::uniform_int_distribution<int> dist(1, 4);

	std::mt19937 randomValueGeneratorForLastPoint(time(0));
	std::uniform_int_distribution<int> dist_2(300, 600);

	chooseMode(mode);

	sf::sleep(sf::milliseconds(2000));
	sf::RenderWindow window(sf::VideoMode(width, height), "Running Duck");
	sf::CircleShape shape(100.f);
	sf::Clock gameClock;
	sf::Clock targetClock;
	sf::Font font;
	font.loadFromFile("res/arialbd.ttf");

	sf::Text text(" ", font, 20);
	text.setFillColor(sf::Color::Black);

	sf::Text uparrow("", font, 100);

	//arrow.setFillColor(sf::Color::Green);

	sf::Text timer("", font, 100);
	timer.setFillColor(sf::Color::White);

	sf::Texture dot;
	if (!dot.loadFromFile(dotTexture))
	{
		// catch exception
		std::cout << "ERROR! Texture missing!" << std::endl;
	}
	sf::Texture arrow;
	if (!arrow.loadFromFile(arrowTexture))
	{
		// catch exception
		std::cout << "ERROR! Texture missing!" << std::endl;
	}
	
	sf::Sprite dot_spr;
	dot_spr.setTexture(dot);
	sf::Sprite arrow_spr;
	arrow_spr.setTexture(arrow);

	int targetTimer = 0;
	int randomValue = 0;
	int randomValueOfPointGenerated = 300;

	//инициализация игры
	while (runGame)
	{
		// Игровой цикл, который можно остановить

		float time = gameClock.getElapsedTime().asMicroseconds(); //дать прошедшее время в микросекундах
		int timeTarget = targetClock.getElapsedTime().asMilliseconds();
		gameClock.restart(); //перезагружает время

		//randomValue = dist(randomValueGenerator);
		//27 62 41 RGB

		//переменная для отрисовки сцен
		int sceneNumber = 0;

		if (timeTarget >= point_0 && timeTarget < point_1)
		{
			sceneNumber = 0;
			stats = 0;
			Core::askStatus(stats);
		}
		else if (timeTarget >= point_1 && timeTarget < point_2)
		{
			if (randomValue == 1)
			{

				sceneNumber = 1;
				//incentiveTarget.setFillColor(sf::Color::Green);
				//nonIncentiveTarget.setFillColor(sf::Color::Black);
				stats = 1;
			}
			else if (randomValue == 2)//if (greenRedTargetStat == false)
			{

				sceneNumber = 2;
				//nonIncentiveTarget.setFillColor(sf::Color::Red);
				//incentiveTarget.setFillColor(sf::Color::Black);
				stats = 2; 
			}
			else if (randomValue == 3)//if (greenRedTargetStat == false)
			{
				sceneNumber = 3;
				//nonIncentiveTarget.setFillColor(sf::Color::Red);
				//incentiveTarget.setFillColor(sf::Color::Black);
				stats = 3;
			}
			else if (randomValue == 4)//if (greenRedTargetStat == false)
			{
				sceneNumber = 4;
				//nonIncentiveTarget.setFillColor(sf::Color::Red);
				//incentiveTarget.setFillColor(sf::Color::Black);
				stats = 4;
			}
			Core::askStatus(stats);
		}
		//МЕЖПРОБНЫЙ СДЕЛАТЬ РАНДОМНЫМ (сделано)
		else if (timeTarget >= point_2 && timeTarget < point_3)
		{
			stats = 0;
			Core::askStatus(stats);
		}
		else if (timeTarget >= point_3 && timeTarget <= point_generated)
		{
			stats = 0;
			Core::askStatus(stats);
		}
		else
		{
			randomValue = dist(randomValueGenerator);
			randomValueOfPointGenerated = dist_2(randomValueGeneratorForLastPoint);
			point_generated = point_3 + randomValueOfPointGenerated;
			targetClock.restart();
		}

		//time = time / 800; //скорость игры
		//std::cout << "time = " << time << std::endl;

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed){
				runGame = false;
				window.close();
			}
		}

		window.clear(sf::Color(0, 0, 0));

			switch (sceneNumber) {
			case 1:

				//left
				dot_spr.setPosition(173, 173);
				window.draw(dot_spr);

				arrow_spr.setColor(sf::Color(128, 128, 128));
				if (mode == 1 && targetArrow != 1) {
					arrow_spr.setColor(sf::Color(0, 0, 128));
				}
				
				arrow_spr.setPosition(23, 173);
				arrow_spr.setRotation(0.f);
				window.draw(arrow_spr);
				break;
			case 2:
				//right
				dot_spr.setPosition(173, 173);
				window.draw(dot_spr);

				if (mode == 1 && targetArrow != 2) {
					arrow_spr.setColor(sf::Color(0, 0, 128));
				}

				arrow_spr.setRotation(180.f);
				arrow_spr.setPosition(377, 227);
				window.draw(arrow_spr);
				break;
			case 3:
				//up
				dot_spr.setPosition(173, 173);
				window.draw(dot_spr);

				if (mode == 1 && targetArrow != 3) {
					arrow_spr.setColor(sf::Color(0, 0, 128));
				}

				arrow_spr.setPosition(227, 23);
				arrow_spr.setRotation(90.f);
				window.draw(arrow_spr);
				break;
			case 4:
				//down
				dot_spr.setPosition(173, 173);
				window.draw(dot_spr);

				if (mode == 1 && targetArrow != 4) {
					arrow_spr.setColor(sf::Color(0, 0, 128));
				}

				arrow_spr.setPosition(173, 377);
				arrow_spr.setRotation(270.f);
				window.draw(arrow_spr);
				break;
			default:
				//noarr
				arrow_spr.setColor(sf::Color(128, 128, 128));
				dot_spr.setPosition(173, 173);
				window.draw(dot_spr);
				break;
			}

		window.display();
	}
	// Здесь удаление динамических объектов
}

void Game::stopGame() // Вызывается когда программа закрывается
{
	runGame = false;
	//Тут действия для завершения работы игры
	//в данном случае - завершение работы цикла
}
