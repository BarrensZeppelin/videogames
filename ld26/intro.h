

void game_intro() {
	music_rain.setVolume(0);
	music_rain.setLoop(true);
	music_rain.play();

	sf::Image img_desaturated;
	img_desaturated.create(1280, 720);
	bzsf::Desaturator * des_screen = 0;
	sf::Texture tex_desaturated; tex_desaturated.loadFromImage(img_desaturated);
	sf::Sprite spr_desaturated(tex_desaturated);


	sf::Image img_fader;
	img_fader.create(640, 360, sf::Color(255, 255, 255, 255));

	sf::Texture tex_fader;
	tex_fader.loadFromImage(img_fader);

	sf::Sprite spr_background(tex_fader);
	spr_background.setColor(sf::Color(255, 255, 255, 0));

	sf::Sprite spr_fader(tex_fader);
	spr_fader.setColor(sf::Color(0,0,0,0));


	sf::Sprite spr_title(tex_title);
	spr_title.setColor(sf::Color(255, 255, 255, 0));


	sf::Text text_guide("Press <enter> for Story Mode\nPress <space> for no BS (Score Attack) Mode", font_default);
	text_guide.setOrigin(0, text_guide.getLocalBounds().height);
	text_guide.setPosition(20, 700);
	text_guide.setColor(sf::Color(255, 255, 255, 0));

	sf::Uint8 state = 0;

	sf::Sprite spr_rain(tex_rain0);
	spr_rain.setColor(sf::Color(90, 90, 90, 90));

	sf::Image img_player;
	img_player.create(24, 24);

	sf::Texture tex_player;
	tex_player.loadFromImage(img_player);

	sf::Sprite spr_player(tex_player);
	spr_player.setOrigin(12, 12);
	spr_player.setPosition(120, -20);
	
	float vSpeed = 0;

	sf::View view(sf::Vector2f(320, 180), sf::Vector2f(640, 360));
	sf::View mainView(sf::Vector2f(640, 360), sf::Vector2f(1280, 720));
	sf::Vector2f viewSize = mainView.getSize();

	sf::Clock clock;
	sf::Clock frameClock;
	sf::Clock rainClock;
	float mDelta = 0;
	while(bzsflegacy::game::currentState == INTRO) {
		
		mDelta = frameClock.restart().asSeconds();

		sf::Event Event;
		while(bzsflegacy::game::window->pollEvent(Event)) {
			switch(Event.type) {
				case sf::Event::Closed:
					bzsflegacy::game::currentState = DIE;
					break;

				case sf::Event::KeyPressed:
					if(Event.key.code == sf::Keyboard::Escape) {
						bzsflegacy::game::currentState = DIE;
					} else if(state == 3) {
						if(Event.key.code == sf::Keyboard::Return) {
							snd_blip.play();
							state++;
							vSpeed = 200;
							clock.restart();
							text_guide.setColor(sf::Color(255, 255, 255, 0));

							mode = STORY;
						} else if(Event.key.code == sf::Keyboard::Space) {
							snd_blip.play();
							vSpeed = 200;
							state++;
							text_guide.setColor(sf::Color(255, 255, 255, 0));
							clock.restart();
							mode = SCORE;
						}
					}
					break;
			}
		}

		if(rainClock.getElapsedTime().asMilliseconds() % 200 < 100) {
			spr_rain.setTexture(tex_rain0);
		} else {
			spr_rain.setTexture(tex_rain1);
		}
		
		sf::Int32 elapsed = clock.getElapsedTime().asMilliseconds();
		if(state == 0) {
			if(elapsed >= 500) {
				if(elapsed < 2000) {
					sf::Int32 time = elapsed-500;
					spr_background.setColor(sf::Color(255, 255, 255, (float(time)/1500) * 255));
				} else {
					spr_background.setColor(sf::Color(255, 255, 255, 255));
					state++;
					clock.restart();
				}
			}
		} else if(state == 1) {
			if(elapsed < 2000) {
				spr_title.setColor(sf::Color(255, 255, 255, 255*(float(elapsed)/2000)));
			} else {
				spr_title.setColor(sf::Color(255, 255, 255, 255));
				state++;
				clock.restart();
			}
		} else if(state == 2) {
			if(elapsed < 1000) {
				spr_background.setColor(sf::Color(255, 255, 255, 255- 255*(float(elapsed)/1000)));
				music_rain.setVolume(50*(float(elapsed)/1000));
				text_guide.setColor(sf::Color(255, 255, 255, 170*(float(elapsed)/1000)));
			} else {
				spr_background.setColor(sf::Color(255, 255, 255, 0));
				text_guide.setColor(sf::Color(255, 255, 255, 170));
				music_rain.setVolume(50);
				state++;
				clock.restart();
			}
		} else if(state == 4) {
			vSpeed += 200*mDelta;

			spr_player.setPosition(spr_player.getPosition().x, spr_player.getPosition().y + vSpeed*mDelta);
			spr_player.setRotation(spr_player.getRotation() + 180*mDelta);

			if(spr_player.getPosition().y >= 500) {
				state++;
				clock.restart();
			}
		} else if(state == 5) {
			if(des_screen == 0) {
				img_desaturated = bzsflegacy::game::window->capture();
				tex_desaturated.loadFromImage(img_desaturated);

				des_screen = new Desaturator(img_desaturated, 1500);
			}

			if(elapsed >= 1500) {
				state++;
				clock.restart();
				snd_hit.play();
			}
		} else if(state == 6) {
			if(elapsed < 1000) {
				music_rain.setVolume(50 - 50*(float(elapsed)/1000));
			} else {
				state++;
				clock.restart();
			}
		} else if(state == 7) {
			if(elapsed < 1000) {
				spr_fader.setColor(sf::Color(0,0,0, 255*(float(elapsed)/1000)));
			} else {
				spr_fader.setColor(sf::Color(0,0,0, 255));

				if(mode == STORY) bzsflegacy::game::currentState = TRANSITION; else bzsflegacy::game::currentState = SCORE_ATTACK;
			}
		}
		

		bzsflegacy::game::window->setView(view);
		bzsflegacy::game::window->clear(sf::Color(8,8,8));

		if(state>=2) {
			bzsflegacy::game::window->draw(spr_rain);
		}

		bzsflegacy::game::window->draw(spr_player);

		bzsflegacy::game::window->draw(spr_background);

		bzsflegacy::game::window->draw(spr_title);

		

		bzsflegacy::game::window->setView(mainView);		
		if(state >= 5 && des_screen != 0) {
			tex_desaturated.update(des_screen->getPixels());
			bzsflegacy::game::window->draw(spr_desaturated);
		}

		bzsflegacy::game::window->setView(view);
		bzsflegacy::game::window->draw(spr_fader);

		bzsflegacy::game::window->setView(bzsflegacy::game::window->getDefaultView());

		bzsflegacy::game::window->draw(text_guide);

		bzsflegacy::game::window->display();
	}

	delete des_screen;
}