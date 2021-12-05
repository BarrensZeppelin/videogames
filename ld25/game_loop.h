#include <cstring>

player * Player;

void parseLevel(const vector<string>& vec) {
	for(unsigned int i=0; i<vec.size(); i++) {
		for(unsigned int k=0; k<vec[i].size(); k++) {
			if(vec[i].substr(k, 1).compare("7")==0) {
					newObject(new wall(Walls->get(0, 0), k*32, i*32, 4));

			} else if (vec[i].substr(k, 1).compare("8")==0) {
					newObject(new wall(Walls->get(1, 0), k*32, i*32, 4));

			} else if (vec[i].substr(k, 1).compare("9")==0) {
					newObject(new wall(Walls->get(2, 0), k*32, i*32, 4));

			} else if (vec[i].substr(k, 1).compare("4")==0) {
					newObject(new wall(Walls->get(0, 1), k*32, i*32, 4));

			} else if (vec[i].substr(k, 1).compare("5")==0) {
					newObject(new wall(Walls->get(1, 1), k*32, i*32, 4));

			} else if (vec[i].substr(k, 1).compare("6")==0) {
					newObject(new wall(Walls->get(2, 1), k*32, i*32, 4));

			} else if (vec[i].substr(k, 1).compare("1")==0) {
					newObject(new wall(Walls->get(0, 2), k*32, i*32, 4));

			} else if (vec[i].substr(k, 1).compare("2")==0) {
					newObject(new wall(Walls->get(1, 2), k*32, i*32, 4));

			} else if (vec[i].substr(k, 1).compare("3")==0) {
					newObject(new wall(Walls->get(2, 2), k*32, i*32, 4));

			} else if (vec[i].substr(k, 1).compare("P")==0) {
					Player = newPlayerPtr(new player(playerTexture, k*32, i*32, 0));

			} else if (vec[i].substr(k, 1).compare("Q")==0) {
					newObject(new helper(string("Use A & D to move!"), k*32, i*32, 1));

			} else if (vec[i].substr(k, 1).compare("W")==0) {
					newObject(new helper(string("Jump by pressing W"), k*32, i*32, 1));

			} else if (vec[i].substr(k, 1).compare("E")==0) {
					newObject(new helper(string("Go for a long jump!"), k*32, i*32, 1));

			} else if (vec[i].substr(k, 1).compare("R")==0) {
					newObject(new helper(string("You can teleport.\nTry it by holding\nand releasing\n space."), k*32, i*32, 1));

			} else if (vec[i].substr(k, 1).compare("T")==0) {
					newObject(new helper(string("Beware of teleporting into walls.\nWatch the teleport marker!"), k*32, i*32, 1));

			} else if (vec[i].substr(k, 1).compare("Y")==0) {
					newObject(new helper(string("Teleporting resets your\nvertical momentum!"), k*32, i*32, 1));

			} else if (vec[i].substr(k, 1).compare("D")==0) {
					newObject(new death(k*32, i*32, 1));

			} else if (vec[i].substr(k, 1).compare("F")==0) {
					Finish = newObjectPtr(new finish(k*32, i*32, 1));

			}
		}
	}
}

