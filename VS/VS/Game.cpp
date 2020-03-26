/*
 * "Game module"
 * 
 * This module contain the base logic of window
 * with target generation.
 * 
 * Module use SFML graphic library.
 *
 * Neizhko Vladimir,
 * Saint-Petersburg,
 * 2020.03
 */

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
int WIDTH = 680; //1900
int HEIGHT = 680; //1800

int circleRadius = 30; // радиус окружности
double arrowDistance = 2; // множитель, регулирует расстояние стрелок от круга
int indent = 20;


std::string arrowTexture = "res/textures/arrow.png";
std::string windowName = "P-300";

unsigned short int point_0 = 0;
unsigned short int point_1 = 300;
unsigned short int point_2 = 400;
unsigned short int point_3 = 1500;
unsigned short int point_generated = 300;
unsigned short int stats = 0;

bool fullscreenMode = false;
bool isKeyPressedOneTime = false;

int arrowLargeSide = 130;
int arrowSmallSide = 52;

sf::Color colorArrow = sf::Color(0, 0, 0); //128, 128, 128
sf::Color colorTagretArrow = sf::Color(0, 0, 128);
sf::Color colorCircle = sf::Color(0, 0, 0); //255, 255, 255
sf::Color colorRectangle = sf::Color(128, 128, 128); //0, 0, 0
sf::Color colorBackground = sf::Color(0, 0, 0);
/////////////////////////////////////////////////////////////////////////
using namespace Game;

bool runGame = true;

sf::RenderWindow window;
sf::CircleShape circle(circleRadius);
sf::RectangleShape rectangle;
sf::Sprite arrow_spr;

void Game::input() {

	/* In this void we give window management instructions:
	 * we can set the size of frame by the key "F" on a keyboard
	 * and we can close the window by the key "Escape".
     */

	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			runGame = false;
			window.close();
		}

		if (isKeyPressedOneTime == false) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
				window.close();
				isKeyPressedOneTime = true;
			}
				
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
				//if (fullscreenMode) window.create(sf::VideoMode::getFullscreenModes()[0], windowName);
				if (fullscreenMode) window.create(sf::VideoMode::getDesktopMode(), windowName);
				if (!fullscreenMode) window.create(sf::VideoMode(WIDTH, HEIGHT), windowName);
				fullscreenMode = !fullscreenMode;
				isKeyPressedOneTime = true;
			}
		} else if (event.type == sf::Event::KeyReleased) isKeyPressedOneTime = false;
		
	}
}

void DrawAllArrows(int rectanglePositionX, int rectanglePositionY,
				   int circlePositionX, int circlePositionY,
	               int upArrowPositionX, int upArrowPositionY,
                   int downArrowPositionX, int downArrowPositionY,
                   int leftArrowPositionX, int leftArrowPositionY,
	               int rightArrowPositionX, int rightArrowPositionY) {
	/* 
	 * In this void we drawing all arrows, circle and rectangle
	 * in static mode. Just for testing.
	 */

	sf::Texture arrow;
	if (!arrow.loadFromFile(arrowTexture))
	{
		std::cout << "ERROR! Texture missing!" << std::endl;
	}

	rectangle.setPosition(rectanglePositionX, rectanglePositionY);
	window.draw(rectangle);

	arrow_spr.setTexture(arrow);
	arrow_spr.setColor(colorArrow);

	arrow_spr.setPosition(upArrowPositionX, upArrowPositionY);
	arrow_spr.setRotation(90.f);
	window.draw(arrow_spr);

	arrow_spr.setPosition(downArrowPositionX, downArrowPositionY);
	arrow_spr.setRotation(270.f);
	window.draw(arrow_spr);

	arrow_spr.setPosition(leftArrowPositionX, leftArrowPositionY);
	arrow_spr.setRotation(0.f);
	window.draw(arrow_spr);

	arrow_spr.setPosition(rightArrowPositionX, rightArrowPositionY);
	arrow_spr.setRotation(180.f);
	window.draw(arrow_spr);

	circle.setPosition(circlePositionX, circlePositionY);
	window.draw(circle);
}

