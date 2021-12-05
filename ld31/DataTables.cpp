#include "DataTables.hpp"

#include "Particle.hpp"
#include "Pickup.hpp"

std::vector<PickupData> initializePickupData() {
	std::vector<PickupData> data(Pickup::Type::TypeCount);

	data[Pickup::Coin].texture = Texture::Coin;
	data[Pickup::Coin].action = [](Eagle& player) {player.accumulatedScore += 1; };

	data[Pickup::Fish].action = [](Eagle& player) {
		player.energy = std::min(player.energy + 25.f, 200.f);
		player.accumulatedScore += 5;
	};

	return data;
}

std::vector<ParticleData> initializeParticleData() {
	std::vector<ParticleData> data(Particle::Type::ParticleCount);


	data[Particle::Snow].color = sf::Color(sf::Color::White);
	data[Particle::Snow].lifetime = sf::seconds(3);
	data[Particle::Snow].texture = Texture::Snow;
	data[Particle::Snow].velocityRangeX = sf::Vector2f(0, 0);
	data[Particle::Snow].velocityRangeY = sf::Vector2f(0, 0);
	data[Particle::Snow].affectors.push_back([](Particle& p, sf::Time dt) {p.position += sf::Vector2f(0, 600.f) * dt.asSeconds(); });

	data[Particle::Blood].color = sf::Color(sf::Color::White);
	data[Particle::Blood].lifetime = sf::seconds(3);
	data[Particle::Blood].texture = Texture::BloodParticle;
	data[Particle::Blood].velocityRangeX = sf::Vector2f(-100, 100);
	data[Particle::Blood].velocityRangeY = sf::Vector2f(-100, 100);
	data[Particle::Blood].affectors.push_back([](Particle& p, sf::Time dt) {p.velocity += sf::Vector2f(0, 400.f) * dt.asSeconds(); });
	data[Particle::Blood].affectors.push_back([](Particle& p, sf::Time dt) {p.position += p.velocity * dt.asSeconds(); });

	data[Particle::Water].color = sf::Color(sf::Color::White);
	data[Particle::Water].lifetime = sf::seconds(1.f);
	data[Particle::Water].texture = Texture::WaterParticle;
	data[Particle::Water].velocityRangeX = sf::Vector2f(-200, 200);
	data[Particle::Water].velocityRangeY = sf::Vector2f(-200, 100);
	data[Particle::Water].affectors.push_back([](Particle& p, sf::Time dt) {p.velocity += sf::Vector2f(0, 400.f) * dt.asSeconds(); });
	data[Particle::Water].affectors.push_back([](Particle& p, sf::Time dt) {p.position += p.velocity * dt.asSeconds(); });

	return data;
}