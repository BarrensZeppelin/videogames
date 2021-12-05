/*double DEGTORAD(double deg) {return ((deg * bzsf::PI) / 180.0);}
double RADTODEG(double rad) {return ((180.0 * rad) / bzsf::PI);}*/

enum GameStates {
	INTRO = 13,
	DIE = 999,
	MENU = 33,
	LEVEL0 = 1,
	LEVEL1 = 2,
	LEVEL2 = 3,
	SCORE_ATTACK = 4,
	RESTART = 10,
	ENDING = 111,
	TRANSITION = 500
};

enum Mode {
	SCORE = 1,
	STORY = 2
};

Mode mode = STORY;

std::string name = "";
/*
int round(float n) {
	return (int) floor(n + 0.5f);
}
*/
sf::Uint8 currentTransition = 0;

std::string transitions [] = { //STORY101
	"You find yourself on a cliff.\n\nThe weather is quite...\nbad.\n\nWhy did you even come up here?\n\nIt was a really bad idea...",
	"Oh no.\n\nEverything was so...\nbright\notherwordly.\n\nAnd suddenly\nI was on my way to who-knows-where...",
	"On the way I encounted unforeseen complications.\nHeadaches...\n\nI can do better than this, I thought to myself.\n\nDo not give up.\nI would never give up.",
	"Time to give it another whirl.\n\nMore focus.\nLet determination lead the way.",
	"Despite my efforts\nI quickly came to the realization:\n\nMy combined luck and struggle is futile.\n\nJumping of a cliff\n(even accidentally)\nis pointless.\n\nYou will never make it alive.\n\nYou will never win.",
	"Your efforts are in vain.\n\nThanks for playing!\n\nMade in 48 hours for the Ludum Dare 26 competition April 2013\nby Oskar Veileborg:\n\nTwitter: @BarrensZeppelin\nGitHub: BarrensZeppelin\n\n(:\n\nPeace out!"
};
