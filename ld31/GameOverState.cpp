#include "GameOverState.hpp"
#include "Utility.hpp"

std::string submitName = std::string();
sf::Http http = sf::Http("http://autistklassen.dk/");

GameOverState::GameOverState(StateStack& stack, Context context)
	: State(stack, context)
	, bg(sf::Vector2f(1280, 600))
	, last("", context.fonts->get(Font::Emulator), 30)
	, best("", context.fonts->get(Font::Emulator))
	, startAgain("Press SPACE to play again!", context.fonts->get(Font::Emulator), 30)
	, rankText("Rank: ", context.fonts->get(Font::Emulator), 32)
	, submitEnter("", context.fonts->get(Font::Emulator), 15) {

	CenterOrigin(startAgain);
	startAgain.setPosition(640, 450);
	startAgain.setColor(sf::Color());

	std::stringstream ss; ss << lastScore;
	scoreString = ss.str();
	last.setString("Your score: " + scoreString);
	last.setColor(sf::Color());

	ss = std::stringstream();
	if(lastScore > bestScore) {
		bestScore = lastScore;

		ss << "New best score: " << bestScore << "!";
		best.setString(ss.str());
		best.setColor(sf::Color(255, 134, 32));
		best.setCharacterSize(50);
	} else {
		ss << "Best score: " << bestScore;
		best.setString(ss.str());
		best.setColor(sf::Color());
	}

	CenterOrigin(last);
	CenterOrigin(best);


	// Fetch online rank
	sf::Http::Request request("LD31Scoreboard.php", sf::Http::Request::Post, "score=" + scoreString);
	sf::Http::Response response = http.sendRequest(request, sf::seconds(0.2f));

	rankText.setColor(sf::Color());

	if(response.getStatus() == sf::Http::Response::Ok) {
		rankText.setString("Online rank: " + response.getBody());
		submitEnter.setString("Press Enter to submit");
	} else
		rankText.setString("Timeout");

	sf::Texture* t = const_cast<sf::Texture*>(&rankText.getFont()->getTexture(32));
	t->setSmooth(false);

	CenterOrigin(rankText);
	rankText.setPosition(640, 325);

	submitEnter.setColor(sf::Color());
	CenterOrigin(submitEnter);
	submitEnter.setPosition(640, 360);

	last.setPosition(640, 100);
	best.setPosition(640, 200);

	bg.setFillColor(sf::Color(255, 255, 255, 220));
}

bool GameOverState::handleEvent(const sf::Event& e) {
	switch(e.type) {
	case sf::Event::KeyPressed:
		if(e.key.code == sf::Keyboard::Space) {
			requestStackClear();
			requestStackPush(States::GAMESTATE);
		}
		break;

	case sf::Event::TextEntered:
		if(rankText.getString() == "Success!")
			break;

		sf::Uint32 uc = e.text.unicode;
		if(uc == 8) {
			if(submitName.length() != 0) {
				submitName = submitName.substr(0, submitName.length() - 1);
				rankText.setString(submitName);
				CenterOrigin(rankText);
			} else getContext().soundPlayer->play(SoundEffect::Pickup);
		} else if(uc == 13) {
			if(submitName == "") {
				rankText.setString("");
				CenterOrigin(rankText);

				submitEnter.setString("Enter name!");
				CenterOrigin(submitEnter);
			
				getContext().soundPlayer->play(SoundEffect::Pickup);
			} else {
				submitEnter.setString("Submitting..");
				CenterOrigin(submitEnter);

				sf::Http::Request request("LD31Scoreboard.php", sf::Http::Request::Post, "name=" + submitName + "&score=" + scoreString);
				sf::Http::Response response = http.sendRequest(request, sf::seconds(1.f));

				if(response.getStatus() == sf::Http::Response::Ok && !cScheme) {
					rankText.setString("Success!");
					rankText.setColor(sf::Color(255, 134, 32));
					CenterOrigin(rankText);

					getContext().soundPlayer->play(SoundEffect::Powerup);
				} else {
					rankText.setString("Error");
					rankText.setColor(sf::Color::Red);
					CenterOrigin(rankText);
				}

				submitEnter.setString("");
			}
		} else if(submitName.length() < 20 && submitEnter.getString() == "Enter name!")  {
			std::stringstream ss; ss << (char)uc;
			submitName.append(ss.str());
			rankText.setString(submitName);
			CenterOrigin(rankText);
		} else getContext().soundPlayer->play(SoundEffect::Pickup);
		break;
	}

	return false;
}

bool GameOverState::update(sf::Time dt) {

	return false;
}

void GameOverState::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.setView(target.getDefaultView());
	
	target.draw(bg);
	target.draw(last);
	target.draw(best);
	target.draw(startAgain);
	target.draw(rankText);
	target.draw(submitEnter);
}