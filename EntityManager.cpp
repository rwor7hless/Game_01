#include "EntityManager.h"

EntityManager::EntityManager() {};

void EntityManager::update() {
	for (auto e : m_entitiesToAdd)
	{
		m_entities.push_back(e);
		m_entityMap[e->tag()].push_back(e);
	}

	m_entitiesToAdd.clear();
	removeDeadEntities(m_entities);

	for (auto& [tag, entityVec] : m_entityMap)
	{
		removeDeadEntities(entityVec);
	}
	
}

void EntityManager::removeDeadEntities(EntityVec& vec)
{


	for (int i = 0; i < vec.size(); i++)
	{
		if (!vec[i]->isActive())
		{
			vec.erase(vec.begin() + i);
		}
	}

}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
	auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));

	m_entitiesToAdd.push_back(entity);

	return entity;
}

const std::map<std::string, EntityVec>& EntityManager::getEntityMap() 
{
	return m_entityMap;
}

const EntityVec& EntityManager::getEntities() 
{
	return m_entities;
}

const EntityVec& EntityManager::getEntities(const std::string& tag)
{
	return m_entityMap[tag];
}