#include "WaterSplashNode.hpp"

WaterSplashNode::WaterSplashNode(const TextureStorage& textures, const float SEALINE)
	: SEALINE(SEALINE)
	, vertexArray(sf::Quads)
	, texture(textures.get(Texture::WaterSplash)) {}



void WaterSplashNode::updateCurrent(sf::Time dt, CommandQueue& commandQueue) {
	while(!splashes.empty() && splashes.front().lifetime <= sf::Time::Zero) {
		detachChild(*splashes.front().emitter);
		splashes.pop_front();
	}

	for(WaterSplash& s : splashes)
		s.lifetime -= dt;

	needsVertexUpdate = true;
}

void WaterSplashNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) {
	if(needsVertexUpdate) {
		computeVertices();
		
		needsVertexUpdate = false;
	}

	states.texture = &texture;

	target.draw(vertexArray, states);
}


void WaterSplashNode::computeVertices() {
	sf::Vector2f size(texture.getSize());
	sf::Vector2f half = size / 2.f;

	// Refill vertex array
	vertexArray.clear();
	for(WaterSplash& splash : splashes) {
		sf::Vector2f pos = sf::Vector2f(splash.position, SEALINE);
		sf::Color color = splash.color;

		float ratio = splash.lifetime.asSeconds() / 0.5f;
		splash.scale = std::sin(ratio * bzsf::PI);

		addVertex(pos.x - half.x, pos.y - size.y * splash.scale, 0.f, 0.f, color);
		addVertex(pos.x + half.x, pos.y - size.y * splash.scale, size.x, 0.f, color);
		addVertex(pos.x + half.x, pos.y, size.x, size.y, color);
		addVertex(pos.x - half.x, pos.y, 0.f, size.y, color);
	}
}

void WaterSplashNode::addVertex(float x, float y, float tx, float ty, const sf::Color& color) {
	sf::Vertex vertex;
	vertex.position = sf::Vector2f(x, y);
	vertex.texCoords = sf::Vector2f(tx, ty);
	vertex.color = color;

	vertexArray.append(vertex);
}



void WaterSplashNode::splash(float position) {
	WaterSplash s;
	s.color = sf::Color::White;
	s.emitter = new EmitterNode(Particle::Water, 60);
	s.emitter->setPosition(position, SEALINE);
	s.position = position;
	s.lifetime = sf::seconds(0.5f);
	s.scale = 0.f;

	splashes.push_back(s);
	attachChild(SceneNode::Ptr(s.emitter));
}


sf::Uint32 WaterSplashNode::getCategory() {
	return Category::SplashEffect;
}