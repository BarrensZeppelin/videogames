#include "SnowNode.hpp"
#include "ParticleNode.hpp"
#include "Application.hpp"

SnowNode::SnowNode() : EmitterNode(Particle::Snow, 80.f) {}

void SnowNode::emitParticles(sf::Time dt) {
	const sf::Time interval = sf::seconds(1.f) / emissionRate;

	overflow += dt;

	while(overflow >= interval) {
		overflow -= interval;
		if(rand() % 3 != 0)
			particleSystem->addParticle(sf::Vector2f(-400 + rand() % (Application::settings.windowSize.x + 400), -20));
		else
			particleSystem->addParticle(sf::Vector2f((rand() % 2 == 0 ? -20 : Application::settings.windowSize.x + 20), 50 + rand() % 200));
	}
}