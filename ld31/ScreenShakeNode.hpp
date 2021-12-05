#include "SceneNode.hpp"

class ScreenShakeNode : public SceneNode {
private:
	sf::View* seaView;
	sf::View* aboveSeaView;
	sf::View* seaReflectionView;

	sf::Vector2f seaViewCenter;
	sf::Vector2f aboveSeaViewCenter;
	sf::Vector2f seaReflectionViewCenter;
private:
	void updateCurrent(sf::Time dt, CommandQueue& commandQueue);

public:
	float shake;

	ScreenShakeNode(sf::View* seaView, sf::View* aboveSeaView, sf::View* seaReflectionView);

	sf::Uint32 getCategory();
};