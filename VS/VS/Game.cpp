#include "stdafx.h"
#include "Game.h"

#include <random>
#include <ctime>
#include <thread>
#include <iostream>
#include <string>
#include "Core.h"

using namespace Game;

bool runGame = true;

void Game::main(std::thread* gameThread)
{
	std::mt19937 randomValueGenerator(time(0));
	std::uniform_int_distribution<int> dist(1, 5);

	sf::sleep(sf::milliseconds(2000));
	sf::RenderWindow window(sf::VideoMode(1440, 480), "Running Duck");
	sf::CircleShape shape(100.f);
	sf::Clock gameClock;
	sf::Clock targetClock;
	sf::Font font;
	font.loadFromFile("res/arialbd.ttf");

	sf::Text text(" ", font, 20);
	text.setFillColor(sf::Color::Black);

	sf::Text incentiveTarget("", font, 100);
	sf::Text nonIncentiveTarget("", font, 100);
	incentiveTarget.setString(">>");
	nonIncentiveTarget.setString("X");

	//arrow.setFillColor(sf::Color::Green);

	sf::Text timer("", font, 100);
	timer.setFillColor(sf::Color::White);

	sf::Texture duckTexture;
	if (!duckTexture.loadFromFile("res/DuckTile.png"))
	{
		// catch exception
		std::cout << "ERROR! Texture missing!" << std::endl;
	}

	sf::Texture backgroundTexture;
	if (!backgroundTexture.loadFromFile("res/backgroundTexture.png"))
	{
		// catch exception
		std::cout << "ERROR! Texture missing!" << std::endl;
	}

	sf::Sprite groundSprite;
	groundSprite.setTexture(backgroundTexture);
	groundSprite.setScale(5, 5);

	/* coords:
	 * 0) default - 0,0
	 * 1) squinted - 44,0
	 * 2) closed eyes - 88,0
	 * 3) looks away 1 - 132,0
	 * 4) looks away 2 - 176,0
	 * 5) step 1 - 220,0
	 * 6) step 2 - 264,0
	 */

	sf::Sprite duckSprite;
	duckSprite.setTexture(duckTexture);
	duckSprite.setTextureRect(sf::IntRect(0, 0, 44, 44));
	duckSprite.setPosition(50, 50);
	duckSprite.setScale(5, 5);

	bool step = 0;
	bool isRun = false;
	bool closedEyes = false;
	bool looksAway = false;

	int groundCoord = 260;

	double offset = 0.2;
	double currentIsRunFrame = 5;
	double currentClosedEyesFrame = 0;
	double currentLooksAwayFrame = 0;

	double score = 0;
	std::string strScore;

	int targetTimer = 0;
	unsigned short int stats;
	//bool greenRedTargetStatFlag = true;
	int randomValue = 0;

	//инициализация игры
	while (runGame)
	{
		 // Игровой цикл, который можно остановить

		float time = gameClock.getElapsedTime().asMicroseconds(); //дать прошедшее время в микросекундах
		int timeTarget = targetClock.getElapsedTime().asMilliseconds();
		gameClock.restart(); //перезагружает время
		
		//randomValue = dist(randomValueGenerator);

		if (timeTarget >= 0 && timeTarget< 250)
		{
			//arrow.setString("");
			incentiveTarget.setFillColor(sf::Color::Black);
			nonIncentiveTarget.setFillColor(sf::Color::Black);
			stats = 0;
			Core::askStatus(stats);
		} 
		else if (timeTarget >= 250 && timeTarget < 400)
		{
			if (randomValue == 1)
			{
				// целевой
				incentiveTarget.setFillColor(sf::Color::Green);
				nonIncentiveTarget.setFillColor(sf::Color::Black);
				stats = 1;
			}
			else if (randomValue == 2 || randomValue == 3 || randomValue == 4 || randomValue == 5)//if (greenRedTargetStat == false)
			{
				// не целевой
				nonIncentiveTarget.setFillColor(sf::Color::Red);
				incentiveTarget.setFillColor(sf::Color::Black);
				stats = 2;
			}
			Core::askStatus(stats);
		}
		else if (timeTarget >= 400 && timeTarget < 1400)
		{
			incentiveTarget.setFillColor(sf::Color::Black);
			nonIncentiveTarget.setFillColor(sf::Color::Black);
			stats = 0;
			Core::askStatus(stats);
		}
		else
		{/*
			if (greenRedTargetStat == true)
			{
				greenRedTargetStat = false;
			}
			else if (greenRedTargetStat == false)
			{
				greenRedTargetStat = true;
			}*/
			randomValue = dist(randomValueGenerator);
			incentiveTarget.setFillColor(sf::Color::Black);
			nonIncentiveTarget.setFillColor(sf::Color::Black);
			targetClock.restart();
		}

		//time = time / 800; //скорость игры
		//std::cout << "time = " << time << std::endl;

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// вызов функции моргания по кнопке (должен быть рандом)
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) closedEyes = true;
		// вызов функции пристального взгляда (должен быть рандом)
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) looksAway = true;

		// реакция на P300
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) isRun = true;


		if (closedEyes)
		{
			currentClosedEyesFrame += 0.01 * (time / 1000);
			if (currentClosedEyesFrame > 0 && currentClosedEyesFrame < 1) duckSprite.setTextureRect(sf::IntRect(0, 0, 44, 44));
			if (currentClosedEyesFrame > 1 && currentClosedEyesFrame < 2) duckSprite.setTextureRect(sf::IntRect(44, 0, 44, 44));
			if (currentClosedEyesFrame > 2 && currentClosedEyesFrame < 3) duckSprite.setTextureRect(sf::IntRect(88, 0, 44, 44));
			if (currentClosedEyesFrame > 3 && currentClosedEyesFrame < 4) duckSprite.setTextureRect(sf::IntRect(44, 0, 44, 44));
			if (currentClosedEyesFrame > 4)
			{
				currentClosedEyesFrame -= 4;
				duckSprite.setTextureRect(sf::IntRect(0, 0, 44, 44));
				closedEyes = false;
			}

		}

		if (looksAway)
		{
			currentLooksAwayFrame += 0.005 * (time / 1000);
			if (currentLooksAwayFrame > 0 && currentLooksAwayFrame < 1) duckSprite.setTextureRect(sf::IntRect(0, 0, 44, 44));
			if (currentLooksAwayFrame > 1 && currentLooksAwayFrame < 2) duckSprite.setTextureRect(sf::IntRect(132, 0, 44, 44));
			if (currentLooksAwayFrame > 2 && currentLooksAwayFrame < 3) duckSprite.setTextureRect(sf::IntRect(176, 0, 44, 44));
			if (currentLooksAwayFrame > 3 && currentLooksAwayFrame < 4) duckSprite.setTextureRect(sf::IntRect(132, 0, 44, 44));
			if (currentLooksAwayFrame > 4)
			{
				currentLooksAwayFrame -= 4;
				duckSprite.setTextureRect(sf::IntRect(0, 0, 44, 44));
				looksAway = false;
			}
		}

		if (isRun)
		{
			//переписать!
			//detect.setString("P300 DETECTED!");

			score += time/1000;

			currentIsRunFrame += 0.01 * time/1000;
			if (currentIsRunFrame > 7) currentIsRunFrame -= 2;
			duckSprite.setTextureRect(sf::IntRect(44 * int(currentIsRunFrame), 0, 44, 44));
			duckSprite.move(offset * (time/1000), 0);

			offset -= 0.0001;
			if (offset < 0)
			{
				offset = 0.2;
				isRun = false;
				duckSprite.setTextureRect(sf::IntRect(0, 0, 44, 44));
			}

		}

		window.clear(sf::Color(190, 208, 202));


		for (int i = 0; i < 6; i++)
		{
			groundSprite.setPosition(groundCoord * i, 270);
			window.draw(groundSprite);
		}

		strScore = std::to_string((int)score);

		text.setString("SCORE: " + strScore);//задает строку тексту
		text.setPosition(10, 10);//задаем позицию текста
		
		//arrow.setPosition(650, 340);
		incentiveTarget.setPosition(1200, 340);
		nonIncentiveTarget.setPosition(100, 340);
		window.draw(incentiveTarget);
		window.draw(nonIncentiveTarget);

		timer.setPosition(100, 340);
		window.draw(timer);

		window.draw(text);//рисую этот текст

		window.draw(duckSprite);
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