void game_loop() {
	enum GS {LEVEL, EXIT};
	GS gameState = LEVEL;

	sf::View view;
	view.reset(sf::FloatRect(640, 360, 1280, 720));

	switch(gameState) {
		case LEVEL:

			stringstream levelStream;
			levelStream << "levels/" << level[0] << "-" << level[1] << ".txt";
			fstream levelReader(levelStream.str().c_str(), fstream::in);
			assert(levelReader);

			char tempString[1000];
			unsigned int maxLength = 0;
			vector<string> lines;
			while(levelReader) {
				levelReader.getline(tempString, 1000);
				if(strlen(tempString) > maxLength) {maxLength = strlen(tempString);}
				lines.push_back(tempString);
			}
			tbc::InitCollisionTiles(maxLength*32, lines.size()*32);
			parseLevel(lines);


			bzsf::Animation * playerAnim = new bzsf::Animation(sf::Vector2u(12, 32), playerTexture, sf::milliseconds(166), true);

			Player->setAnimation(*playerAnim);
			Player->setOrigin(6, 16);

			sf::Sprite background2(backgroundTexture);
			sf::Sprite background(backgroundTexture);
			background.scale(3.f,3.f);
			background.setPosition(-1000, -600);
			background2.scale(3.f, 3.f);
			background2.setPosition(background.getPosition().x + background.getGlobalBounds().width, background.getPosition().y);

			sf::Font debugFont; debugFont.loadFromFile("imgs/arial.ttf");
			sf::Text debugText("", debugFont, 12);
			debugText.setColor(sf::Color(255, 255, 255, 255));
			debugText.setPosition(700, 200);

			view.setSize(2560, 1440);
			view.setCenter(Player->getPosition().x, Player->getPosition().y);

			bool loading = true;
			sf::Clock deltaClock;
			generalClock.restart();
			float mDelta;
			while(gameState == LEVEL) {
				mDelta = deltaClock.restart().asSeconds();

				if((Player->IsDead() && generalClock.getElapsedTime().asSeconds() >= 3)) {
					break;
				}

				if(Player->IsFinishing() && (Player->getPosition().x == Finish->getPosition().x+16)) {
					if (bzsflegacy::game::currentState != RUNNING) {
						gameState = EXIT;
					}
					break;
				}


				sf::Event Event;
				while(bzsflegacy::game::window->pollEvent(Event)) {
					switch(Event.type) {
						case sf::Event::Closed:
							gameState = EXIT;
							break;

						case sf::Event::KeyPressed:
							if(Event.key.code == sf::Keyboard::Space && !Player->IsTeleporting() && !Player->IsStartingTeleport() && !Player->IsFinishingTeleport()) {
								Player->teleport();
							} else if(Event.key.code == sf::Keyboard::Escape) {
								gameState = EXIT;
							}
							break;

						case sf::Event::KeyReleased:
							if(Event.key.code == sf::Keyboard::Space && Player->IsTeleporting() && !Player->IsStartingTeleport() && !Player->IsFinishingTeleport()) {
								Player->finishTeleport();
							}
							break;
					}
				}

				if(!loading) {
					Player->tick(mDelta, view);
				} else {
					float elapsedTime = generalClock.getElapsedTime().asSeconds()*1;
					if(elapsedTime < 1.f) {
						view.setSize(2560 - 1920*elapsedTime, 1440 - 1080*elapsedTime);
					} else {
						view.setSize(640, 360);
						loading = false;
					}
				}

				if(debug) {
					string debug_string;
					stringstream debug_stringstream;

					debug_stringstream << "Debug:\nmDelta: " << mDelta
						<< "\ncurSpeedX: " << Player->GetCurSpeedX()
						<< "\ncurSpeedY: " << Player->GetCurSpeedY()
						<< "\nrealSpeedX: " << Player->GetCurSpeedX()*mDelta
						<< "\nrealSpeedY: " << Player->GetCurSpeedY()*mDelta
						<< "\nOverflowX: " << Player->GetOverflow().left << " - " << Player->GetOverflow().top
						<< "\nOverflowY: " << Player->GetOverflow().width << " - " << Player->GetOverflow().height
						<< "\nposX: " << Player->getPosition().x
						<< "\nposY: " << Player->getPosition().y
						<< "\ninAir: " << Player->IsInAir()
						<< "\nisTeleporting: " << Player->IsTeleporting();

					debug_string = debug_stringstream.str();

					debugText.setString(debug_string);
				}

				//RENDER
				bzsflegacy::game::window->setView(view);
				bzsflegacy::game::window->clear(sf::Color(15, 109, 1, 255));

				bzsflegacy::game::window->draw(background);
				bzsflegacy::game::window->draw(background2);
				for(unsigned int i = 0; i < objects.size(); i++) {
					objects[i]->update(sf::seconds(mDelta));
					objects[i]->draw();
				}

				Player->GetTeleportTarget().update(sf::seconds(mDelta));
				bzsflegacy::game::window->draw(Player->GetTeleportTarget());

				bzsflegacy::game::window->setView(bzsflegacy::game::window->getDefaultView());
				bzsflegacy::game::window->draw(debugText);

				bzsflegacy::game::window->display();

				stringstream frameStream;
				frameStream << "ANONHACK.EXE  -  " << deltaClock.getElapsedTime().asMicroseconds() << " Microseconds  --  FPS: " << 1000000.0f / deltaClock.getElapsedTime().asMicroseconds();
				bzsflegacy::game::window->setTitle(frameStream.str());
			}

			clearObjects();

			break;
	}

	if(gameState == LEVEL) {
		bzsflegacy::game::currentState = RUNNING;
	} else if(bzsflegacy::game::currentState != BOSS) {bzsflegacy::game::currentState = MENU;}
}



void game_die() {
	bzsflegacy::game::run = false;
}
