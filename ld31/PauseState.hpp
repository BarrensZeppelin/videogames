#ifndef LD31_PAUSESTATE
#define LD31_PAUSESTATE

#include "Application.hpp"

class PauseState : public State {
private:
	sf::RectangleShape bg;
	sf::Text text;

	bool hasFocus;

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const;

public:
	explicit PauseState(StateStack& stack, Context context);
	
	bool handleEvent(const sf::Event& event);
	bool update(sf::Time dt);
	
};

#endif