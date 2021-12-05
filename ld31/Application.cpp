#include "Application.hpp"
#include "GameState.hpp"
#include "PauseState.hpp"
#include "TitleState.hpp"
#include "GameOverState.hpp"
#include <cassert>

float lastScore = 0;
float bestScore = 0;
bool cScheme = false;

Settings::ARMap Settings::AspectRatios = {
	{"4x3", 0},
	{"16x9", 1},
	{"8x5", 2},
	{"16x10", 2}
};

Settings::Settings(std::string AspectRatio,
	sf::Vector2u WindowSize,
	bool Fullscreen) :
	aspectRatio(AspectRatios[AspectRatio]),
	windowSize(WindowSize),
	fullscreen(Fullscreen) {

}




Context::Context(sf::RenderWindow& window,
	TextureStorage& textureStorage,
	FontStorage& fontStorage,
	AnimationStorage& animationStorage,
	SoundPlayer& soundPlayer) :
	renderWindow(&window),
	textures(&textureStorage),
	fonts(&fontStorage),
	animations(&animationStorage),
	soundPlayer(&soundPlayer)
{}

Context::Context() {}




// Initalize static members
Settings Application::settings("16x9",
	sf::Vector2u(1280, 600),
	false);


void Application::load() {
	try {
		textures.load(Texture::Clouds, "res/clouds.png");
		textures.get(Texture::Clouds).setRepeated(true);

		textures.load(Texture::Snow, "res/snoflake.png");
		textures.load(Texture::BloodParticle, "res/bloodParticle.png");
		textures.load(Texture::WaterParticle, "res/waterParticle.png");
			
		textures.load(Texture::ReturnArrow, "res/returnarrow.png");

		textures.load(Texture::Coin, "res/coin.png");

		textures.load(Texture::TopCliff, "res/clifftop.png");
		textures.load(Texture::BottomCliff, "res/cliffbottom.png");

		textures.load(Texture::WaterSplash, "res/watersplash.png");

		textures.load(Texture::TitleSplash, "res/titleSplash.png");
		textures.load(Texture::BZepIco, "res/BZep.png");
		textures.get(Texture::BZepIco).setSmooth(true);

		fonts.load(Font::Arial, "res/arial.ttf");
		fonts.load(Font::Emulator, "res/Emulator.ttf");

		AnimationStruct eagle(sf::Vector2u(2, 1), 300, false, "res/eagle.png");
		animations.insert(std::make_pair(Animation::Eagle, eagle));

		AnimationStruct fish(sf::Vector2u(2, 1), 300, true, "res/fish.png");
		animations.insert(std::make_pair(Animation::Fish, fish));

	} catch(std::runtime_error e) {

		throw;

	}

}


Application::Application() :
window(sf::VideoMode(settings.windowSize.x, settings.windowSize.y), "Ludum Dare 31", sf::Style::Close),
textures(),
fonts(),
animations(),
stateStack(Context(window, textures, fonts, animations, soundPlayer)) {

	window.setKeyRepeatEnabled(false);

	try {
		load();

		registerStates();
		stateStack.pushState(States::TITLESTATE);
	} catch(std::runtime_error e) {
		assert(false); // resource loading error
	}
}


int Application::run() {

	sf::Clock dClock;
	const sf::Time frameCap = sf::seconds(1.f / 15.f);

	while(window.isOpen()) {
		sf::Time dt = std::min(dClock.restart(), frameCap);

		/* Implement some sort of fixed-time step here */
		handleEvents();
		update(dt);
		render();

		if(stateStack.empty())
			window.close();
	}


	return 0;
}

void Application::handleEvents() {

	sf::Event event;
	while(window.pollEvent(event)) {
		stateStack.handleEvent(event);

		if(event.type == sf::Event::Closed)
			window.close();
	}

}


void Application::update(sf::Time& dt) {
	stateStack.update(dt);
}


void Application::render() {

	window.clear(sf::Color(31, 38, 119));

	window.draw(stateStack);

	window.setView(window.getDefaultView());

	window.display();

}


void Application::registerStates() {

	stateStack.registerState<TitleState>(States::TITLESTATE);
	stateStack.registerState<GameState>(States::GAMESTATE);
	stateStack.registerState<PauseState>(States::PAUSESTATE);
	stateStack.registerState<GameOverState>(States::GAMEOVERSTATE);
}