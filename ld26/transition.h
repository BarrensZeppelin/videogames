
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}



void transition() {
	
	std::string transitionString = transitions[currentTransition];
	
	std::vector<std::string> splitString = split(transitionString, '\n');


	std::vector<sf::Text> textObjects;
	for(sf::Uint8 i = 0; i<splitString.size(); i++) {
		std::string s = splitString[i];

		sf::Text t(s, font_transition, 30);
		t.setOrigin(t.getLocalBounds().width/2, 0);
		t.setPosition(640, (currentTransition==5 ? 50 : 150)+i*40);
		t.setColor(sf::Color(255, 255, 255, 0));
		
		textObjects.push_back(t);
	}

	sf::View view(sf::Vector2f(640, 360), sf::Vector2f(1280, 720));
	bzsflegacy::game::window->setView(view);


	sf::Image img_fader;
	img_fader.create(1280, 720, sf::Color(0, 0, 0, 255));

	sf::Texture tex_fader;
	tex_fader.loadFromImage(img_fader);

	sf::Sprite spr_fader(tex_fader);
	spr_fader.setColor(sf::Color(255, 255, 255, 0));
	

	sf::Text text_skip("Press space if 2deep4u..", font_default, 20);
	text_skip.setColor(sf::Color(255, 255, 255, 20));
	text_skip.setOrigin(0, text_skip.getLocalBounds().height);
	text_skip.setPosition(20, 700);

	int state = -1;

	bool done = false;

	sf::Clock clock;
	while(bzsflegacy::game::currentState == TRANSITION && !done) {
		
		sf::Event Event;
		while(bzsflegacy::game::window->pollEvent(Event)) {
			switch(Event.type) {
				case sf::Event::Closed:
					bzsflegacy::game::currentState = DIE;
					break;

				case sf::Event::KeyPressed:
					if(Event.key.code == sf::Keyboard::Escape) {
						bzsflegacy::game::currentState = DIE;
					} else if(Event.key.code == sf::Keyboard::Space) {
						text_skip.setColor(sf::Color(255, 255, 255, 200));
						state = textObjects.size();
					}	
					break;
			}
		}
		
		if(state == -1) {
			if(clock.getElapsedTime().asSeconds()>=1) {
				clock.restart();
				state++;
			}
		} else {
			if(state < textObjects.size()) {
				sf::Int32 elapsed = clock.getElapsedTime().asMilliseconds();
				if(elapsed<1000) {
					textObjects[state].setColor(sf::Color(255, 255, 255, 255*(float(elapsed)/1000)));
				} else {
					textObjects[state].setColor(sf::Color(255, 255, 255, 255));
					clock.restart();
					state++;
				}
			} else {
				float elapsed = clock.getElapsedTime().asSeconds();
				if(currentTransition < 5) {
					if(clock.getElapsedTime().asSeconds()>= 1) {
						if(clock.getElapsedTime().asSeconds() < 2) {
							spr_fader.setColor(sf::Color(255, 255, 255, 255*(clock.getElapsedTime().asSeconds()-1.f)));
						} else {
							spr_fader.setColor(sf::Color(255, 255, 255, 255));
						}
					}

					if(clock.getElapsedTime().asSeconds()>=2) {
						if(currentTransition == 0 || currentTransition == 2) {
							bzsflegacy::game::currentState = LEVEL0;
						} else if(currentTransition == 1 || currentTransition == 3) {
							bzsflegacy::game::currentState = LEVEL2;
						} else if(currentTransition == 4) {
							done = true;
						}
						currentTransition++;
					}
				} else {
					if(clock.getElapsedTime().asSeconds()>= 5) {
						if(clock.getElapsedTime().asSeconds() < 7) {
							spr_fader.setColor(sf::Color(255, 255, 255, 255*((clock.getElapsedTime().asSeconds()-5.f)/2)));
						} else {
							spr_fader.setColor(sf::Color(255, 255, 255, 255));
						}

						if(clock.getElapsedTime().asSeconds() >= 7) {
							currentTransition = 0;
							bzsflegacy::game::currentState = INTRO;
						}
					}
				}
			}
		}
		


		bzsflegacy::game::window->clear(sf::Color(0,0,0,255));

		for(std::vector<sf::Text>::iterator iter = textObjects.begin(); iter!=textObjects.end(); iter++) {
			bzsflegacy::game::window->draw(*iter);
		}
		
		bzsflegacy::game::window->draw(text_skip);
		bzsflegacy::game::window->draw(spr_fader);
		
		bzsflegacy::game::window->display();
	}
}