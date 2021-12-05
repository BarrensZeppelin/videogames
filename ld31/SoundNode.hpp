#ifndef LD31_SOUNDNODE
#define LD31_SOUNDNODE

#include "SceneNode.hpp"
#include "resid.hpp"

class SoundPlayer;

class SoundNode : public SceneNode {
private:
	SoundPlayer&			sounds;

public:
	explicit				SoundNode(SoundPlayer& player);
	void					playSound(SoundEffect::ID sound);

	virtual sf::Uint32	getCategory();
};

#endif
