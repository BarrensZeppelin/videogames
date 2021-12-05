#include "SceneNode.hpp"


class BarNode : public SceneNode {
private:
	const float MAXVALUE;

	sf::RectangleShape bgShape;
	sf::RectangleShape barShape;

	bool xscale;
	sf::Vector2f size;

private:
	void drawCurrent(sf::RenderTarget& target, sf::RenderStates states);

public:
	BarNode(const float MAXVALUE, const sf::Color& color, sf::Vector2f size, bool xscale = true);
	
	bool d;

	void setValue(float val);
	void setColor(const sf::Color& color);

	const sf::Color& getColor();

	void drawSea(sf::RenderTarget& target, sf::RenderStates states);
};