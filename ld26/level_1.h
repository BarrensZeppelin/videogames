void level_1() {

	music_rain.setVolume(20);


	sf::Font font; font.loadFromFile("imgs/transition.ttf");
	sf::Text text_FPS("0 FPS", font);

	sf::Text text_wow("!", font, 40);
	text_wow.setPosition(100, 150);
	bool wow_played = false;
	text_wow.setColor(sf::Color(255, 255, 255, 0));

	sf::Image img_player;
	img_player.create(24, 24);

	sf::Texture tex_player;
	tex_player.loadFromImage(img_player);

	sf::Sprite spr_player(tex_player);
	spr_player.setOrigin(12, 12);
	spr_player.setPosition(-50, 0);

	float hSpeed = 0;
	float vSpeed = 0;


	sf::Image img_fader;
	img_fader.create(640, 360, sf::Color(0, 0, 0, 255));

	sf::Texture tex_fader;
	tex_fader.loadFromImage(img_fader);

	sf::Sprite spr_fader(tex_fader);
	spr_fader.setColor(sf::Color(255, 255, 255, 255));

	sf::Sprite spr_rain(tex_rain0);
	spr_rain.setColor(sf::Color(90, 90, 90, 90));
	spr_rain.setPosition(3, -222);

	sf::Sprite spr_rain1(tex_rain0);
	spr_rain1.setColor(sf::Color(90, 90, 90, 90));
	spr_rain1.setPosition(78, 138);

	sf::Sprite spr_ground(tex_ground1);

	bool fadedin = false;
	bool fadedout = false;

	sf::View view(sf::Vector2f(320, 180), sf::Vector2f(640, 360));
	bzsflegacy::game::window->setView(view);

	sf::Clock clock;
	sf::Clock frameClock;
	float mDelta;
	while(bzsflegacy::game::currentState == LEVEL1) {

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
					}
					break;
			}
		}

		sf::Int32 elapsed = clock.getElapsedTime().asMilliseconds();
		if(elapsed % 200 < 100) {
			spr_rain.setTexture(tex_rain0);
			spr_rain1.setTexture(tex_rain0);
		} else {
			spr_rain.setTexture(tex_rain1);
			spr_rain1.setTexture(tex_rain1);
		}


		if(elapsed < 1000) {
			spr_fader.setColor(sf::Color(255, 255, 255, 255 - 255*(float(elapsed)/1000)));
			music_rain.setVolume(20 + 30*(float(elapsed)/1000));
		} else if(!fadedin) {
			spr_fader.setColor(sf::Color(255, 255, 255, 0));
			music_rain.setVolume(50);

			hSpeed = 500;
			vSpeed = 100;

			fadedin = true;
		}

		if(fadedin) {
			vSpeed += 500*mDelta;

			spr_player.setPosition(spr_player.getPosition().x + hSpeed*mDelta, spr_player.getPosition().y+ vSpeed*mDelta);
			spr_player.setRotation(spr_player.getRotation() + 720.f*mDelta);

			/*if(spr_player.getPosition().x > 75 && !wow_played) {
				//snd_wow.play();
				text_wow.setColor(sf::Color(255, 255, 255, 255));
				wow_played = true;
			}*/
		}

		if(spr_player.getPosition().y > 360) {
			if(spr_fader.getColor().a == 255) {
				music_rain.stop();
				bzsflegacy::game::currentState = TRANSITION;
			} else {
				sf::Uint16 alpha = spr_fader.getColor().a + 255*mDelta;
				if(alpha>=255) {
					spr_fader.setColor(sf::Color(255, 255, 255, 255));
				} else {
					spr_fader.setColor(sf::Color(255,255,255, alpha));
					music_rain.setVolume(50-50*(float(alpha)/255));
				}
			}
		}

		bzsflegacy::game::window->setView(view);
		bzsflegacy::game::window->clear(sf::Color(8,8,8));


		bzsflegacy::game::window->draw(spr_rain);
		bzsflegacy::game::window->draw(spr_rain1);

		bzsflegacy::game::window->draw(spr_ground);



		bzsflegacy::game::window->draw(spr_player);

		bzsflegacy::game::window->setView(bzsflegacy::game::window->getDefaultView());
		bzsflegacy::game::window->draw(text_wow);

		bzsflegacy::game::window->setView(view);
		bzsflegacy::game::window->draw(spr_fader);


		/*bzsflegacy::game::window->setView(bzsflegacy::game::window->getDefaultView());
		std::stringstream fps;
		fps << "FPS: " << 1000000.0f / frameClock.getElapsedTime().asMicroseconds();
		text_FPS.setString(fps.str());
		bzsflegacy::game::window->draw(text_FPS);*/

		bzsflegacy::game::window->display();
	}
}
