sf::Texture playerTexture;
sf::Texture backgroundTexture;
sf::Texture teleportTargetTexture;
sf::Texture tilesetTexture;
sf::Texture bloodTexture;
sf::Texture helperTexture;
sf::Texture deathTexture;
sf::Texture finishTexture;
sf::Texture exploTexture;
sf::Texture titleTexture;
sf::Texture cpuTexture;

sf::Font helperFont;

bzsf::Tileset* Walls;

void LoadTextures() {
	playerTexture.loadFromFile("imgs/player.png");
	backgroundTexture.loadFromFile("imgs/background6.png");
	teleportTargetTexture.loadFromFile("imgs/teleportTarget.png");
	deathTexture.loadFromFile("imgs/death2.png");
	finishTexture.loadFromFile("imgs/finish.png");
	bloodTexture.loadFromFile("imgs/blood.png");
	titleTexture.loadFromFile("imgs/title.png");
	cpuTexture.loadFromFile("imgs/cpu.png");
	exploTexture.loadFromFile("imgs/explo.png");

	tilesetTexture.loadFromFile("imgs/tileset.png");
	Walls = new bzsf::Tileset(tilesetTexture, sf::Vector2u(32, 32));

	helperFont.loadFromFile("imgs/helperFont.ttf");
	helperTexture.loadFromFile("imgs/helper.png");
}
