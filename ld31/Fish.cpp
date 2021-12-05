#include "Fish.hpp"
#include "Utility.hpp"

Fish::Fish(const AnimationStorage& animations, const sf::Vector2f TARGET)
	: Pickup(Pickup::Fish)
	, TARGET(TARGET)
	, reachedTarget(false) {

	drawable.setAnimation(*ParseAnimation(animations.at(Animation::Fish)));
	CenterOrigin(drawable);
}


void Fish::updateCurrent(sf::Time dt, CommandQueue& commandQueue) {
	drawable.update(dt);
	
	sf::Vector2f dir = UnitVector(TARGET - getWorldPosition());
	move(dir * 200.f * dt.asSeconds());

	if(dir.x < 0)
		setScale(-1.f, 1.f);
	else
		setScale(1.f, 1.f);

	if(!reachedTarget && getWorldPosition().x <= TARGET.x) {
		TARGET.x = Application::settings.windowSize.x + 200;
		TARGET.y += rand() % 75;

		reachedTarget = true;
	}
}


sf::FloatRect Fish::getBoundingRect() {
	return getTransform().transformRect(drawable.getGlobalBounds());
}

bool Fish::isAffectedByWind() {
	return false;
}

void Fish::drawSea(sf::RenderTarget& target, sf::RenderStates states) {
	states.transform *= getTransform();

	target.draw(drawable, states);
}