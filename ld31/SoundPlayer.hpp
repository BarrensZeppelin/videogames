#ifndef LD31_SOUNDPLAYER
#define LD31_SOUNDPLAYER

#include <SFML/Audio.hpp>
#include "resid.hpp"
#include <list>


class SoundPlayer {
public:
	SoundPlayer();

	void						play(SoundEffect::ID effect);
	void						play(SoundEffect::ID effect, sf::Vector2f position);

	void						removeStoppedSounds();
	void						setListenerPosition(sf::Vector2f position);
	sf::Vector2f				getListenerPosition() const;


private:
	SoundBufferStorage			soundBuffers;
	std::list<sf::Sound>		sounds;
};

#endif
