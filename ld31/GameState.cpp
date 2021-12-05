#include "Application.hpp"
#include "GameState.hpp"
#include "PauseState.hpp"
#include "World.hpp"
#include "Eagle.hpp"

GameState::GameState(StateStack& stack, Context context) : State(stack, context), world(context) {

}


bool GameState::handleEvent(const sf::Event& e) {
	switch(e.type) {
	case sf::Event::KeyPressed:
		if(e.key.code == sf::Keyboard::T)
			cScheme = !cScheme;
			world.getPlayer().controlScheme = cScheme;
		break;

	case sf::Event::LostFocus:
		requestStackPush(States::PAUSESTATE);
		break;
	}
	return true;
}


bool GameState::update(sf::Time dt) {
	// Restart when player dead
	if(world.getPlayer().isMarkedForRemoval()) {
		lastScore = world.getPlayer().score;
		requestStackPush(States::GAMEOVERSTATE);
		return false;
	}
	
	world.update(dt);

	return true;
}

void GameState::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	world.draw(target, states);
}