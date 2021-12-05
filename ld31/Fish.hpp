#include "Pickup.hpp"


class Fish : public Pickup {
private:
	bzsf::Drawable drawable;
	sf::Vector2f TARGET;
	bool reachedTarget;

protected:
	void updateCurrent(sf::Time dt, CommandQueue& commandQueue);
	//void drawCurrent(sf::RenderTarget& target, sf::RenderStates states);

public:
	explicit Fish(const AnimationStorage& animations, sf::Vector2f TARGET);

	sf::FloatRect getBoundingRect();
	bool isAffectedByWind();

	void drawSea(sf::RenderTarget& target, sf::RenderStates states);
};