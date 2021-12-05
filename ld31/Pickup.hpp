#ifndef LD31_PICKUP
#define LD31_PICKUP

#include <bzsf/SFML_Snips.hpp>
#include "Application.hpp"
#include "Eagle.hpp"


class Pickup : public Entity {
public:
	enum Type {
		Coin,
		Fish,
		TypeCount
	};

protected:
	bzsf::Drawable drawable;
	Type type;

protected:
	virtual void updateCurrent(sf::Time dt, CommandQueue& commandQueue);
	void drawCurrent(sf::RenderTarget& target, sf::RenderStates states);

public:
	explicit Pickup(Type type);
	Pickup(Type type, Context context);

	virtual sf::FloatRect getBoundingRect();
	virtual sf::Uint32 getCategory();

	void apply(Eagle& player);

	virtual void drawSea(sf::RenderTarget& target, sf::RenderStates states);
};

#endif