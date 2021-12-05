bool compareScale (sf::Sprite& a, sf::Sprite& b) { return (a.getScale().x < b.getScale().x); }


class enemy : public sf::Sprite {
public:
	float direction; //degrees
	float speed;

	bool played;

	float distance;
	sf::Vector2f prevPos;

	sf::Sprite arrow;
	sf::Text text_distance;

	enemy(sf::Texture& tex) : sf::Sprite(tex), arrow(tex_arrow), text_distance("1000", font_default, 10), distance(9999), played(false) {
		setOrigin(12, 12);
		setPosition(180 - 1500 + rand()%3000, 2000 + rand()%3000);

		direction = atan2(-3000, (180 - 1000 + rand()%2000) - getPosition().x) * (180/PI);
		speed = 600 + rand()%400;

		arrow.setOrigin(5, 5);
		arrow.setColor(sf::Color(230, 0, 10, 200));
	}

	enemy(sf::Texture& tex, sf::Vector2f spawn, sf::Vector2f target, float _speed) : sf::Sprite(tex), arrow(tex_arrow), speed(_speed), text_distance("1000", font_default, 10), distance(9999), played(false)  {
		setOrigin(12, 12);
		setPosition(spawn);

		direction = atan2(target.y - spawn.y, target.x - spawn.x) * (180/PI);

		arrow.setOrigin(10, 10);
		arrow.setColor(sf::Color(230, 0, 10, 200));
	}
};

