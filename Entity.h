#ifndef ENTITY_H
#define ENTITY_H
#include <string>
#include <memory> 
#include "Components.h"
class Entity
{
private:
	friend class EntityManager;
	const size_t		m_id     = 0;
	const std::string	m_tag    = "Default";
	bool				m_active = true;
	Entity(const size_t i, const std::string& t);

public:
	std::shared_ptr<CTransform>		cTransform;
	std::shared_ptr<CInput>			cInput;
	std::shared_ptr<CShape>			cShape;
	std::shared_ptr<CCollision>		cCollision;
	std::shared_ptr<CScore>			cScore;
	std::shared_ptr<CLifeSpan>		cLifespan;
	
	bool isActive() const;
	const size_t id() const;
	const std::string& tag() const;
	void destroy();
};
#endif // !ENTITY_H

