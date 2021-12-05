

class particle : public bzsf::Drawable {
	float velocity;
	float friction;
	float angle;
	
	sf::Color color;
	bool dead;

	float life;
	sf::Clock fadeClock;
public:
	sf::Clock getClock() {return fadeClock;}
	bool isDead() {return dead;}

	particle(float _a, float _v, sf::Vector2f pos, float scale, sf::Color _color) : velocity(_v), angle(_a), friction(800.f), dead(false), color(_color) {
		//cout << "angle: " << angle << endl;
		
		life = 1.6f + (rand()%8)/10.f;

		setTile(particles->get(rand()%particles->getSize().x, rand()%particles->getSize().y));
		setPosition(pos);
		setScale(0.5f, 0.5f);
		setColor(color);
	}

	void update(float mDelta) {
		float elapsed = fadeClock.getElapsedTime().asSeconds() / life;
		if(elapsed >= 1) {dead = true; return;}
		
		sf::Color c = getColor();
		setColor(sf::Color(c.r, c.g ,c.b, sf::Uint8(255 - 255*elapsed)));
		setPosition(getPosition().x + cos(angle)*velocity * mDelta, getPosition().y + sin(angle)*velocity * mDelta);

		velocity -= friction * mDelta;
		if(velocity < 0) {velocity = 0; dead = true;}
	}
};