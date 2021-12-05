#ifndef LD31_EAGLE
#define LD31_EAGLE

// Player object
#include "Entity.hpp"
#include "resid.hpp"

struct Context;
class CliffNode;
class BarNode;

class Eagle : public Entity {
private:
	const float propelForce = 1000.f;
	const float rotSpeed = 360.f;
	const float deadlyForce = 600.f;

	const float SEALINE;

	bool offscreen;
	bool underwater;

	bzsf::Drawable drawable;
	const sf::Color uwColor = sf::Color(18, 23, 71);
	sf::CircleShape uwCircle;

	CliffNode* cliff;

	BarNode* airBar;
	BarNode* energyBar;

	const sf::Time sprayTime = sf::seconds(0.1f);
	const sf::Time dieTime = sf::seconds(2.f);
	sf::Clock dieClock;
	sf::Clock restartClock;
	SceneNode* deadSpray;


	sf::Clock offScreenClock;

	sf::Uint32 prevFrame;

	sf::Text returnText;
	float returnProgress;

	bool showNestArrow;
	sf::Sprite nestArrow;
	sf::FloatRect nestRect;
	sf::Text scoreText;
	sf::Text accScoreText;


private:
	struct ReturnArrow {
		sf::Sprite sprite;
		sf::Text text;
	};

	ReturnArrow returnArrow;

private:
	void updateCurrent(sf::Time dt, CommandQueue& commandQueue);
	void drawCurrent(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default);

	void checkInput(sf::Time dt);
	void propel(sf::Time dt);

	void shake(float value, bool increment, CommandQueue& commandQueue);

	void checkOffscreen();

public:
	Eagle(Context context, CliffNode* cliff, const float SEALINE, SceneNode& upperLayer);
	
	float accumulatedScore;
	float score;
	float air;
	float energy;

	bool controlScheme;

	sf::FloatRect getBoundingRect();
	sf::Uint32 getCategory();

	void destroy();
	bool isMarkedForRemoval();
	bool isAffectedByWind();

	void drawSea(sf::RenderTarget& target, sf::RenderStates states);
};


#endif