#include "Entity.hpp"

Entity::Entity() : destroyed(false) {}

void Entity::applyGravity(sf::Time dt) {
	velocity += gravity * dt.asSeconds();
}

void Entity::updateCurrent(sf::Time dt, CommandQueue& commandQueue) {
	applyGravity(dt);
	move(velocity * dt.asSeconds());
}


void Entity::destroy() {
	destroyed = true;
}

bool Entity::isDestroyed() {
	return destroyed;
}

bool Entity::isMarkedForRemoval() {
	return isDestroyed();
}

bool Entity::isAffectedByWind() {
	return true;
}