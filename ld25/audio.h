sf::SoundBuffer exploBuf;
sf::Sound exploSnd;

sf::SoundBuffer teleportBuf;
sf::Sound teleportSnd;
sf::SoundBuffer teleportBuf2;
sf::Sound teleportSnd2;

sf::SoundBuffer selectBuf;
sf::Sound selectSnd;

sf::SoundBuffer electroBuf;
sf::Sound electroSnd;

sf::SoundBuffer menuBuf;
sf::Sound menuSnd;


void LoadAudio() {
	exploBuf.loadFromFile("audio/explo.wav");
	exploSnd.setBuffer(exploBuf);

	teleportBuf.loadFromFile("audio/teleport.wav");
	teleportSnd.setBuffer(teleportBuf);
	teleportSnd.setVolume(30);
	teleportBuf2.loadFromFile("audio/teleport2.wav");
	teleportSnd2.setBuffer(teleportBuf2);
	teleportSnd2.setVolume(30);

	selectBuf.loadFromFile("audio/select.wav");
	selectSnd.setBuffer(selectBuf);
	selectSnd.setVolume(20);

	electroBuf.loadFromFile("audio/electro.wav");
	electroSnd.setBuffer(electroBuf);
	electroSnd.setVolume(50);

	menuBuf.loadFromFile("audio/menuSound.wav");
	menuSnd.setBuffer(menuBuf);
}
