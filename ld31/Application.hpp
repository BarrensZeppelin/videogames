#ifndef LD31_APPLICATION
#define LD31_APPLICATION

#include <map>
#include <bzsf/SFML_Snips.hpp>
#include "resid.hpp"
#include "SoundPlayer.hpp"

extern float lastScore;
extern float bestScore;
extern bool cScheme;

namespace States {
	enum ID {
		TITLESTATE,
		GAMESTATE,
		PAUSESTATE,
		GAMEOVERSTATE
	};
}

struct Settings {
	typedef std::map<std::string, sf::Uint16> ARMap;
	static ARMap AspectRatios;

	sf::Uint16 aspectRatio;
	sf::Vector2u windowSize;
	bool fullscreen;

	Settings(std::string AspectRatio,
		sf::Vector2u WindowSize,
		bool Fullscreen);
};

struct Context {
	sf::RenderWindow* renderWindow;
	TextureStorage* textures;
	FontStorage* fonts;
	AnimationStorage* animations;
	SoundPlayer* soundPlayer;

	Context(sf::RenderWindow& window,
		TextureStorage& textureStorage,
		FontStorage& fontStorage,
		AnimationStorage& animationStorage,
		SoundPlayer& soundPlayer);

	Context();
};




typedef bzsf::StateStack<States::ID, Context> StateStack;
typedef bzsf::State<States::ID, Context> State;


class Application : private sf::NonCopyable {
	/* Vars */
private:
	sf::RenderWindow window;
	TextureStorage textures;
	FontStorage fonts;
	AnimationStorage animations;
	SoundPlayer soundPlayer;

	StateStack stateStack;

public:
	static Settings settings;

	/* Functions */
private:
	void registerStates();

	void handleEvents();
	void update(sf::Time& dt);
	void render();

	void load(); // Resources


public:
	Application();

	int run();
};

#endif