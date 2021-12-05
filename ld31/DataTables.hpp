#ifndef LD31_TABLES
#define LD31_TABLES

#include <SFML/Graphics.hpp>
#include <vector>
#include "resid.hpp"
#include "Particle.hpp"

class Eagle;

struct PickupData {
	std::function<void(Eagle&)> action;
	Texture::ID texture;
};

struct ParticleData {
	sf::Color color;
	sf::Time lifetime;
	Texture::ID texture;
	sf::Vector2f velocityRangeX;
	sf::Vector2f velocityRangeY;

	std::vector<Particle::Affector> affectors;
};


std::vector<PickupData> initializePickupData();
std::vector<ParticleData> initializeParticleData();

#endif