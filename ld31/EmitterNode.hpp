#ifndef LD31_EMITTER
#define LD31_EMITTER

#include "SceneNode.hpp"
#include "Particle.hpp"

class ParticleNode;

class EmitterNode : public SceneNode {
protected:
	sf::Time overflow;
	Particle::Type type;
	ParticleNode* particleSystem;

	float emissionRate;

private:
	virtual void updateCurrent(sf::Time dt, CommandQueue& commandQueue);

	virtual void emitParticles(sf::Time dt);

public:
	explicit EmitterNode(Particle::Type type, float emissionRate = 30.f);

	float getEmissionRate();
};


#endif