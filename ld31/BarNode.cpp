#include "BarNode.hpp"

BarNode::BarNode(const float MAXVALUE, const sf::Color& color, sf::Vector2f size, bool xscale)
	: MAXVALUE(MAXVALUE)
	, size(size)
	, xscale(xscale)
	, bgShape(size + sf::Vector2f(4, 4))
	, barShape(size)
	, d(true) {

	bgShape.setPosition(-2, -2);

	bgShape.setFillColor(sf::Color::Black);
	barShape.setFillColor(color);

	setOrigin(barShape.getLocalBounds().width / 2, barShape.getLocalBounds().height / 2);
}


void BarNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) {
	if(d) {
		target.draw(bgShape, states);
		target.draw(barShape, states);
	}
}

void BarNode::setValue(float val) {
	if(xscale)
		barShape.setScale(std::min(val / MAXVALUE, MAXVALUE), 1);
	else
		barShape.setScale(1, std::min(val / MAXVALUE, MAXVALUE));
}

void BarNode::setColor(const sf::Color& color) {
	barShape.setFillColor(color);

	bgShape.setFillColor(sf::Color(0, 0, 0, color.a));
}


const sf::Color& BarNode::getColor() {
	return barShape.getFillColor();
}


void BarNode::drawSea(sf::RenderTarget& target, sf::RenderStates states) {
	states.transform *= getTransform();

	drawCurrent(target, states);
}