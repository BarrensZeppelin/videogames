#include "EmitterNode.hpp"



class SnowNode : public EmitterNode {
private:
	void emitParticles(sf::Time dt);

public:
	SnowNode();
};