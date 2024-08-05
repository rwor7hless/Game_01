#include "Game.h"
#include <iostream>
#include <fstream>
#include <time.h>
#include <windows.h>
#include <mmsystem.h>


Game::Game(const std::string& config)
{
	init(config);
}

void Game::init(const std::string& path)
{
	std::ifstream fin(path);

	m_window.create(sf::VideoMode(1280, 720), "Assignment 2");
	m_window.setFramerateLimit(60);

	spawnPlayer();
}

void Game::run()
{
	while (m_running)
	{

		m_entities.update();

		if (m_paused == 0)
		{
			sEnemySpawner();
			sMovement();
		}
		sCollision();
		sUserInput();
		sLifeSpan();
		sRender();
		m_currentFrame++;
	}
}

void Game::setPaused(bool paused)
{
	m_paused = paused;
}

void Game::spawnPlayer()
{
	auto entity = m_entities.addEntity("player");

	int p_posx = m_window.getSize().x / 2;
	int p_posy = m_window.getSize().y / 2;

	entity->cTransform = std::make_shared<CTransform>(Vec2(p_posx, p_posy), Vec2(0.0f, 0.0f), 0);
	entity->cShape = std::make_shared<CShape>(32.0f, 8, sf::Color(10, 10, 10), sf::Color(255, 0, 0), 4.0f);

	entity->cInput = std::make_shared<CInput>();

	m_player = entity;
}

void Game::spawnEnemy()
{
	auto entity = m_entities.addEntity("enemy");

	// random seed
	srand(time(NULL));

	int e_size = rand() % 30 + 4;

	int e_sides = rand() % 10 + 3;

	float e_posx = rand() % (m_window.getSize().x - e_size) + e_size;
	float e_posy = rand() % (m_window.getSize().y - e_size) + e_size;

	float e_velx = rand() % 10 - 5;
	float e_vely = rand() % 10 - 5;

	entity->cTransform = std::make_shared<CTransform>(Vec2(e_posx, e_posy), Vec2(e_velx, e_vely), 0);
	entity->cShape = std::make_shared<CShape>(e_size, e_sides, sf::Color(rand() % 255, rand() % 255, rand() % 255), sf::Color(255, 255, 0), 4.0f);
	entity->cScore = std::make_shared<CScore>(e_sides * 100);

	m_lastEnemySpawnTime = m_currentFrame;
}

void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2& mousePos)
{
	auto bullet = m_entities.addEntity("bullet");
	Vec2 dir = mousePos - Vec2(Vec2(entity->cTransform->pos.x, entity->cTransform->pos.y));
	dir.norm();
	bullet->cTransform = std::make_shared<CTransform>(Vec2(entity->cTransform->pos.x, entity->cTransform->pos.y), Vec2(5 * dir.x, 5 * dir.y), 0);
	bullet->cLifespan = std::make_shared<CLifeSpan>(100);
	bullet->cShape = std::make_shared<CShape>(10, 8, sf::Color(255, 255, 255), sf::Color(255, 0, 0), 2);
}

void Game::sEnemySpawner()
{
	if (m_currentFrame - m_lastEnemySpawnTime > 120)
		spawnEnemy();
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
	// TODO
}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> entity)
{
	// TODO
}

void Game::sCollision()
{
	for (auto b : m_entities.getEntities("bullet"))
	{
		for (auto e : m_entities.getEntities("enemy"))
		{
			// рассчет коллизии
			Vec2 distance = Vec2(b->cTransform->pos.x, b->cTransform->pos.y) - Vec2(e->cTransform->pos.x, e->cTransform->pos.y);
			if (abs(distance.length()) < b->cShape->circle.getRadius() + e->cShape->circle.getRadius())
			{
				// уничтожение врага и пули
				e->destroy();
				b->destroy();

				// Увеличение количества очков
				m_score = m_score + e->cScore->score;

				PlaySound(L"enemy_collision.wav", NULL, SND_ASYNC);
			}
		}
	}


	// столкновение игрока с врагом
	for (auto e : m_entities.getEntities("enemy"))
	{

		Vec2 distance = Vec2(m_player->cTransform->pos.x, m_player->cTransform->pos.y) - Vec2(e->cTransform->pos.x, e->cTransform->pos.y);
		if (abs(distance.length()) < m_player->cShape->circle.getRadius() + e->cShape->circle.getRadius())
		{
			m_player->destroy();
			e->destroy();
			spawnPlayer();
		}
	}
}

