#include "Eagle.hpp"
#include "Utility.hpp"
#include "Application.hpp"
#include "CliffNode.hpp"
#include "EmitterNode.hpp"
#include "BarNode.hpp"
#include "ScreenShakeNode.hpp"

Eagle::Eagle(Context context, CliffNode* cliff, const float SEALINE, SceneNode& upperLayer)
	: offscreen(false)
	, score(0)
	, air(100.f)
	, energy(200.f)
	, prevFrame(0)
	, nestRect(57, 147, 54, 23)
	, showNestArrow(false)
	, accumulatedScore(0)
	, SEALINE(SEALINE)
	, returnArrow()
	, nestArrow()
	, controlScheme(cScheme)
	, cliff(cliff)
	, returnText("RETURN TO MISSION!", context.fonts->get(Font::Emulator), 60)
	, returnProgress(0.f)
	, scoreText("Score: 0", context.fonts->get(Font::Emulator), 50.f)
	, accScoreText("0", context.fonts->get(Font::Emulator), 20) {

	drawable.setAnimation(*ParseAnimation(context.animations->at(Animation::Eagle)));
	drawable.getAnimation()->setFrame(1);
	CenterOrigin(drawable);
	setRotation(-90.f);

	uwCircle.setRadius(10);
	uwCircle.setFillColor(uwColor);
	CenterOrigin(uwCircle);

	nestArrow.setTexture(context.textures->get(Texture::ReturnArrow));
	//nestArrow.setColor(sf::Color::Green);
	CenterOrigin(nestArrow);
	nestArrow.setScale(-1, 1);
	nestArrow.setPosition(nestRect.left + nestRect.width + 20, nestRect.top + nestRect.height / 2);

	CenterOrigin(accScoreText);
	accScoreText.setPosition(-20, 0);
	accScoreText.rotate(90);

	returnArrow.sprite.setTexture(context.textures->get(Texture::ReturnArrow));
	CenterOrigin(returnArrow.sprite);
	returnArrow.text.setFont(context.fonts->get(Font::Arial));
	returnArrow.text.setCharacterSize(12);
	CenterOrigin(returnArrow.text);

	CenterOrigin(returnText);
	returnText.setPosition(Application::settings.windowSize.x / 2, Application::settings.windowSize.y / 2);

	CenterOrigin(scoreText);
	scoreText.setPosition(Application::settings.windowSize.x / 2, 50);
	scoreText.setColor(sf::Color(229, 255, 17));

	airBar = new BarNode(100, sf::Color(29, 195, 219), sf::Vector2f(100.f, 20.f));
	airBar->d = false;
	SceneNode::Ptr ab(airBar);
	ab->rotate(90);
	ab->setPosition(50, 0);
	attachChild(std::move(ab));

	energyBar = new BarNode(200, sf::Color(255, 171, 6), sf::Vector2f(30.f, 100.f), false);
	SceneNode::Ptr eb(energyBar);
	eb->setPosition(40, 70);
	upperLayer.attachChild(std::move(eb));
}


