#include "particle.h"

class particleSystem {
	
	std::vector<particle*> particles;
public:
	particleSystem(){

	}

	/*void initPSystem(int * intense, bool right, int ballX, int ballY){
		_ison = true;
		_particleSpeed = intense;
		_particleTimer.restart();
		
		for(int i = 0; i < 20; i++){
			particles.push_back(new particle(*_particleSpeed, ballX, ballY, right));
		}
	}*/

	void fuel(float angle, float angleSpread, float intensity, sf::Vector2f origin, float amountFactor = 1.f, sf::Color color = sf::Color(255, 255, 255, 255)) {
		int amount = floor(intensity / 1.5f) * amountFactor;
		
		angleSpread = bzsf::PI/4 + angleSpread;
		if(angleSpread > 2*bzsf::PI) angleSpread = 2*bzsf::PI;

		//cout << "angle: " << angle << endl << "as: " << as << endl;

		for(int i = 0; i < amount; i++) {
			float as = float(rand()%(int)(angleSpread*1000))/1000;
			particles.push_back(new particle(angle - angleSpread/2 + as,  600.f*intensity/100 + (rand()%((int)(200*intensity/10)))*intensity/50, origin, 1.f + 1.f*intensity/40, color ));
		}
	}



	void draw(float mDelta) {
		for(unsigned int i = 0; i < particles.size(); i++){
			particles[i]->update(mDelta);
			if(particles[i]->isDead()) {
				particles.erase(particles.begin() + i);
				i--;
			} else {
				bzsflegacy::game::window->draw(*particles[i]);
			}
		}
	}
};