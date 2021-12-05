#ifndef LD31_ENTITY
#define LD31_ENTITY

#include "SceneNode.hpp"

class Entity : public SceneNode {
private:
	bool destroyed;

public:
	sf::Vector2f velocity;
	const sf::Vector2f gravity = sf::Vector2f(0, 400.f);

protected:
	void applyGravity(sf::Time dt);
	virtual void updateCurrent(sf::Time dt, CommandQueue& commandQueue);

public:
	Entity();

	virtual void destroy();

	virtual bool isDestroyed();
	virtual bool isMarkedForRemoval();

	virtual bool isAffectedByWind();
};

#endif