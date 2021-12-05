#ifndef LD31_RESID
#define LD31_RESID

#include <ResStore/ResourceStorage.hpp>
#include "AnimationStruct.hpp"

namespace sf {
	class SoundBuffer;
}

namespace Texture {
	enum ID {
		Clouds,
		Snow,
		ReturnArrow,
		Coin,
		TopCliff,
		BottomCliff,
		BloodParticle,
		WaterParticle,
		WaterSplash,
		TitleSplash,
		BZepIco
	};
}

namespace Animation {
	enum ID {
		Eagle,
		Fish
	};
}

namespace Font {
	enum ID {
		Arial,
		Emulator
	};
}

namespace SoundEffect {
	enum ID {
		Die,
		Flap,
		Pickup,
		Powerup,
		WaterSplashBig
	};
}

namespace Shader {
	enum ID {
		BrightnessPass,
		DownSamplePass,
		GaussianBlurPass,
		AddPass,
	};
}

typedef ResourceStorage<sf::Texture, Texture::ID> TextureStorage;
typedef ResourceStorage<sf::Font, Font::ID> FontStorage;
typedef ResourceStorage<sf::SoundBuffer, SoundEffect::ID> SoundBufferStorage;
typedef ResourceStorage<sf::Shader, Shader::ID> ShaderStorage;
typedef std::map<Animation::ID, AnimationStruct> AnimationStorage;

#endif