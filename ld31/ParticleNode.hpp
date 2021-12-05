#include "Entity.hpp"
#include "DataTables.hpp"
#include "Particle.hpp"

class ParticleNode : public Entity {
private:
	std::deque<Particle> particles;
	const sf::Texture& texture;
	Particle::Type type;

	sf::VertexArray vertexArray;
	bool needsVertexUpdate;

private:
	virtual void updateCurrent(sf::Time dt, CommandQueue& commandQueue);
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states);

	void computeVertices();
	void addVertex(float x, float y, float tx, float ty, const sf::Color& color);

public:
	ParticleNode(Particle::Type type, const TextureStorage& textures);

	void applyWind(float wind, sf::Time dt);

	void addParticle(sf::Vector2f position);
	Particle::Type getParticleType();
	virtual sf::Uint32 getCategory();
};