void Game::sRender()
{
	m_window.clear();

	for (auto e : m_entities.getEntities())
	{

		// Проверка границ окна
		if (e->tag() == "enemy")
		{
			if (e->cTransform->pos.x - e->cShape->circle.getRadius() < 0 || e->cTransform->pos.x + e->cShape->circle.getRadius() > m_window.getSize().x)
			{
				e->cTransform->velocity.x = -e->cTransform->velocity.x;
			}
			if (e->cTransform->pos.y - e->cShape->circle.getRadius() < 0 || e->cTransform->pos.y + e->cShape->circle.getRadius() > m_window.getSize().y)
			{
				e->cTransform->velocity.y = -e->cTransform->velocity.y;
			}
		}

		if (e->tag() == "bullet")
		{
			e->cShape->circle.setFillColor(sf::Color(255, 255, 255, (float(e->cLifespan->remaining) / float(e->cLifespan->total)) * 255));
			e->cShape->circle.setOutlineColor(sf::Color(255, 0, 0, (float(e->cLifespan->remaining) / float(e->cLifespan->total)) * 255));
		}


		// Вращение сущностей
		e->cTransform->angle += 1.0f;
		e->cShape->circle.setRotation(e->cTransform->angle);


		m_window.draw(e->cShape->circle);
	}

	sf::Font myFont;
	myFont.loadFromFile("fonts/tech.ttf");
	sf::Text text(std::to_string(m_score), myFont, 24);
	text.setPosition(20, 20 - (float)text.getCharacterSize());
	m_window.draw(text);
	m_window.display();
}

void Game::sMovement()
{

	m_player->cTransform->velocity = { 0,0 };

	// Реализация передвижения персонажа
	if (m_player->cInput->up && (m_player->cTransform->pos.y - m_player->cShape->circle.getRadius()) > 5)
	{
		m_player->cTransform->velocity.y = -5;
	}
	if (m_player->cInput->down && (m_player->cTransform->pos.y + m_player->cShape->circle.getRadius()) < m_window.getSize().y - 5)
	{
		m_player->cTransform->velocity.y = 5;
	}
	if (m_player->cInput->left && (m_player->cTransform->pos.x - m_player->cShape->circle.getRadius()) > 5)
	{
		m_player->cTransform->velocity.x = -5;
	}
	if (m_player->cInput->right && (m_player->cTransform->pos.x + m_player->cShape->circle.getRadius()) < m_window.getSize().x - 5)
	{
		m_player->cTransform->velocity.x = 5;
	}

	for (auto e : m_entities.getEntities())
	{
		// Добавление передвижения
		e->cTransform->pos.x += e->cTransform->velocity.x;
		e->cTransform->pos.y += e->cTransform->velocity.y;

		// Установка позиции
		e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);
	}
}



void Game::sUserInput()
{
	sf::Event event;

	while (m_window.pollEvent(event))
	{

		if (event.type == sf::Event::Closed)
		{
			m_running = false;
		}

		if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W:

				m_player->cInput->up = true;
				break;
			case sf::Keyboard::S:

				m_player->cInput->down = true;
				break;
			case sf::Keyboard::A:

				m_player->cInput->left = true;
				break;
			case sf::Keyboard::D:

				m_player->cInput->right = true;
				break;
			case sf::Keyboard::P:
				if (m_paused == false)
				{
					setPaused(true);
					std::cout << m_paused << "\n";
				}
				else if (m_paused == true)
				{
					setPaused(false);
					std::cout << m_paused << "\n";
				}
				break;
			case sf::Keyboard::Escape:
			{
				m_running = false;
			}
			default: break;
			}
		}

		if (event.type == sf::Event::KeyReleased)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W:

				m_player->cInput->up = false;
				break;
			case sf::Keyboard::S:

				m_player->cInput->down = false;
				break;
			case sf::Keyboard::A:

				m_player->cInput->left = false;
				break;
			case sf::Keyboard::D:

				m_player->cInput->right = false;
				break;
			default: break;
			}
		}

		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				spawnBullet(m_player, Vec2(event.mouseButton.x, event.mouseButton.y));
			}
			if (event.mouseButton.button == sf::Mouse::Right)
			{
				// call special weapon here
			}

		}


	}
}

void Game::sLifeSpan()
{
	for (auto e : m_entities.getEntities())
	{
		if (e->tag() == "bullet")
		{
			if (e->cLifespan->remaining > 0)
			{
				e->cLifespan->remaining -= 1;
			}
			else if (e->cLifespan->remaining <= 0)
			{
				e->destroy();
			}
		}
	}

}



