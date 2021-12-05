#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <math.h>
#include <fstream>
#include <time.h>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>



#include <bzsf/SFML_Snips.hpp>
//#include "TBC-1.0\collision_detection.hpp"

#include "globals.h"

#include "resources.h"
#include "particleSystem.h"

#include "lineintersect.h"

using namespace bzsf;

#include "intro.h"
#include "transition.h"
#include "level_0.h"
#include "level_1.h"
#include "level_2.h"

#include "score_attack.h"


//Function to make the game die after a clean loop by gameHandler, instead of quitting mid-loop
void game_die2() {
	bzsflegacy::game::run = false;
}

void restart_game() {
	bzsflegacy::game::currentState = SCORE_ATTACK;
}


int main(int argc, char * argv[]) {
	bzsflegacy::game::window = new sf::RenderWindow(sf::VideoMode(1280, 720), "Your Efforts are in Vain", sf::Style::Close);

	srand((unsigned)time(NULL));

	LoadResources();

	bzsflegacy::game::window->setFramerateLimit(120);

	bzsflegacy::game::addFunc(DIE, game_die2);
	bzsflegacy::game::addFunc(INTRO, game_intro);
	bzsflegacy::game::addFunc(LEVEL0, level_0);
	bzsflegacy::game::addFunc(LEVEL1, level_1);
	bzsflegacy::game::addFunc(LEVEL2, level_2);
	bzsflegacy::game::addFunc(RESTART, restart_game);
	bzsflegacy::game::addFunc(SCORE_ATTACK, score_attack);
	bzsflegacy::game::addFunc(TRANSITION, transition);



	bzsflegacy::game::currentState = INTRO;


	return bzsflegacy::game::runGame();
}
