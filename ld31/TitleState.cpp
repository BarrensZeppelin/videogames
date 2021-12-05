#include "TitleState.hpp"
#include "Utility.hpp"

TitleState::TitleState(StateStack& stack, Context context)
	: State(stack, context)
	, bg(sf::Vector2f(Application::settings.windowSize.x, Application::settings.windowSize.y))
	, startText("Press SPACE to start!", context.fonts->get(Font::Emulator), 60)
	, prog(0.f)
	, ico(sf::Vector2f(200, 200))
	, bzepText("@BarrensZeppelin", context.fonts->get(Font::Emulator), 20) {

	bg.setTexture(&context.textures->get(Texture::TitleSplash));
	
	CenterOrigin(startText);
	startText.setPosition(600, 500);
	startText.setColor(sf::Color(8, 202, 27));

	ico.setTexture(&context.textures->get(Texture::BZepIco));
	CenterOrigin(ico);
	ico.setPosition(400, 100);

	CenterOrigin(bzepText);
	bzepText.setPosition(420, 180);
	bzepText.setColor(sf::Color());
}

bool TitleState::handleEvent(const sf::Event& e) {
	switch(e.type) {
	case sf::Event::KeyPressed:
		if(e.key.code == sf::Keyboard::Space) {
			requestStackPop();
			requestStackPush(States::GAMESTATE);
		}
		break;
	}

	return true;
}

bool TitleState::update(sf::Time dt) {
	prog += 3.f * dt.asSeconds();

	startText.setPosition(600, 500 + std::sin(prog * bzsf::PI) * 20);
	startText.setColor(sf::Color(8, 202 + std::sin(prog * bzsf::PI) * 30, 27));

	return true;
}

void TitleState::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(bg, states);
	target.draw(startText, states);

	target.draw(ico, states);
	target.draw(bzepText, states);
}