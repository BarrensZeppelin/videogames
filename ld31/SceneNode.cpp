#include "SceneNode.hpp"
#include "SoundNode.hpp"
#include "WaterSplashNode.hpp"

#include <cassert>

using namespace std::placeholders;

SceneNode::SceneNode(Category::Category category)
: category(category)
, parent(nullptr)
, children() {}


void SceneNode::updateCurrent(sf::Time dt, CommandQueue& commandQueue) {}

void SceneNode::updateChildren(sf::Time dt, CommandQueue& commandQueue) {
	for(SceneNode::Ptr& s : children)
		s->update(dt, commandQueue);
}

void SceneNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) {}

void SceneNode::drawChildren(sf::RenderTarget& target, sf::RenderStates states) {
	for(SceneNode::Ptr& s : children)
		s->draw(target, states);
}

void SceneNode::drawBoundingRect(sf::RenderTarget& target, sf::RenderStates states) {
	sf::FloatRect rect = getBoundingRect();

	sf::RectangleShape shape;
	shape.setPosition(sf::Vector2f(rect.left, rect.top));
	shape.setSize(sf::Vector2f(rect.width, rect.height));
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineColor(sf::Color::Green);
	shape.setOutlineThickness(1.f);

	target.draw(shape);
}


void SceneNode::playSound(SoundEffect::ID effect, CommandQueue& commandQueue) {
	Command command(Category::SoundEffect,
		derivedAction<SoundNode>(std::bind(&SoundNode::playSound, _1, effect)));

	commandQueue.push(command);
}


void SceneNode::splashWater(float position, CommandQueue& commandQueue) {
	Command command(Category::SplashEffect, derivedAction<WaterSplashNode>(std::bind(&WaterSplashNode::splash, _1, position)));

	commandQueue.push(command);
}

///

sf::Uint32 SceneNode::getCategory() {
	return category;
}

sf::Vector2f SceneNode::getWorldPosition() {
	return getWorldTransform() * sf::Vector2f();
}

sf::Transform SceneNode::getWorldTransform() {
	sf::Transform transform = sf::Transform::Identity;

	for(const SceneNode* node = this; node != nullptr; node = node->parent)
		transform *= node->getTransform();

	return transform;
}

bool SceneNode::isMarkedForRemoval() {
	return false;
}

sf::FloatRect SceneNode::getBoundingRect() {
	return sf::FloatRect();
}


void SceneNode::clean() {
	children.erase(std::remove_if(children.begin(), children.end(), std::mem_fn(&SceneNode::isMarkedForRemoval)), children.end());

	for(Ptr& s : children)
		s->clean();
}


void SceneNode::attachChild(Ptr child) {
	child->parent = this;
	children.push_back(std::move(child));
}


SceneNode::Ptr SceneNode::detachChild(const SceneNode& node) {
	auto found = std::find_if(children.begin(), children.end(), [&](Ptr& p) { return p.get() == &node; });
	
	if(found != children.end()) {
		Ptr result = std::move(*found);
		result->parent = nullptr;
		children.erase(found);
		return result;
	} else
		return SceneNode::Ptr();
}


void SceneNode::onCommand(const Command& cm, sf::Time dt) {
	if(cm.category & getCategory())
		cm.action(*this, dt);

	for(SceneNode::Ptr& s : children)
		s->onCommand(cm, dt);
}

void SceneNode::update(sf::Time dt, CommandQueue& commandQueue) {
	updateCurrent(dt, commandQueue);
	updateChildren(dt, commandQueue);
}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) {
	states.transform *= getTransform();

	drawCurrent(target, states);
	drawChildren(target, states);

	//drawBoundingRect(target, states);
}

void SceneNode::drawSea(sf::RenderTarget& target, sf::RenderStates states) {
	states.transform *= getTransform();

	for(Ptr& s : children)
		s->drawSea(target, states);
}


bool collision(SceneNode& lhs, SceneNode& rhs) {
	return lhs.getBoundingRect().intersects(rhs.getBoundingRect());
}

void SceneNode::checkSceneCollision(SceneNode& sceneGraph, std::set<Pair>& collisionPairs) {
	checkNodeCollision(sceneGraph, collisionPairs);

	for(Ptr& s : sceneGraph.children)
		checkSceneCollision(*s, collisionPairs);
}

void SceneNode::checkNodeCollision(SceneNode& node, std::set<Pair>& collisionPairs) {
	if(this != &node && collision(*this, node))
		collisionPairs.insert(std::minmax(this, &node));

	for(Ptr& s : children)
		s->checkNodeCollision(node, collisionPairs);
}