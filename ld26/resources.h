////// TEXTURES
sf::Texture tex_lightning;
sf::Texture tex_ground0;
sf::Texture tex_ground1;

sf::Texture tex_rain0;
sf::Texture tex_rain1;

sf::Texture tex_bush;

sf::Texture tex_cloud0;
sf::Texture tex_cloud1;
sf::Texture tex_cloud2;
sf::Texture tex_cloud3;

sf::Texture tex_healthbar;
sf::Texture tex_healthfill;

sf::Texture tex_arrow;

sf::Texture tex_title;

sf::Texture tex_particles;
bzsf::Tileset::Ptr particles;

////// SOUND
sf::SoundBuffer sndBuf_Lightning;
sf::Sound snd_Lightning;

sf::SoundBuffer sndBuf_healthtick;
sf::Sound snd_healthtick;

sf::SoundBuffer sndBuf_hop;
sf::Sound snd_hop;

sf::SoundBuffer sndBuf_wow;
sf::Sound snd_wow;

sf::SoundBuffer sndBuf_buildup;
sf::Sound snd_buildup;

sf::SoundBuffer sndBuf_warning;
sf::Sound snd_warning;

sf::SoundBuffer sndBuf_blip;
sf::Sound snd_blip;

sf::SoundBuffer sndBuf_hit;
sf::Sound snd_hit;

sf::SoundBuffer sndBuf_swoosh0;
sf::Sound snd_swoosh0;
sf::SoundBuffer sndBuf_swoosh1;
sf::Sound snd_swoosh1;
sf::SoundBuffer sndBuf_swoosh2;
sf::Sound snd_swoosh2;

sf::Music music_rain;
sf::Music music_theme;

sf::Font font_default;
sf::Font font_transition;

void LoadResources() {
	font_default; font_default.loadFromFile("imgs/transition.ttf");
	const sf::Texture* tex = &font_default.getTexture(10);
    ((sf::Texture*) tex)->setSmooth(false);

	font_transition.loadFromFile("imgs/transition.ttf");


	//// TEXTURES
	tex_lightning.loadFromFile("imgs/lightning.png");

	tex_ground0.loadFromFile("imgs/ground.png");
	tex_ground1.loadFromFile("imgs/ground1.png");

	tex_rain0.loadFromFile("imgs/rain0.png");
	tex_rain1.loadFromFile("imgs/rain1.png");

	tex_bush.loadFromFile("imgs/bush.png");

	tex_cloud0.loadFromFile("imgs/cloud0.png");
	tex_cloud1.loadFromFile("imgs/cloud1.png");
	tex_cloud2.loadFromFile("imgs/cloud2.png");
	tex_cloud3.loadFromFile("imgs/cloud3.png");

	tex_particles.loadFromFile("imgs/particles.png");
	particles = bzsf::Tileset::Ptr(new bzsf::Tileset(tex_particles, sf::Vector2u(4, 4)));

	tex_healthbar.loadFromFile("imgs/healthbar.png");
	tex_healthfill.loadFromFile("imgs/healthfill.png");

	tex_arrow.loadFromFile("imgs/arrow.png");

	tex_title.loadFromFile("imgs/title.png");

	/// SOUND
	sndBuf_Lightning.loadFromFile("audio/lightning.wav");
	snd_Lightning.setBuffer(sndBuf_Lightning);

	sndBuf_hop.loadFromFile("audio/hop.wav");
	snd_hop.setBuffer(sndBuf_hop);

	sndBuf_wow.loadFromFile("audio/wow.wav");
	snd_wow.setBuffer(sndBuf_wow);

	sndBuf_healthtick.loadFromFile("audio/healthtick.wav");
	snd_healthtick.setBuffer(sndBuf_healthtick);

	sndBuf_buildup.loadFromFile("audio/buildup.wav");
	snd_buildup.setBuffer(sndBuf_buildup);

	sndBuf_warning.loadFromFile("audio/warning.wav");
	snd_warning.setBuffer(sndBuf_warning);

	sndBuf_swoosh0.loadFromFile("audio/swoosh0.wav");
	snd_swoosh0.setBuffer(sndBuf_swoosh0);
	sndBuf_swoosh1.loadFromFile("audio/swoosh1.wav");
	snd_swoosh1.setBuffer(sndBuf_swoosh1);
	sndBuf_swoosh2.loadFromFile("audio/swoosh2.wav");
	snd_swoosh2.setBuffer(sndBuf_swoosh2);

	sndBuf_blip.loadFromFile("audio/blip.wav");
	snd_blip.setBuffer(sndBuf_blip);

	sndBuf_hit.loadFromFile("audio/hit.wav");
	snd_hit.setBuffer(sndBuf_hit);


	music_rain.openFromFile("audio/rain.wav");

	music_theme.openFromFile("audio/theme.wav");
}
