#include "Application.hpp"
#include <SFML/Network.hpp>

extern std::string submitName;
extern sf::Http http;

class GameOverState : public State {
private:
	sf::RectangleShape bg;

	sf::Text last;
	sf::Text best;

	sf::Text startAgain;

	sf::Text rankText;
	sf::Text submitEnter;

	std::string scoreString;

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const;

public:
	GameOverState(StateStack& stack, Context context);

	bool handleEvent(const sf::Event& e);
	bool update(sf::Time dt);
};