#include "Application.hpp"
#include "World.hpp"


class GameState : public State {
private:
	mutable World world;

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const;

public:
	GameState(StateStack& stack, Context context);

	bool handleEvent(const sf::Event& e);
	bool update(sf::Time dt);
};