void level_2() {

	music_rain.play();
	music_rain.setVolume(20);
	music_rain.setLoop(true);

	music_theme.setLoop(true);
	music_theme.setVolume(0);
	music_theme.play();


	particleSystem PS;

	sf::Image img_beforeHit;
	img_beforeHit.create(1280, 720);
	Desaturator * des_buildup = 0;
	sf::Texture tex_buildup; tex_buildup.loadFromImage(img_beforeHit);
	sf::Sprite spr_buildup(tex_buildup);


	sf::Font font; font.loadFromFile("imgs/transition.ttf");
	sf::Text text_FPS("0 FPS", font);


	sf::Image img_enemy;
	img_enemy.create(24, 24, sf::Color(255, 0, 0));

	sf::Texture tex_enemy;
	tex_enemy.loadFromImage(img_enemy);

	std::vector<enemy> enemies; // first is arrow, second is enemy

	sf::Image img_player;
	img_player.create(16, 16);

	sf::Texture tex_player;
	tex_player.loadFromImage(img_player);

	sf::Sprite spr_player(tex_player);
	spr_player.setOrigin(8, 8);
	spr_player.setPosition(160, -20);

	float hSpeed = 0;
	float vSpeed = 0;
	float rotSpeed = 0;

	sf::Uint8 health = 0;


	sf::Image img_fader;
	img_fader.create(640, 360, sf::Color(0, 0, 0, 255));

	sf::Texture tex_fader;
	tex_fader.loadFromImage(img_fader);

	sf::Sprite spr_fader(tex_fader);
	spr_fader.setColor(sf::Color(255, 255, 255, 255));

	sf::Sprite spr_rain(tex_rain0);
	spr_rain.setColor(sf::Color(90, 90, 90, 90));

	sf::Sprite spr_healthbar(tex_healthbar);
	spr_healthbar.setPosition(20, 140);
	spr_healthbar.setColor(sf::Color(255, 255, 255, 0));

	std::vector<sf::Sprite> healthfills;
	for(unsigned int i = 0; i<4; i++) {
		sf::Sprite hf(tex_healthfill);
		hf.setColor(sf::Color(255, 255, 255, 0));

		hf.setPosition(21 + i*20, 142);
		healthfills.push_back(hf);
	}

	bool fadedin = false;
	bool begun = false;

	bool hit = false;
	bool buildup = false;
	bool afterhit = false;

	bool dead = false;


	sf::Vector2f hitDisplace(0,0);
	bool displaced = false;

	sf::View view(sf::Vector2f(160, 90), sf::Vector2f(640, 360));
	sf::View HUDView(sf::Vector2f(160, 90), sf::Vector2f(320, 180));
	sf::View rainView(sf::Vector2f(320, 180), sf::Vector2f(640, 360));


	sf::Vector2f viewSize = HUDView.getSize();


	std::vector<sf::Sprite> clouds;
	sf::Sprite cloud(tex_cloud0);
	cloud.setOrigin(30, 30);
	cloud.setRotation(-15);
	cloud.setPosition(100, 60);
	clouds.push_back(cloud);

	cloud = sf::Sprite(tex_cloud1);
	cloud.setOrigin(30, 30);
	cloud.setRotation(5);
	cloud.setPosition(240, 80);
	clouds.push_back(cloud);

	cloud = sf::Sprite(tex_cloud3);
	cloud.setOrigin(30, 30);
	cloud.setRotation(180);
	cloud.setPosition(160, 170);
	clouds.push_back(cloud);


	sf::Image img_flash;
	img_flash.create(1280, 720, sf::Color(255, 255, 255, 255));
	sf::Texture tex_flash; tex_flash.loadFromImage(img_flash);

	sf::Sprite spr_flash(tex_flash);
	spr_flash.setColor(sf::Color(255, 255, 255, 0));


	sf::Uint32 enemySpawnFrequency = 2000;


	sf::Clock clock;
	sf::Clock rainClock;
	sf::Clock cloudSpawn;
	sf::Clock enemyClock;
	sf::Clock frameClock;
	sf::Clock hitClock;
	float mDelta;
	while(bzsflegacy::game::currentState == LEVEL2) {

		mDelta = frameClock.restart().asSeconds();

		if(hit) {
			if(buildup) mDelta*=0; else mDelta*=0.1;
		} else if(dead) mDelta*=0.5;

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

		if(rainClock.getElapsedTime().asMilliseconds() % 200 < 100) {
			spr_rain.setTexture(tex_rain0);
		} else {
			spr_rain.setTexture(tex_rain1);
		}

		////////////////// INTRO STUFF /////////////////////////
		sf::Int32 elapsed = clock.getElapsedTime().asMilliseconds();
		if(!fadedin) {
			if(elapsed < 1000) {
				spr_player.setPosition(160, -20 + 110*(float(elapsed)/1000));
				spr_fader.setColor(sf::Color(255, 255, 255, 255 - 255*(float(elapsed)/1000)));
			} else {
				spr_player.setPosition(160, 90);
				fadedin=true;
				spr_fader.setColor(sf::Color(255, 255, 255, 0));
				clock.restart();
			}
		} else if(!begun) {
			if(elapsed < 3000) {
				view.setSize(640 - float(640-320)*sin((PI/2)*(float(elapsed)/3000)), 360 - float(360-180)*sin((PI/2)*(float(elapsed)/3000)));
				music_theme.setVolume(100.f * (float(elapsed)/3000));
				if(elapsed < 500) {
					spr_healthbar.setColor(sf::Color(255, 255, 255, 255*(float(elapsed)/500)));
					vSpeed = 300.f*(float(elapsed)/500);
				} else {
					vSpeed = 300.f;
					if(elapsed >= 1000 && health==0) {
						healthfills[0].setColor(sf::Color(255, 255, 255, 255));
						snd_healthtick.play();
						health = 1;
					} else if(elapsed >= 1500 && health==1) {
						healthfills[1].setColor(sf::Color(255, 255, 255, 255));
						snd_healthtick.play();
						health = 2;
					} else if(elapsed >= 2000 && health==2) {
						healthfills[2].setColor(sf::Color(255, 255, 255, 255));
						snd_healthtick.play();
						health = 3;
					} else if(elapsed >= 2500 && health==3) {
						healthfills[3].setColor(sf::Color(255, 255, 255, 255));
						snd_healthtick.play();
						health = 4;
					}
					spr_healthbar.setColor(sf::Color(255, 255, 255, 255));
				}

			} else {
				view.setSize(320, 180);
				music_theme.setVolume(100.f);
				begun = true;
				clock.restart();
			}

			spr_player.setPosition(160, 90 - (vSpeed/700)*60);

			//Test clouds during spawn
			if(cloudSpawn.getElapsedTime().asMilliseconds() >= 120) {
				cloudSpawn.restart();

				sf::Uint8 chance = rand()%4;
				if(chance == 0) {
					cloud = sf::Sprite(tex_cloud0);
				} else if(chance == 1) {
					cloud = sf::Sprite(tex_cloud1);
				} else if(chance == 2) {
					cloud = sf::Sprite(tex_cloud2);
				} else if(chance == 3) {
					cloud = sf::Sprite(tex_cloud3);
				}

				float scale = 0.2f + float(rand()%15)/10;
				cloud.setScale(scale, scale);
				cloud.setOrigin(cloud.getLocalBounds().width/2, cloud.getLocalBounds().height/2);
				cloud.setPosition(-70 + rand()%500 + hSpeed, 300);
				cloud.setRotation(rand()%360);

				clouds.push_back(cloud);

				std::sort(clouds.begin(), clouds.end(), compareScale);
			}
			for(sf::Uint8 i = 0; i<clouds.size(); i++) {
																																			//// Try powering scale by two
				clouds[i].setPosition(clouds[i].getPosition().x - hSpeed*clouds[i].getScale().x*mDelta, clouds[i].getPosition().y - vSpeed*clouds[i].getScale().x*mDelta*0.5);

				if(clouds[i].getPosition().y < -100) {
					clouds.erase(clouds.begin() + i);
					i--;
				}
			}
		} else {
		///////////////////////////////////////////////////////


			//// ADJUST Spawn frequency according to time spent /////////
			float eElapsed = clock.getElapsedTime().asSeconds();
			if(eElapsed < 30) {
				enemySpawnFrequency = 100 + 1900 - 1900*(eElapsed/30);
			} else {
				enemySpawnFrequency = 100;
			}


			vSpeed += 200*mDelta* (dead ? 2 : 1);
			if(vSpeed > 700) {vSpeed = 700;}
			if(vSpeed < -300) {vSpeed +=600*mDelta;}

			if(!dead) {
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
					if(hSpeed > -150) hSpeed -= 200*mDelta;
				} else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
					if(hSpeed < 150) hSpeed += 200*mDelta;
				} else {
					if(hSpeed > 0) {
						if(hSpeed < 50*mDelta) {
							hSpeed = 0;
						} else {
							hSpeed -= 50*mDelta;
						}
					} else if(hSpeed < 0) {
						if(hSpeed > -50*mDelta) {
							hSpeed = 0;
						} else {
							hSpeed += 50*mDelta;
						}
					}
				}
				if(hSpeed > 150) hSpeed -= 100*mDelta;
				if(hSpeed < -150) hSpeed += 100*mDelta;
			}


			if(rotSpeed < 180 && rotSpeed > -180) rotSpeed += hSpeed/50;
			if(rotSpeed > 50) {rotSpeed -= 100*mDelta;}
			if(rotSpeed < -50) {rotSpeed += 100*mDelta;}
			if(rotSpeed > 720) rotSpeed = 720;
			if(rotSpeed < -720) rotSpeed = -720;

			spr_player.setRotation(spr_player.getRotation() + rotSpeed*mDelta);
			spr_player.setPosition(160, 90 - (vSpeed/700)*60 * (vSpeed < 0 ? 0.2 : 1));

			if(hit) {
				sf::Uint32 elapsed = hitClock.getElapsedTime().asMilliseconds();
				if(buildup) {
					if(elapsed >= 500 && elapsed < 700) {
						spr_flash.setColor(sf::Color(255, 255, 255, 255 * (float(elapsed-500)/200)));
					} else if(elapsed > 700) {
						spr_flash.setColor(sf::Color(255, 255, 255, 255));
						buildup = false;
						snd_Lightning.play();
						hitClock.restart();

						view.zoom(3);
					}
				} else {
					if(elapsed < 200) {
						spr_flash.setColor(sf::Color(255, 255, 255, 255 - 255 * sin((PI/2)*(float(elapsed)/200))));

					} else {
						spr_flash.setColor(sf::Color(255, 255, 255, 0));
					}

					if(elapsed < 700) {
						view.setSize(view.getSize().x + (viewSize.x - view.getSize().x)*(float(elapsed)/700), view.getSize().y + (viewSize.y - view.getSize().y)*(float(elapsed)/700));
						view.setCenter(160 + rand()%(1+int((1.f-(float(elapsed)/700)) * 20)) - (1.f-(float(elapsed)/700))/2 , 90 + rand()%(1+int((1.f-(float(elapsed)/700)) * 20)) - (1.f-(float(elapsed)/700)/2));
					} else {
						view.setSize(viewSize);
						view.setCenter(160, 90);

						if(hSpeed < 0) rotSpeed-=720;
						if(hSpeed >= 0) rotSpeed+=720;

						hit = false;
						afterhit = true;
						hitClock.restart();

						if(health > 1) {
							health--;
							healthfills[health].setColor(sf::Color(255, 255, 255, 0));
						} else { // You die
							/// TRANSITION HERE
							healthfills[0].setColor(sf::Color(255, 255, 255, 0));
							dead = true;

							enemies.clear();
							clock.restart();
						}
					}
				}
			}

			if(dead) {
				sf::Uint32 elapsed = clock.getElapsedTime().asMilliseconds();
				if(elapsed < 10000) {
					spr_fader.setColor(sf::Color(255, 255, 255, 255*(float(elapsed)/10000)));
					music_theme.setVolume(100.f - 100.f*(float(elapsed)/10000));
					music_rain.setVolume(20.f - 20.f*(float(elapsed)/10000));
				} else {
					spr_fader.setColor(sf::Color(255, 255, 255, 255));
					music_theme.stop();
					music_rain.stop();

					bzsflegacy::game::currentState = TRANSITION;
				}
			}

			if(afterhit) {
				sf::Uint32 elapsed = hitClock.getElapsedTime().asMilliseconds();
				if(elapsed < 500) {
					view.setCenter(160 + rand()%(1+int((1.f-(float(elapsed)/500)) * 10)) - (1.f-(float(elapsed)/500))/2 , 90 + rand()%(1+int((1.f-(float(elapsed)/500)) * 10)) - (1.f-(float(elapsed)/500)/2));
				} else {
					view.setCenter(160, 90);
					afterhit = false;
				}
			}

			if(!buildup) {
				// Cloudspawn ////////////////
				if(cloudSpawn.getElapsedTime().asMilliseconds() >= 80) {
					cloudSpawn.restart();

					sf::Uint8 chance = rand()%4;
					if(chance == 0) {
						cloud = sf::Sprite(tex_cloud0);
					} else if(chance == 1) {
						cloud = sf::Sprite(tex_cloud1);
					} else if(chance == 2) {
						cloud = sf::Sprite(tex_cloud2);
					} else if(chance == 3) {
						cloud = sf::Sprite(tex_cloud3);
					}


					cloud.setPosition(-70 + rand()%500 + hSpeed, 300);
					float scale = 0.2f + float(rand()%15)/10;
					cloud.setScale(scale, scale);
					cloud.setRotation(rand()%360);

					clouds.push_back(cloud);

					std::sort(clouds.begin(), clouds.end(), compareScale);
				}
				////////////////////////////////////

				/////////////// MOVE CLOUDS  /////
				for(sf::Uint8 i = 0; i<clouds.size(); i++) {
																																				//// Try powering scale by two
					clouds[i].setPosition(clouds[i].getPosition().x - hSpeed*clouds[i].getScale().x*mDelta, clouds[i].getPosition().y - vSpeed*clouds[i].getScale().x*mDelta*0.5);

					if(clouds[i].getPosition().y < -100) {
						clouds.erase(clouds.begin() + i);
						i--;
					}
				}
				//////////////////////////////////////////


				if(!dead) {
					///////// SPAWN ENEMIES /////////
					if(!hit) {
						if(enemyClock.getElapsedTime().asMilliseconds() >= enemySpawnFrequency) {
							enemyClock.restart();
							enemies.push_back(enemy(tex_enemy));
						}
					}


					//// UPDATE ENEMIES ////
					for(sf::Uint8 i = 0; i<enemies.size(); i++) {
						enemies[i].prevPos = enemies[i].getPosition();

						enemies[i].setPosition(enemies[i].getPosition().x + cos(DEGTORAD(enemies[i].direction))*enemies[i].speed*mDelta, enemies[i].getPosition().y + sin(DEGTORAD(enemies[i].direction))*enemies[i].speed*mDelta);
						enemies[i].setPosition(enemies[i].getPosition().x - hSpeed*mDelta, enemies[i].getPosition().y);

						if(enemies[i].distance < 200 && enemies[i].getPosition().y > spr_player.getPosition().y - 20) {
							if(!enemies[i].played) {
								if(rand()%3 == 0) {
									snd_swoosh0.play();
								} else if(rand()%2 == 0) {
									snd_swoosh1.play();
								} else {
									snd_swoosh2.play();
								}

								hitClock.restart();
								afterhit = true;
								enemies[i].played = true;
							}
						}

						if(!hit) {
							if(LineIntersectsRect(sf::Vector2f(enemies[i].prevPos.x - 10, enemies[i].prevPos.y), sf::Vector2f(enemies[i].getPosition().x - 10, enemies[i].getPosition().y), spr_player.getGlobalBounds()) ||
								LineIntersectsRect(sf::Vector2f(enemies[i].prevPos.x + 10, enemies[i].prevPos.y), sf::Vector2f(enemies[i].getPosition().x + 10, enemies[i].getPosition().y), spr_player.getGlobalBounds())) {
									hit = true;
									buildup = true;
									afterhit = false;

									img_beforeHit = bzsflegacy::game::window->capture();
									tex_buildup.loadFromImage(img_beforeHit);

									delete des_buildup;
									des_buildup = new Desaturator(img_beforeHit, 500);

									PS.fuel(DEGTORAD(enemies[i].direction), PI/2, 30, enemies[i].getPosition(), 0.75f, sf::Color(230, 10, 0, 255));
									PS.fuel(DEGTORAD(enemies[i].direction), PI/2, 30, spr_player.getPosition(), 0.75f, sf::Color(0, 0, 0, 255));


									hSpeed += cos(DEGTORAD(enemies[i].direction))*enemies[i].speed * 1.5; // more oomph
									vSpeed += sin(DEGTORAD(enemies[i].direction))*enemies[i].speed;

									hitClock.restart();

									enemies[i].setPosition(enemies[i].prevPos);
									snd_buildup.play();

							}
						}

						if(enemies[i].getPosition().y < -1000) {
							enemies.erase(enemies.begin() + i);
							i--;
						}
					}
				}
			}
			/////////////////////////////////////

		}




		bzsflegacy::game::window->clear(sf::Color(8,8,8));

		bzsflegacy::game::window->setView(rainView);
		bzsflegacy::game::window->draw(spr_rain);



		bzsflegacy::game::window->setView(view);


		// Clouds behind player
		for(std::vector<sf::Sprite>::iterator iter = clouds.begin(); iter!=clouds.end(); iter++) {
			if(((sf::Sprite)(*iter)).getScale().x < 1.f) bzsflegacy::game::window->draw(*iter);
		}

		bzsflegacy::game::window->draw(spr_player);

		//Clouds in front of player
		for(std::vector<sf::Sprite>::iterator iter = clouds.begin(); iter!=clouds.end(); iter++) {
			if(((sf::Sprite)(*iter)).getScale().x >= 1.f) bzsflegacy::game::window->draw(*iter);
		}


		std::stringstream ss;
		for(std::vector<enemy>::iterator iter = enemies.begin(); iter!=enemies.end(); iter++) {
			enemy * e = &(*iter);
			float angle = atan2(e->getPosition().y - spr_player.getPosition().y, e->getPosition().x - spr_player.getPosition().x);
			e->arrow.setRotation(RADTODEG(angle));

			double cosV = cos(angle); double sinV = sin(angle);

			if(angle < 0) {angle+=2*PI;}
			e->arrow.setPosition(320 + cosV*200, 2.f*40-(vSpeed/50.f) + sinV*200);

			int distance = sqrt((spr_player.getPosition().x - e->getPosition().x) * (spr_player.getPosition().x - e->getPosition().x) + (spr_player.getPosition().y - e->getPosition().y) * (spr_player.getPosition().y - e->getPosition().y));

			e->distance = distance;

			if(distance < 1500) {
				if(distance < 1000 && e->getPosition().y > spr_player.getPosition().y) {
					if(LineIntersectsRect(e->getPosition(), sf::Vector2f(e->getPosition().x-10 + distance*cos(DEGTORAD(e->direction)), e->getPosition().y + distance*sin(DEGTORAD(e->direction))), spr_player.getGlobalBounds()) ||
						LineIntersectsRect(e->getPosition(), sf::Vector2f(e->getPosition().x+10 + distance*cos(DEGTORAD(e->direction)), e->getPosition().y + distance*sin(DEGTORAD(e->direction))), spr_player.getGlobalBounds())) {

						e->arrow.setColor(sf::Color(240, 0, 0, 200));
						e->text_distance.setColor(sf::Color(255, 200, 200, 255));
						e->text_distance.setScale(1.2f, 1.2f);
						e->arrow.setScale(1.f + 1.f - 1.f*(float(distance-500)/1000), 1.f + 1.f - 1.f*(float(distance-500)/1000));
					}
				} else {
					e->arrow.setColor(sf::Color(240, 0, 0, 200));
					e->text_distance.setColor(sf::Color(255, 255, 255, 255));
					e->arrow.setScale(1.f + 1.f - 1.f*(float(distance-500)/1000), 1.f + 1.f - 1.f*(float(distance-500)/1000));
				}
			} else if(distance >= 2500) {
				e->arrow.setColor(sf::Color(0, 230, 0, 10));
				e->text_distance.setColor(sf::Color(255, 255, 255, 0));
			} else {
				e->arrow.setColor(sf::Color(255, 231, 0, 120 - 100*(float(distance-1500)/1000)));
				e->text_distance.setColor(sf::Color(255, 255, 255, 0));
			}

			ss << distance;
			e->text_distance.setString(ss.str());
			e->text_distance.setOrigin(e->text_distance.getLocalBounds().width/2, e->text_distance.getLocalBounds().height/2);
			e->text_distance.setPosition(int(e->arrow.getPosition().x - cosV*15*e->arrow.getScale().x), int(e->arrow.getPosition().y - sinV*15*e->arrow.getScale().x));
			e->text_distance.setRotation(RADTODEG(angle-PI/2));
			ss.str(std::string());

			bzsflegacy::game::window->setView(rainView);
			bzsflegacy::game::window->draw(e->text_distance);
			bzsflegacy::game::window->draw(e->arrow);

			bzsflegacy::game::window->setView(view);
			bzsflegacy::game::window->draw(*e);
		}

		PS.draw(mDelta);


		bzsflegacy::game::window->setView(HUDView);
		bzsflegacy::game::window->draw(spr_healthbar);
		for(std::vector<sf::Sprite>::iterator iter = healthfills.begin(); iter!=healthfills.end(); iter++) {
			bzsflegacy::game::window->draw(*iter);
		}


		bzsflegacy::game::window->setView(rainView);
		bzsflegacy::game::window->draw(spr_fader);

		bzsflegacy::game::window->setView(bzsflegacy::game::window->getDefaultView());

		if(hit && buildup) {
			// Desaturated buildup image
			tex_buildup.update(des_buildup->getPixels());
			bzsflegacy::game::window->draw(spr_buildup);
		}
		bzsflegacy::game::window->draw(spr_flash);


		/*std::stringstream fps;
		fps << "FPS: " << 1000000.0f / frameClock.getElapsedTime().asMicroseconds() << "\nhSpeed: " << hSpeed << "\nvSpeed: " << vSpeed;
		text_FPS.setString(fps.str());
		bzsflegacy::game::window->draw(text_FPS);
		*/

		bzsflegacy::game::window->display();
	}

	delete des_buildup;
}
