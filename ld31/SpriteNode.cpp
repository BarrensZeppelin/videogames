#include "SpriteNode.hpp"


SpriteNode::SpriteNode(const sf::Texture& texture, sf::Uint32 category) : sprite(texture), cat(category) {}
SpriteNode::SpriteNode(const sf::Texture& texture, const sf::IntRect& textureRect, sf::Uint32 category)
	: sprite(texture, textureRect)
	, cat(category) {}

void SpriteNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) {
	target.draw(sprite, states);
}

sf::Uint32 SpriteNode::getCategory() {
	return cat;
}