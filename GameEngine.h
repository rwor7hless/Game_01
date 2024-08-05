#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H
#include "EntityManager.h"
class GameEngine
{	
	EntityManager m_entityManager;
	void mainLoop();
	void sEnemySpawner();
};

#endif // !GAME_ENGINE_H


