
void level_0() {

	music_rain.play();
	music_rain.setVolume(50);

	particleSystem PS;

	sf::Font font; font.loadFromFile("imgs/transition.ttf");
	sf::Text text_FPS("0 FPS", font);

	sf::Image img_player;
	img_player.create(24, 24);

	sf::Texture tex_player;
	tex_player.loadFromImage(img_player);

	sf::Sprite spr_player(tex_player);
	spr_player.setOrigin(12, 12);
	spr_player.setPosition(120, 300);

	sf::Image img_flash;
	img_flash.create(640, 360, sf::Color(255, 255, 255));

	sf::Texture tex_flash;
	tex_flash.loadFromImage(img_flash);

	sf::Sprite spr_flash(tex_flash);
	spr_flash.setColor(sf::Color(255, 255, 255, 0));


	sf::Sprite spr_lightning(tex_lightning);
	spr_lightning.setColor(sf::Color(255,255,255,0));
	spr_lightning.setOrigin(30, 0);
	spr_lightning.setPosition(120, -15);

	sf::Image img_fader;
	img_fader.create(640, 360, sf::Color(0, 0, 0, 255));

	sf::Texture tex_fader;
	tex_fader.loadFromImage(img_fader);

	sf::Sprite spr_fader(tex_fader);
	spr_fader.setColor(sf::Color(255, 255, 255, 255));

	bool fadedin = false;


	sf::Sprite spr_ground(tex_ground0);
	spr_ground.setPosition(0, 3);


	sf::Sprite spr_rain(tex_rain0);
	spr_rain.setColor(sf::Color(90, 90, 90, 90));
	spr_rain.setPosition(0, -38);

	sf::Sprite spr_bush(tex_bush);
	spr_bush.setPosition(70, 270);


	float vSpeed = 0;
	float hSpeed = 0;

	bool lightning = false;

	sf::View view(sf::Vector2f(320, 180), sf::Vector2f(640, 360));

	sf::Vector2f viewSize = view.getSize();

	sf::Clock clock;
	sf::Clock frameClock;
	sf::Clock rainClock;
	float mDelta;
	while(bzsflegacy::game::currentState == LEVEL0) {

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
					} else if(Event.key.code == sf::Keyboard::W && spr_player.getPosition().y==300) { //Add more options
						vSpeed = -60;
						snd_hop.play();
					}
					break;
			}
		}

		vSpeed += 450*mDelta;
		spr_player.setPosition(spr_player.getPosition().x, spr_player.getPosition().y+vSpeed*mDelta);
		if(spr_player.getPosition().y > 300) {
			spr_player.setPosition(spr_player.getPosition().x, 300);
		}
		if(hSpeed != 0) {
			spr_player.setRotation(spr_player.getRotation()+720*mDelta);
			spr_player.setPosition(spr_player.getPosition().x+hSpeed*mDelta, spr_player.getPosition().y);
		}

		if(spr_player.getPosition().x > 640) {
			if(spr_fader.getColor().a == 255) {
				music_rain.setVolume(20);
				bzsflegacy::game::currentState = LEVEL1;
			} else {
				sf::Uint16 alpha = spr_fader.getColor().a + 255*2*mDelta;
				if(alpha>=255) {
					spr_fader.setColor(sf::Color(255, 255, 255, 255));
				} else {
					spr_fader.setColor(sf::Color(255,255,255, alpha));
					music_rain.setVolume(50-30*(float(alpha)/255));
				}
			}
		}


		sf::Int32 eElapsed = rainClock.getElapsedTime().asMilliseconds();
		if(eElapsed % 200 < 100) {
			spr_rain.setTexture(tex_rain0);
		} else {
			spr_rain.setTexture(tex_rain1);
		}

		sf::Int32 elapsed = clock.getElapsedTime().asMilliseconds();
		if(!fadedin) {
			if(elapsed < 1500) {
				spr_fader.setColor(sf::Color(255, 255, 255, 255 - 255*(float(elapsed)/1500)));
			} else {
				spr_fader.setColor(sf::Color(255, 255, 255, 0));
				fadedin = true;
				clock.restart();
			}
		} else if(!lightning) {
			if(clock.getElapsedTime().asSeconds()>=3) {
				lightning = true;
				//spr_flash.setColor(sf::Color(255, 255, 255, 255));
			}
		} else {
			elapsed = elapsed - 3000;


			if(elapsed < 60) {
				spr_flash.setColor(sf::Color(255, 255, 255, 255*sin(PI/2 * (float(elapsed)/60))));
			} else if(elapsed >= 100 && elapsed <300) {
				if(hSpeed == 0) {
					hSpeed = 400+rand()%200;
					vSpeed = -350 - rand()%100;
					snd_Lightning.play();
					spr_lightning.setColor(sf::Color(255,255,255,255));
					view.zoom(0.8);
					PS.fuel(-PI/2, PI, 30, sf::Vector2f(spr_player.getPosition().x , spr_player.getPosition().y+12), 0.75f, sf::Color(255, 230, 0, 255));
				}
				spr_flash.setColor(sf::Color(255, 255, 255, 255 - 255*(float(elapsed-100)/200)));
				spr_lightning.setColor(sf::Color(255, 255, 255, 255 - 255*(float(elapsed-100)/200)));
				view.setSize(view.getSize().x + (viewSize.x - view.getSize().x)*(float(elapsed-100)/200), view.getSize().y + (viewSize.y - view.getSize().y)*(float(elapsed-100)/200));
				view.setCenter(320 + rand()%(1+int((1.f-(float(elapsed-100)/200)) * 20)) - (1.f-(float(elapsed-100)/200))/2 , 180 + rand()%(1+int((1.f-(float(elapsed-100)/200)) * 20)) - (1.f-(float(elapsed-100)/200)/2));

			} else if(elapsed >= 300) {
				spr_flash.setColor(sf::Color(255, 255, 255, 0));
				spr_lightning.setColor(sf::Color(255,255,255,0));
			}
		}

		bzsflegacy::game::window->setView(view);
		bzsflegacy::game::window->clear(sf::Color(8,8,8));

		bzsflegacy::game::window->draw(spr_ground);
		bzsflegacy::game::window->draw(spr_rain);
		bzsflegacy::game::window->draw(spr_flash);



		bzsflegacy::game::window->draw(spr_player);
		bzsflegacy::game::window->draw(spr_bush);


		bzsflegacy::game::window->draw(spr_lightning);



		bzsflegacy::game::window->draw(spr_fader);

		PS.draw(mDelta);


		/*bzsflegacy::game::window->setView(bzsflegacy::game::window->getDefaultView());
		std::stringstream fps;
		fps << "FPS: " << 1000000.0f / frameClock.getElapsedTime().asMicroseconds();
		text_FPS.setString(fps.str());
		bzsflegacy::game::window->draw(text_FPS);*/

		bzsflegacy::game::window->display();
	}
}
