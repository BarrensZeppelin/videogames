#ifndef LD31_PARTICLE
#define LD31_PARTICLE

#include <SFML/Graphics.hpp>

struct Particle {
	typedef std::function<void(Particle& p, sf::Time dt)> Affector;

	enum Type
	{
		Snow,
		Blood,
		Water,
		ParticleCount
	};

	sf::Vector2f	position;
	sf::Color		color;
	sf::Time		lifetime;
	sf::Vector2f	velocity;
};

#endif