

void boss_level() {
	//cout << "we are bos";

	sf::Sprite background(backgroundTexture);
	background.scale(3.f,3.f);
	background.setPosition(-400, -400);

	tbc::InitCollisionTiles(500, 500);

	vector<bzsf::Drawable *> walls;
	for(unsigned int i=1; i<9; i++) {
		for(unsigned int u=1; u<6; u++) {
			if(i==1 || i == 8 || u == 1 || u == 5) {
				walls.push_back(new bzsf::Drawable());
				walls[walls.size()-1]->setTile(Walls->get(rand()%3, rand()%3));
				walls[walls.size()-1]->setPosition(i*32, u*32);
			}
		}
	}

	sf::Sprite cpu(cpuTexture);
	cpu.setPosition(150, 100);

	vector<bzsf::Drawable> theExplos;
	theExplos.resize(3);

	sf::Text youWonText("You successfully hacked the CPU!", helperFont, 30);
	youWonText.setRotation(-10);
	youWonText.setPosition(-200, 20);
	youWonText.setColor(sf::Color(0,0,0,255));

	sf::Text cpuText("CPU", helperFont, 20);
	cpuText.setPosition(150, 100);
	cpuText.setColor(sf::Color(0,0,0,255));

	sf::View view;
	view.reset(sf::FloatRect(640, 360, 1280, 720));
	view.zoom(0.5f);
	view.setCenter(100, 100);

	bool done = false;
	int phase = 1;
	sf::Clock theClock;
	sf::Clock deltaClock;
	float mDelta;
	while(!done && bzsflegacy::game::currentState == BOSS) {
		mDelta = deltaClock.restart().asSeconds();
		sf::Event Event;
		while(bzsflegacy::game::window->pollEvent(Event)) {
			switch(Event.type) {
				case sf::Event::Closed:
					bzsflegacy::game::currentState = DIE;
					break;

				case sf::Event::KeyPressed:
					if(phase == 2) done = true;
					break;
			}
		}

		if((theClock.getElapsedTime().asSeconds() >= 4 && phase==1)) {
			phase++;
			if(phase==2) {
				for(unsigned int i=0; i<3; i++) {
					bzsf::Animation * theAnim = new bzsf::Animation(sf::Vector2u(32, 32), exploTexture, sf::milliseconds(rand() % 80 + 40), true);
					theExplos[i].setAnimation(*theAnim);
					theExplos[i].setPosition(155, 110);
					float numb = (rand()%1000)/1000;
					theExplos[i].setScale(numb + 1.5f, numb + 1.5f);
					theExplos[i].setOrigin(16, 16);
				}
			}
		}

		if(phase==2 && theClock.getElapsedTime().asSeconds() >= 0.2f) {
			theClock.restart();
			for(unsigned int i=0; i<3; i++) {
				theExplos[i].setPosition(theExplos[i].getPosition().x+rand()%5-2,theExplos[i].getPosition().y+rand()%5-2);
			}

			exploSnd.play();
		}

		////RENDER
		bzsflegacy::game::window->setView(view);
		bzsflegacy::game::window->clear(sf::Color(0, 0, 0, 255));

		bzsflegacy::game::window->draw(background);
		for(unsigned int i=0; i<walls.size(); i++) {
			bzsflegacy::game::window->draw(*walls[i]);
		}
		bzsflegacy::game::window->draw(youWonText);
		bzsflegacy::game::window->draw(cpu);
		bzsflegacy::game::window->draw(cpuText);
		if(phase == 2) {
			for(unsigned int i=0; i<3; i++) {
				theExplos[i].update(sf::seconds(mDelta));
				bzsflegacy::game::window->draw(theExplos[i]);
			}
		}

		bzsflegacy::game::window->display();

		stringstream frameStream;
		frameStream << "ANONHACK.EXE  -  " << deltaClock.getElapsedTime().asMicroseconds() << " Microseconds  --  FPS: " << 1000000.0f / deltaClock.getElapsedTime().asMicroseconds();
		bzsflegacy::game::window->setTitle(frameStream.str());
	}

	for(unsigned int i=0; i<walls.size(); i++) {
		delete walls[i];
	}
	walls.clear();

	bzsflegacy::game::currentState = MENU;
}
