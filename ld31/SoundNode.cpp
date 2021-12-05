#include "SoundNode.hpp"
#include "SoundPlayer.hpp"

SoundNode::SoundNode(SoundPlayer& player)
	: sounds(player) {}

void SoundNode::playSound(SoundEffect::ID sound) {
	sounds.play(sound);//, position);
}

sf::Uint32 SoundNode::getCategory() {
	return Category::SoundEffect;
}
