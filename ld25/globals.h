const double PI = 3.1415926535;
double DEGTORAD(double deg) {return ((deg * PI) / 180.0);}

enum GameStates {
	RUNNING = 25,
	DIE = 999,
	MENU = 33,
	BOSS = 77
};
bool debug = false;

unsigned int level[2] = {0, 0};

class object;
class player;

object * findObject(unsigned int);
object * Finish;

sf::Clock generalClock;

namespace tbc {
	class cdTile;
	bool CheckCollision(player * Ent_obj, int addToX, int addToY , string sOccupier);
	bool CheckCollision(sf::Sprite& Ent_obj, int addToX, int addToY , string sOccupier);
	//bool CheckCollision(object * Ent_obj, int addToX, int addToY);
	vector<cdTile *> CollisionArray(player * Ent_obj, int addToX, int addToY, bool OnlyReturnTaken);
}
