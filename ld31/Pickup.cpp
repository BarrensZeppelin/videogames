#include "Pickup.hpp"
#include "DataTables.hpp"

namespace {
	std::vector<PickupData> Table = initializePickupData();
}

Pickup::Pickup(Type type) : type(type) {}

Pickup::Pickup(Type type, Context context)
	: type(type) {
	drawable.setTexture(context.textures->get(Table[type].texture));
}

void Pickup::updateCurrent(sf::Time dt, CommandQueue& commandQueue) {
	Entity::updateCurrent(dt, commandQueue);
}

void Pickup::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) {
	target.draw(drawable, states);
}


sf::FloatRect Pickup::getBoundingRect() {
	return getWorldTransform().transformRect(drawable.getGlobalBounds());
}

sf::Uint32 Pickup::getCategory() {
	return Category::Pickup;
}


void Pickup::apply(Eagle& player) {
	Table[type].action(player);
}

void Pickup::drawSea(sf::RenderTarget& target, sf::RenderStates states) {
	states.transform *= getTransform();

	target.draw(drawable, states);
}