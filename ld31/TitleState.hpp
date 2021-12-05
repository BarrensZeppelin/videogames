#include "Application.hpp"


class TitleState : public State {
private:
	sf::RectangleShape bg;
	sf::Text startText;

	sf::RectangleShape ico;
	sf::Text bzepText;

	float prog;

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const;

public:
	TitleState(StateStack& stack, Context context);

	bool handleEvent(const sf::Event& e);
	bool update(sf::Time dt);
};