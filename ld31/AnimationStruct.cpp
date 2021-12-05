#include "AnimationStruct.hpp"


AnimationStruct::AnimationStruct() :
animColsRows(0, 0),
speed(0),
repeat(1),
texture() {}

AnimationStruct::AnimationStruct(sf::Vector2u AnimColRows, sf::Uint32 Speed, bool Repeat, std::string texturePath) :
animColsRows(AnimColRows),
speed(Speed),
repeat(Repeat),
texture() {
	texture.loadFromFile(texturePath);
}



bzsf::Animation* ParseAnimation(const AnimationStruct& aStruct, bzsf::Animation* animation) {
	if(animation != nullptr) delete animation;

	animation = new bzsf::Animation(
		aStruct.animColsRows.x,
		aStruct.animColsRows.y,
		aStruct.texture,
		sf::milliseconds(aStruct.speed),
		aStruct.repeat);

	return animation;
}
