/*						  ______._
                         / /    \ \
                        / /     / /
                        \ \
                         \ \____.
                        /    /  \\
                       / @   \  / \,
                      /       \/    \___          _______
                      \____/         \  `--------'       `-,
                        ;; |                                `__
                          |                                 ~, \
                          |                                 | ; \
                          |                                 | ),/
                           \                               /
                          / \.                  |         |
                         /  / \     /            \       /
                        /  /   |   / `~~~~~~~~~~~'\     /
                       /  /    |  |               |\   |
                       \  \    |  |               | |  |
                        \  \   |  |               | |  |
                         \  \  |  |               | |  (
                          \-L  |  |               | |  |
                           \Z  |  |               | |  |
                               |  |               | |  |
                               |__|               |_|__|     valkyrie
                               |/\/               |/|/\/
*/
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <math.h>
#include <fstream>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <bzsf/SFML_Snips.hpp>

using namespace std;
#include "globals.h"
#include "textures.h"
#include "audio.h"
//#include "particle.h"


#include <TBC-1.0/collision_detection.hpp>
#include "objects.h"

#include "game_loop.h"
#include "menu.h"
#include "boss.h"



int main(int argc, char * argv[]) {
	bzsflegacy::game::window = new sf::RenderWindow(sf::VideoMode(1280, 720, 32), "LD25 Bzep Game");
	bzsflegacy::game::window->setFramerateLimit(120);

	LoadTextures();
	LoadAudio();

	bzsflegacy::game::addFunc(RUNNING, game_loop);
	bzsflegacy::game::addFunc(DIE, game_die);
	bzsflegacy::game::addFunc(MENU, do_menu);
	bzsflegacy::game::addFunc(BOSS, boss_level);

	bzsflegacy::game::currentState = MENU;

	return bzsflegacy::game::runGame();
}