void Eagle::updateCurrent(sf::Time dt, CommandQueue& commandQueue) {
	drawable.update(dt);
	
	if(!isDestroyed()) {
		if(accumulatedScore > 0) {
			if(getBoundingRect().intersects(nestRect)) {
				playSound(SoundEffect::Powerup, commandQueue);
				score += accumulatedScore;
				accumulatedScore = 0;
			}

			showNestArrow = true;

			std::stringstream ss; ss << accumulatedScore;
			accScoreText.setString(ss.str());
			CenterOrigin(accScoreText);
		} else showNestArrow = false;
		
		auto coll = cliff->testCollision(getBoundingRect());
		if(coll.first) {
			if(std::abs(velocity.x) >= deadlyForce) { // Die when hitting too fast
				destroy();
				playSound(SoundEffect::Die, commandQueue);
				shake(5.f, false, commandQueue);
				return;
			}

			move(coll.second);
			velocity.x = 0;

			if(coll.second.x == 0 && coll.second.y < 0)
				velocity.y = 0;
		}

		checkInput(dt);
		applyGravity(dt);
		
		// Push downwards when too far up
		if(getWorldPosition().y <= 50) {
			float p = getWorldPosition().y;
			velocity.y += std::min(((50.f - p) / 50) * 2000.f * dt.asSeconds(), 2000.f * dt.asSeconds());
		}


		// Underwater physics
		if(underwater) {
			if(Length(velocity) >= 200) {
				sf::Vector2f tempVel(velocity * 0.9f * dt.asSeconds());
				if(Length(tempVel) > 200)
					velocity = tempVel;
				else velocity = UnitVector(velocity) * 200.f;
			} else {
				velocity -= velocity * 0.9f * dt.asSeconds();
			}
		}

		move(velocity * dt.asSeconds());

		checkOffscreen();
		if(offscreen && offScreenClock.getElapsedTime() >= sf::seconds(3)) {
			destroy();
			playSound(SoundEffect::Die, commandQueue);
			return;
		}

		returnProgress += 5.f * dt.asSeconds();

		// Handle energy
		energy = std::max(energy - 2.f * dt.asSeconds(), 0.f);
		energyBar->setValue(energy);

		if(energy <= 0) {
			destroy();
			playSound(SoundEffect::Die, commandQueue);
			return;
		}


		// Handle underwater events
		if(getWorldPosition().y >= SEALINE) {
			if(!underwater) {
				splashWater(getWorldPosition().x, commandQueue);
				playSound(SoundEffect::WaterSplashBig, commandQueue);
				shake(2.f, true, commandQueue);

				airBar->d = true;
				underwater = true;
			}

			air = std::max(air - 50.f * dt.asSeconds(), 0.f);

			if(air <= 0) {
				destroy();
				playSound(SoundEffect::Die, commandQueue);
				shake(1.f, true, commandQueue);
				return;
			} else if(air <= 40.f) {
				airBar->setColor(sf::Color(237, 61, 22));
				shake((40 - air) / 8.f + 1.f, false, commandQueue);
			} else airBar->setColor(sf::Color(29, 195, 219));

		} else {
			if(air >= 100)
				airBar->d = false;

			airBar->setColor(sf::Color(29, 195, 219, 128));
			underwater = false;

			air = std::min(air + 40.f * dt.asSeconds(), 100.f);
		}

		airBar->setValue(air);

		// Flap sounds
		if(drawable.getAnimation()->getIndex() != prevFrame) {
			if(drawable.getAnimation()->getIndex() == 0)
				playSound(SoundEffect::Flap, commandQueue);
		}

		prevFrame = drawable.getAnimation()->getIndex();

		std::stringstream ss; ss << "Score: " << score;
		scoreText.setString(ss.str());
	} else
		if(dieClock.getElapsedTime() >= sprayTime)
			detachChild(*deadSpray);
}

void Eagle::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) {
	target.draw(drawable, states);
	target.draw(accScoreText, states);
	target.draw(scoreText);

	if(showNestArrow)
		target.draw(nestArrow);

	if(offscreen) {
		target.draw(returnArrow.sprite);
		target.draw(returnArrow.text);
		
		sf::Color color(255, 127 + std::sin(returnProgress * bzsf::PI) * 127, 62);
		returnText.setColor(color);
		target.draw(returnText);
	}

	if(getWorldPosition().y < 10) {
		sf::Color color(255, 127 + std::sin(returnProgress * bzsf::PI) * 127, 62);
		returnText.setColor(color);
		target.draw(returnText);
	}
}

void Eagle::checkInput(sf::Time dt) {
	if(!controlScheme) {
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			rotate(-rotSpeed * dt.asSeconds() * (underwater ? 0.5f : 1.f));

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			rotate(rotSpeed * dt.asSeconds() * (underwater ? 0.5f : 1.f));

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			propel(dt);
		else
			drawable.getAnimation()->setRepeating(false);
	} else {
		sf::Vector2f nudge;
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			nudge.x -= propelForce * dt.asSeconds();

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			nudge.x += propelForce * dt.asSeconds();

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			nudge.y -= propelForce * dt.asSeconds();

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			nudge.y += propelForce * dt.asSeconds();

		velocity += nudge;

		if(nudge != sf::Vector2f()) {
			energy = std::max(energy - 5.f * dt.asSeconds(), 0.f);
			drawable.getAnimation()->setRepeating(true);

			float prevRot = getRotation();
			float angle = (velocity.x != 0 ? bzsf::RADTODEG(std::atan(velocity.y / velocity.x)) : prevRot);
			if(angle != prevRot)
				angle += (velocity.x < 0 ? 180.f : 0.f);

			setRotation(angle);
		} else
			drawable.getAnimation()->setRepeating(false);
	}
}

