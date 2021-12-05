#ifndef LD31_ANIMSTRUCT
#define LD31_ANIMSTRUCT

#include <bzsf/SFML_Snips.hpp>



struct AnimationStruct {
	sf::Vector2u animColsRows;
	sf::Uint32 speed;
	bool repeat;
	sf::Texture texture;

	AnimationStruct();
	AnimationStruct(sf::Vector2u AnimColRows, sf::Uint32 Speed, bool Repeat, std::string texturePath);
};

bzsf::Animation* ParseAnimation(const AnimationStruct&, bzsf::Animation* animation = nullptr);

#endif