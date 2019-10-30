#include "GameScene.h"
#include "GameView.h"
#include <float.h> // Константа FLT_EPSILON
#include <math.h>
#include <iostream>
#include <sstream>

// Добавим часто используемые идентификаторы в глобальное пространство имён.
using sf::Keyboard;
using sf::Vector2f;
using sf::Vector2i;
using sf::Font;
using sf::Text;
using sf::Color;

// Абсолютная скорость движения игрока.
static const float PLAYER_SPEED = 110;

// Выполняет нормализацию вектора (приведение к единичной длине).
static Vector2f Normalize(const Vector2f &value)
{
    const float length = std::hypotf(value.x, value.y);
    if (length < FLT_EPSILON)
    {
        return Vector2f(0, 0);
    }

    return value / length;
}

// Округляет компоненты вектора до ближайших целых чисел.
static Vector2f Round(const Vector2f &value)
{
    return Vector2f(roundf(value.x), roundf(value.y));
}

// Определяет состояние клавиатуры и выбирает направление движения.
// Возвращает нормализованный вектор направления игрока.
static Vector2f GetPlayerDirection()
{
    Vector2f direction;
    if (Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up))
    {
        direction.y = -1;
	}
	else {
		direction.y = 1;
	}
    
    if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left))
    {
        direction.x = -1;
    }
    else if (Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right))
    {
        direction.x = +1;
    }

    return Normalize(direction);
}

Font font; 
Text text("", font, 25);


GameScene* NewGameScene(const char* file)
{
    GameScene* pLogic = new GameScene;
    TmxLevel& level = pLogic->level;

    level.LoadFromFile(file);
    pLogic->player = level.GetFirstObject("player");
    pLogic->coins = level.GetAllObjects("coin");
    pLogic->enemies = level.GetAllObjects("enemy");
	pLogic->blocks = level.GetAllObjects("");

	
	font.loadFromFile("res/arial.ttf");

	//text.setColor(Color::Red);
	text.setStyle(sf::Text::Bold);

    return pLogic;
}

int SCORE = 0;
// ТУТ ЛОГИКА СТОЛКНОВЕНИЙ
void UpdateGameScene(void *pData, GameView &view, float deltaSec)
{
    // Извлекаем указатель на GameLogic, ранее переданный в игровой цикл.
    GameScene *pLogic = reinterpret_cast<GameScene *>(pData);
    (void)deltaSec;

	bool Gravity = true;

    TmxObject &player = pLogic->player;
    const Vector2f movement = Round(GetPlayerDirection() * PLAYER_SPEED * deltaSec);
    player.MoveBy(movement);


	for (int i = 0; i < pLogic->coins.size(); i++) {

		if (pLogic->coins[i].rect.intersects(player.rect)) {
			if (pLogic->coins[i].show) {
				pLogic->coins[i].show = false;
				SCORE += 10;
			}
		}
	}

	for (int i = 0; i < pLogic->enemies.size(); i++) {

		if (pLogic->enemies[i].rect.intersects(player.rect)) {
			if (pLogic->enemies[i].show) {
				pLogic->enemies[i].show = false;
				SCORE -= 20;
			}
		}
	}

	for (int i = 0; i < pLogic->blocks.size(); i++)
	{
		if (pLogic->blocks[i].rect.intersects(player.rect)) {
			if (pLogic->blocks[i].show) {
				Gravity = false;
				player.MoveBy(Vector2f(-movement.x, -movement.y));
				break;
			}
		}
	}

	if (!Gravity) {
		player.MoveBy(Vector2f(movement.x, 0));
		for (int i = 0; i < pLogic->blocks.size(); i++)
		{
			if (pLogic->blocks[i].rect.intersects(player.rect)) {
				player.MoveBy(Vector2f(-movement.x, 0));
				break;
			}
		}
	}
	
	

    const Vector2i windowSize = view.windowSize;
    SetCameraCenter(view, player.sprite.getPosition() + Vector2f(windowSize.x / 4, windowSize.y / 4));
}

void DrawGameScene(void *pData, GameView &view)
{
    // Извлекаем указатель на GameLogic, ранее переданный в игровой цикл.
    GameScene *pLogic = reinterpret_cast<GameScene *>(pData);
    sf::RenderTarget &target = view.window;

    pLogic->level.Draw(target);
    for (const TmxObject &coin : pLogic->coins)
    {
		if(coin.show)
			target.draw(coin.sprite);
    }
    for (const TmxObject &enemy : pLogic->enemies)
    {
		if(enemy.show)
			target.draw(enemy.sprite);
    }
	for (const TmxObject &block : pLogic->blocks)
	{
		if(block.show)
			target.draw(block.sprite);
	}

    target.draw(pLogic->player.sprite);
	
	
	std::ostringstream oss;
	oss << "Score: " << SCORE;

	text.setString(oss.str());
	text.setPosition(view.camera.getCenter().x - 393, view.camera.getCenter().y - 300);//348, 369

	std::cout << view.camera.getCenter().x << " " << view.camera.getCenter().y << "\n";
	view.window.draw(text);
}

void DestroyGameScene(GameScene *&pScene)
{
    delete pScene;
    pScene = nullptr;
}
