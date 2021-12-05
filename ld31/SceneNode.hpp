#ifndef LD31_SCENENODE
#define LD31_SCENENODE

#include <bzsf/SFML_Snips.hpp>
#include "Category.hpp"
#include "CommandQueue.hpp"

#include "resid.hpp"

class SceneNode : public sf::Transformable {
public:
	typedef std::unique_ptr<SceneNode> Ptr;
	typedef std::pair<SceneNode*, SceneNode*> Pair;

protected:
	std::vector<Ptr> children;
	SceneNode* parent;
	sf::Uint32 category;

protected:
	virtual void updateCurrent(sf::Time dt, CommandQueue& commandQueue);
	void updateChildren(sf::Time dt, CommandQueue& commandQueue);

	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default);
	void drawChildren(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default);
	void drawBoundingRect(sf::RenderTarget& target, sf::RenderStates states);

	void playSound(SoundEffect::ID effect, CommandQueue& commandQueue);
	void splashWater(float position, CommandQueue& commandQueue);

public:
	explicit SceneNode(Category::Category category = Category::None);

	virtual sf::Uint32 getCategory();
	sf::Vector2f getWorldPosition();
	sf::Transform getWorldTransform();
	virtual bool isMarkedForRemoval();
	virtual sf::FloatRect getBoundingRect();

	void clean();

	void attachChild(Ptr child);
	Ptr detachChild(const SceneNode& node);

	void onCommand(const Command& cm, sf::Time dt);
	void update(sf::Time dt, CommandQueue& commandQueue);
	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default);
	virtual void drawSea(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default);

	void checkSceneCollision(SceneNode& sceneGraph, std::set<Pair>& collisionPairs);
	void checkNodeCollision(SceneNode& node, std::set<Pair>& collisionPairs);
};

#endif
