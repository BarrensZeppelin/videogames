#ifndef LD31_WORLD
#define LD31_WORLD

#include <bzsf/SFML_Snips.hpp>
#include "SceneNode.hpp"
#include "CommandQueue.hpp"

#include "BloomEffect.hpp"

class Eagle;

class World {
private:
	const int SEALEVEL;
	const int SEALINE;
	const sf::Color bgColor = sf::Color(31, 38, 119);

	Context context;

	sf::Sprite bg;
	sf::RectangleShape seaLine;
	sf::RectangleShape tintOverlay;

	sf::RenderTexture renderTexture;
	sf::RenderTexture sceneTexture;

	sf::View aboveSeaView;
	sf::View seaReflectionView;
	sf::View seaView;

	float wind;
	float windStart;
	float windTarget;
	sf::Clock windClock;
	sf::Time windTime;

	float cloudPos;
	sf::Sprite clouds;

	sf::Clock fishClock;
	sf::Time timeToFish;
	sf::Clock coinClock;

	Eagle* player; 

private:
	CommandQueue commandQueue;

	enum Commands {
		ApplyWind,
		ApplyParticleWind,
		DestroyOutside
	};

	std::map<Commands, Command> commands;

private:
	enum Layer {
		Background,
		Lower,
		Upper,
		LayerCount
	};

	SceneNode sceneGraph;
	std::array<SceneNode*, LayerCount> sceneLayers;

	BloomEffect bloomEffect;

private:
	void buildScene();
	void initializeCommands();

	void spawnPickups(sf::Time dt);
	void calculateWind(sf::Time dt);
	
	void handleCollisions();

public:
	World(Context context);

	CommandQueue& getCommandQueue();
	Eagle& getPlayer();

	void update(sf::Time dt);
	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default);
};

#endif