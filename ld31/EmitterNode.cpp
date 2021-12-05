#include "EmitterNode.hpp"
#include "ParticleNode.hpp"

EmitterNode::EmitterNode(Particle::Type type, float emissionRate)
	: type(type)
	, particleSystem(nullptr)
	, emissionRate(emissionRate) {}

void EmitterNode::updateCurrent(sf::Time dt, CommandQueue& commandQueue) {
	if(particleSystem)
		emitParticles(dt);
	else {
		Command command(Category::ParticleSystem, derivedAction<ParticleNode>([this](ParticleNode& p, sf::Time dt) {
			if(p.getParticleType() == type)
				particleSystem = &p;
		}));

		commandQueue.push(command);
	}
}

void EmitterNode::emitParticles(sf::Time dt) {
	const sf::Time interval = sf::seconds(1.f) / emissionRate;

	overflow += dt;

	while(overflow >= interval) {
		overflow -= interval;
		particleSystem->addParticle(getWorldPosition());
	}
}

float EmitterNode::getEmissionRate() {
	return emissionRate;
}