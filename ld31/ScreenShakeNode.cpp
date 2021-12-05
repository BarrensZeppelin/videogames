#include "ScreenShakeNode.hpp"


ScreenShakeNode::ScreenShakeNode(sf::View* seaView, sf::View* aboveSeaView, sf::View* seaReflectionView)
	: seaView(seaView)
	, aboveSeaView(aboveSeaView)
	, seaReflectionView(seaReflectionView) {

	seaViewCenter = seaView->getCenter();
	aboveSeaViewCenter = aboveSeaView->getCenter();
	seaReflectionViewCenter = seaReflectionView->getCenter();
}


void ScreenShakeNode::updateCurrent(sf::Time dt, CommandQueue& commandQueue) {
	shake = std::max(shake - 10.f * dt.asSeconds(), 0.f);

	sf::Vector2f randV((float(rand() % 1000) / 1000.f) * shake * 2, (float(rand() % 1000) / 1000.f) * shake * 2);
	sf::Vector2f offV(-shake, -shake);
	seaView->setCenter(seaViewCenter - offV + randV);
	aboveSeaView->setCenter(aboveSeaViewCenter - offV + randV);
	seaReflectionView->setCenter(seaReflectionViewCenter - offV + randV);
}


sf::Uint32 ScreenShakeNode::getCategory() {
	return Category::ScreenShake;
}