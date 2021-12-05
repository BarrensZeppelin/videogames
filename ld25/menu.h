void updateSelection(vector<sf::Text *> items, int New, int Old) {
	items[Old]->setColor(sf::Color(255, 255, 255, 255));
	items[Old]->setScale(1.f, 1.f);
	items[New]->setColor(sf::Color(190, 221, 232, 255));
	items[New]->setScale(1.2f, 1.2f);

	selectSnd.play();
}

void do_menu() {
	bool done = false;
	vector<sf::Text *> items;


	for(unsigned int i=0; !done; i++) {
		stringstream levelstream;
			levelstream << "levels/" << i << "-0" << ".txt";
			ifstream file(levelstream.str().c_str());
			if(file) {
				stringstream textstream;
				textstream << "Level: " << i;
				items.push_back(new sf::Text(textstream.str(), helperFont, 24));
			} else {
				done = true;
			}
	}

	int oldSelection = 0;
	int selection = 0;

	for(unsigned int i=0; i<items.size(); i++) {
		items[i]->setPosition((i%4+1)*200, (floor(float(i/4))+2)*150);
	}

	updateSelection(items, 0, 0);

	sf::Sprite title(titleTexture);
	title.setPosition(10, 10);


	bool crash = false;
	sf::Clock CRASH;
	sf::Clock playClock;
	while(bzsflegacy::game::currentState == MENU) {
		sf::Event Event;
		while(bzsflegacy::game::window->pollEvent(Event)) {
			switch(Event.type) {
				case sf::Event::Closed:
					bzsflegacy::game::currentState = DIE;
					break;

				case sf::Event::KeyPressed:
					if(Event.key.code == sf::Keyboard::Return) {
						level[0] = selection;
						level[1] = 0;
						bzsflegacy::game::currentState = RUNNING;
					} else if(Event.key.code == sf::Keyboard::D || Event.key.code == sf::Keyboard::Right) {
						oldSelection = selection;
						if(selection != items.size()-1) {
							selection++;
						} else {
							selection = 0;
						}
						updateSelection(items, selection, oldSelection);
					} else if(Event.key.code == sf::Keyboard::A || Event.key.code == sf::Keyboard::Left) {
						oldSelection = selection;
						if(selection != 0) {
							selection--;
						} else {
							selection = items.size()-1;
						}
						updateSelection(items, selection, oldSelection);
					}
					break;
			}
		}

		if(CRASH.getElapsedTime().asSeconds() > 1) {
			crash = true;
		}

		if(crash) {
			title.setPosition(10+rand()%20-10, 10+rand()%20-10);
		}

		if(playClock.getElapsedTime().asSeconds() > 0.6f) {
			menuSnd.play();
			playClock.restart();
		}

		bzsflegacy::game::window->setView(bzsflegacy::game::window->getDefaultView());
		bzsflegacy::game::window->clear(sf::Color(0, 0, 0, 255));

		bzsflegacy::game::window->draw(title);
		for(unsigned int i=0; i<items.size(); i++) {
			bzsflegacy::game::window->draw(*items[i]);
		}

		bzsflegacy::game::window->display();
	}

	for(unsigned int i=0; i<items.size(); i++) {
		delete items[i];
	}
	items.clear();
}
