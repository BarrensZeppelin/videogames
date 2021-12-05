#include "PauseState.hpp"
#include "Utility.hpp"

PauseState::PauseState(StateStack& stack, Context context)
	: State(stack, context)
	, hasFocus(false)
	, text("Press SPACE to continue", context.fonts->get(Font::Emulator)) {

	sf::Vector2u wSize = Application::settings.windowSize;
	CenterOrigin(text);
	text.setPosition(wSize.x / 2, wSize.y / 2);
	text.setColor(sf::Color());

	bg.setSize(sf::Vector2f(wSize.x, wSize.y));
	bg.setFillColor(sf::Color(255, 255, 255, 192));
}


bool PauseState::handleEvent(const sf::Event& e) {
	switch(e.type) {
	case sf::Event::GainedFocus:
		hasFocus = true;
		break;

	case sf::Event::LostFocus:
		hasFocus = false;
		break;

	case sf::Event::KeyPressed:
		if(e.key.code == sf::Keyboard::Space && hasFocus) {
			requestStackPop();
		}
		break;
	}

	return false;
}

bool PauseState::update(sf::Time dt) {
	return false;
}

void PauseState::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.setView(getContext().renderWindow->getDefaultView());
	
	target.draw(bg);
	target.draw(text);
}