void Eagle::propel(sf::Time dt) {
	//Drain energy
	energy = std::max(energy - 5.f * dt.asSeconds(), 0.f);
	
	drawable.getAnimation()->setRepeating(true);
	velocity += sf::Vector2f(cos(bzsf::DEGTORAD(getRotation())) * propelForce, sin(bzsf::DEGTORAD(getRotation())) * propelForce) * dt.asSeconds();
}


void Eagle::shake(float value, bool increment, CommandQueue& commandQueue) {
	Command cm(Category::ScreenShake, derivedAction<ScreenShakeNode>([=](ScreenShakeNode& e, sf::Time) {
		if(increment)
			e.shake += value;
		else
			e.shake = value;
	}));

	commandQueue.push(cm);
}


void Eagle::checkOffscreen() {
	// Also check if player hits bottom of water
	auto rect = getBoundingRect();
	if(underwater && rect.top + rect.height >= Application::settings.windowSize.y) {
		velocity.y *= -0.5;
		setPosition(getPosition().x, Application::settings.windowSize.y - rect.height / 2);
	}
	
	const float textDistance = 15;
	if(getPosition().x < -5) {
		if(!offscreen) {
			offscreen = true;
			returnArrow.sprite.setScale(-1, 1);

			offScreenClock.restart();
		}

		sf::Vector2f wPos = getWorldPosition();
		returnArrow.sprite.setPosition(14, wPos.y);

		std::stringstream ss; ss << floor(-wPos.x);
		returnArrow.text.setString(ss.str());
		returnArrow.text.setOrigin(sf::Vector2f(0, returnArrow.text.getLocalBounds().height) / 2.f);
		returnArrow.text.setPosition(returnArrow.sprite.getPosition() + sf::Vector2f(textDistance, -5));
	} else if(getPosition().x > Application::settings.windowSize.x + 5) {
		if(!offscreen) {
			offscreen = true;
			returnArrow.sprite.setScale(1, 1);

			offScreenClock.restart();
		}

		int windowX = Application::settings.windowSize.x;
		sf::Vector2f wPos = getWorldPosition();
		returnArrow.sprite.setPosition(windowX - 14, wPos.y);

		std::stringstream ss; ss << floor(wPos.x - windowX);
		returnArrow.text.setString(ss.str());
		returnArrow.text.setOrigin(sf::Vector2f(returnArrow.text.getLocalBounds().width, returnArrow.text.getLocalBounds().height / 2.f));
		returnArrow.text.setPosition(returnArrow.sprite.getPosition() + sf::Vector2f(-textDistance, -5));
	} else offscreen = false;
}


sf::FloatRect Eagle::getBoundingRect() {
	return getWorldTransform().transformRect(sf::FloatRect(-8, -8, 16, 16));
}

sf::Uint32 Eagle::getCategory() {
	return Category::Player;
}

void Eagle::destroy() {
	Entity::destroy();

	dieClock.restart();
	restartClock.restart();

	deadSpray = new EmitterNode(Particle::Blood, 800);
	attachChild(SceneNode::Ptr(deadSpray));

	attachChild(SceneNode::Ptr(new EmitterNode(Particle::Blood, 50)));
}

bool Eagle::isMarkedForRemoval() {
	return isDestroyed() && restartClock.getElapsedTime() >= dieTime;
}

bool Eagle::isAffectedByWind() {
	return !underwater;
}



void Eagle::drawSea(sf::RenderTarget& target, sf::RenderStates states) {
	states.transform *= getTransform();
	target.draw(accScoreText, states);

	if(!isDestroyed())
		target.draw(uwCircle, states);
	else
		target.draw(drawable, states);

	for(Ptr& s : children)
		s->draw(target, states);
}