#include "SceneNode.hpp"


class SpriteNode : public SceneNode {
private:
	sf::Sprite sprite;
	sf::Uint32 cat;

public:
	SpriteNode(const sf::Texture& texture, sf::Uint32 category = Category::None);
	SpriteNode(const sf::Texture& texture, const sf::IntRect& textureRect, sf::Uint32 category = Category::None);


	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states);

	virtual sf::Uint32 getCategory();
};