#include "SceneNode.hpp"
#include "resid.hpp"
#include "EmitterNode.hpp"

struct WaterSplash {
	enum Size {
		Small,
		Medium,
		Big
	};
	
	float			position;
	sf::Color		color;
	sf::Time		lifetime;
	float			scale;
	EmitterNode*	emitter;
};


class WaterSplashNode : public SceneNode {
private:
	const float SEALINE;
	const sf::Texture& texture;
	
	std::deque<WaterSplash> splashes;
	
	sf::VertexArray vertexArray;
	bool needsVertexUpdate;

private:
	virtual void updateCurrent(sf::Time dt, CommandQueue& commandQueue);
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states);

	void computeVertices();
	void addVertex(float x, float y, float tx, float ty, const sf::Color& color);

public:
	WaterSplashNode(const TextureStorage& textures, const float SEALINE);

	void splash(float position);

	virtual sf::Uint32 getCategory();
};