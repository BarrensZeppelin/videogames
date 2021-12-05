#include "SceneNode.hpp"
#include "resid.hpp"

class Eagle;
class CliffNode : public SceneNode {
public:
	struct Triangle {
		sf::FloatRect size;
		sf::Vector2f normal;
	};

private:
	sf::Sprite top, bottom;

	const sf::Uint32 RIGHT = 120;

	static const sf::Uint16 triCount = 5;
	std::array<Triangle, triCount> tris;

private:
	void drawCurrent(sf::RenderTarget& target, sf::RenderStates states);
	
	
	 
protected:
	std::pair<bool, sf::Vector2f> testCollision(const sf::FloatRect& bbox);

public:
	CliffNode(const TextureStorage& textures, const int SEALEVEL);

	virtual sf::Uint32 getCategory();

	void drawSea(sf::RenderTarget& target, sf::RenderStates states);

	friend Eagle;
};

