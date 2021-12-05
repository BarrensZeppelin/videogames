#include "ParticleNode.hpp"
#include "DataTables.hpp"

namespace {
	auto Table = initializeParticleData();
}


ParticleNode::ParticleNode(Particle::Type type, const TextureStorage& textures)
	: type(type)
	, vertexArray(sf::Quads)
	, needsVertexUpdate(true)
	, texture(textures.get(Table[type].texture)) {}


void ParticleNode::updateCurrent(sf::Time dt, CommandQueue& commandQueue) {
	while(!particles.empty() && particles.front().lifetime <= sf::Time::Zero)
		particles.pop_front();

	for(Particle& p : particles) {
		p.lifetime -= dt;
		
		for(Particle::Affector& a : Table[type].affectors)
			a(p, dt);
	}

	needsVertexUpdate = true;
}

void ParticleNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) {
	if(needsVertexUpdate) {
		computeVertices();

		needsVertexUpdate = false;
	}

	states.texture = &texture;

	target.draw(vertexArray, states);
}


void ParticleNode::computeVertices() {
	sf::Vector2f size(texture.getSize());
	sf::Vector2f half = size / 2.f;

	// Refill vertex array
	vertexArray.clear();
	for(Particle& particle : particles) {
		sf::Vector2f pos = particle.position;
		sf::Color color = particle.color;

		float ratio = particle.lifetime.asSeconds() / Table[type].lifetime.asSeconds();
		color.a = static_cast<sf::Uint8>(255 * std::max(ratio, 0.f));

		addVertex(pos.x - half.x, pos.y - half.y, 0.f, 0.f, color);
		addVertex(pos.x + half.x, pos.y - half.y, size.x, 0.f, color);
		addVertex(pos.x + half.x, pos.y + half.y, size.x, size.y, color);
		addVertex(pos.x - half.x, pos.y + half.y, 0.f, size.y, color);
	}
}

void ParticleNode::addVertex(float x, float y, float tx, float ty, const sf::Color& color) {
	sf::Vertex vertex;
	vertex.position = sf::Vector2f(x, y);
	vertex.texCoords = sf::Vector2f(tx, ty);
	vertex.color = color;

	vertexArray.append(vertex);
}


void ParticleNode::applyWind(float wind, sf::Time dt) {
	if(type == Particle::Snow)
		for(Particle& p : particles)
			p.position += sf::Vector2f(wind * 3 * dt.asSeconds(), 0);
}


void ParticleNode::addParticle(sf::Vector2f position) {
	Particle p;
	p.color = Table[type].color;
	p.lifetime = Table[type].lifetime;
	p.position = position;
	p.velocity = sf::Vector2f();

	if(Table[type].velocityRangeX.x != 0)
		p.velocity.x = Table[type].velocityRangeX.x + rand() % int(Table[type].velocityRangeX.y * 2);
	if(Table[type].velocityRangeY.x != 0)
		p.velocity.y = Table[type].velocityRangeY.x + rand() % int(Table[type].velocityRangeY.y * 2);

	particles.push_back(p);
}

Particle::Type ParticleNode::getParticleType() {
	return type;
}

sf::Uint32 ParticleNode::getCategory() {
	return Category::ParticleSystem;
}