void DrawByAlgorythm(int sceneNumber, int rectanglePositionX, int rectanglePositionY,
	int circlePositionX, int circlePositionY,
	int upArrowPositionX, int upArrowPositionY,
	int downArrowPositionX, int downArrowPositionY,
	int leftArrowPositionX, int leftArrowPositionY,
	int rightArrowPositionX, int rightArrowPositionY) {

	/*
	 * In this void we drawing all arrows, circle and rectangle
	 * in dynamic mode by special algirythm.
	 */

	sf::Texture arrow;
	if (!arrow.loadFromFile(arrowTexture))
	{
		std::cout << "ERROR! Texture missing!" << std::endl;
	}
	arrow_spr.setTexture(arrow);
	arrow_spr.setColor(colorArrow);

	rectangle.setPosition(rectanglePositionX, rectanglePositionY);
	window.draw(rectangle);

	circle.setPosition(circlePositionX, circlePositionY);
	window.draw(circle);

	switch (sceneNumber) {
	case 1:
		//left
		arrow_spr.setPosition(leftArrowPositionX, leftArrowPositionY);
		arrow_spr.setRotation(0.f);
		window.draw(arrow_spr);
		break;
	case 2:
		//right
		arrow_spr.setPosition(rightArrowPositionX, rightArrowPositionY);
		arrow_spr.setRotation(180.f);
		window.draw(arrow_spr);
		break;
	case 3:
		//up
		arrow_spr.setPosition(upArrowPositionX, upArrowPositionY);
		arrow_spr.setRotation(90.f);
		window.draw(arrow_spr);
		break;
	case 4:
		//down
		arrow_spr.setPosition(downArrowPositionX, downArrowPositionY);
		arrow_spr.setRotation(270.f);
		window.draw(arrow_spr);
		break;
	default:
		//no arrows
		break;
	}

}

void Game::main(std::thread* gameThread)
{
	std::mt19937 randomValueGenerator(time(0));
	std::uniform_int_distribution<int> dist(1, 4);

	std::mt19937 randomValueGeneratorForLastPoint(time(0));
	std::uniform_int_distribution<int> dist_2(300, 600);

	sf::sleep(sf::milliseconds(2000)); //pause before starting

	
	window.create(sf::VideoMode(WIDTH, HEIGHT), "P-300");

	sf::Clock gameClock;
	sf::Clock targetClock;

	sf::Texture arrow;
	if (!arrow.loadFromFile(arrowTexture))
	{
		// catch exception
		std::cout << "ERROR! Texture missing!" << std::endl;
	}

	int targetTimer = 0;
	int randomValue = 0;
	int randomValueOfPointGenerated = 300;

	//инициализация игры
	//while (runGame)
	while (window.isOpen())
	{
		// Игровой цикл, который можно остановить

		input();

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
			/*
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
			*/
			sceneNumber = randomValue;
			stats = sceneNumber;
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

		//fill screen with BLACK color
		window.clear(colorBackground); 
		// calculating coords of upper left corner
		int rectanglePositionX = (window.getSize().x / 2) - ((arrowLargeSide * arrowDistance) + indent); 
		int rectanglePositionY = (window.getSize().y / 2) - ((arrowLargeSide * arrowDistance) + indent);

		int circlePositionX = (window.getSize().x / 2) - circleRadius;
		int circlePositionY = (window.getSize().y / 2) - circleRadius;

		int upArrowPositionX = (window.getSize().x / 2) + (arrowSmallSide / 2);
		int upArrowPositionY = (window.getSize().y / 2) - (arrowLargeSide * arrowDistance);

		int downArrowPositionX = (window.getSize().x / 2) - (arrowSmallSide / 2);
		int downArrowPositionY = (window.getSize().y / 2) + (arrowLargeSide * arrowDistance);

		int leftArrowPositionX = (window.getSize().x / 2) - (arrowLargeSide * arrowDistance);
		int leftArrowPositionY = (window.getSize().y / 2) - (arrowSmallSide / 2);

		int rightArrowPositionX = (window.getSize().x / 2) + (arrowLargeSide * arrowDistance);
		int rightArrowPositionY = (window.getSize().y / 2) + (arrowSmallSide / 2);
		
		circle.setFillColor(colorCircle);
		rectangle.setFillColor(colorRectangle);
		// setting size of rectangle by subtraction of right buttom corner coords and upper left corner coords
		rectangle.setSize(sf::Vector2f(((window.getSize().x / 2) + ((arrowLargeSide * arrowDistance) + indent) - rectanglePositionX),
			                           ((window.getSize().y / 2) + ((arrowLargeSide * arrowDistance) + indent)) - rectanglePositionY));

		DrawByAlgorythm(sceneNumber, rectanglePositionX, rectanglePositionY,
			circlePositionX, circlePositionY,
			upArrowPositionX, upArrowPositionY,
			downArrowPositionX, downArrowPositionY,
			leftArrowPositionX, leftArrowPositionY,
			rightArrowPositionX, rightArrowPositionY);
    
	/// TODO: make testmode and use this function in testmode
	/*	DrawAllArrows(rectanglePositionX, rectanglePositionY,
					  circlePositionX, circlePositionY, 
			          upArrowPositionX, upArrowPositionY, 
					  downArrowPositionX, downArrowPositionY, 
					  leftArrowPositionX, leftArrowPositionY, 
					  rightArrowPositionX, rightArrowPositionY);
     */

		window.display();
	}
}


void Game::stopGame() // Calling when closing
{
	runGame = false;
	//Тут действия для завершения работы игры
	//в данном случае - завершение работы цикла
}
