#ifndef LD31_CATEGORY
#define LD31_CATEGORY

namespace Category {
	enum Category {
		None = 0,
		SceneAirLayer = 1 << 0,
		Player = 1 << 1,
		ParticleSystem = 1 << 2,
		Pickup = 1 << 3,
		Cliff = 1 << 4,
		SoundEffect = 1 << 5,
		SplashEffect = 1 << 6,
		ScreenShake = 1 << 7,

		AffectedByWind = Player | Pickup
	};
}

#endif