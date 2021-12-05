#include "Application.hpp"
#include "World.hpp"
#include "Eagle.hpp"
#include "ParticleNode.hpp"
#include "SpriteNode.hpp"
#include "EmitterNode.hpp"
#include "SnowNode.hpp"
#include "Pickup.hpp"
#include "CliffNode.hpp"
#include "SoundNode.hpp"
#include "WaterSplashNode.hpp"
#include "Fish.hpp"
#include "BarNode.hpp"
#include "ScreenShakeNode.hpp"


World::World(Context context) 
: SEALEVEL(Application::settings.windowSize.y * 1 / 4)
, SEALINE(Application::settings.windowSize.y - SEALEVEL)
, context(context)
, timeToFish(sf::seconds(1.f)) {
	wind = 0;
	windStart = 0;
	windTarget = 0;
	cloudPos = 0;
	
	int windowX = Application::settings.windowSize.x;
	int windowY = Application::settings.windowSize.y;

	bool created = renderTexture.create(windowX, SEALINE);
	assert(created);

	created = sceneTexture.create(windowX, windowY);
	assert(created);

	//bg.setSize(sf::Vector2f(windowX, SEALINE));
	bg.setOrigin(windowX / 2, SEALINE / 2);
	bg.setPosition(windowX / 2, SEALINE / 2);
	
	seaLine.setSize(sf::Vector2f(windowX, 1));
	seaLine.setPosition(0, SEALINE - 1);
	seaLine.setFillColor(sf::Color::White);

	tintOverlay.setSize(sf::Vector2f(windowX, SEALINE));
	tintOverlay.setFillColor(sf::Color(0, 0, 0, 128));

	aboveSeaView.reset(sf::FloatRect(0, 0, windowX, SEALINE));
	aboveSeaView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 3.f / 4));

	seaReflectionView.setCenter(windowX / 2, SEALINE / 2);
	seaReflectionView.setSize(windowX, SEALEVEL);
	seaReflectionView.reset(sf::FloatRect(0, 0, windowX, SEALINE));
	seaReflectionView.setViewport(sf::FloatRect(0, 3.f / 4, 1.f, 1.f / 4));

	seaView.reset(sf::FloatRect(0, SEALINE, windowX, SEALEVEL));
	seaView.setViewport(seaReflectionView.getViewport());

	clouds.setTexture(context.textures->get(Texture::Clouds));
	clouds.setTextureRect(sf::IntRect(0, 0, 1280, 450));

	initializeCommands();
	buildScene();
}


void World::initializeCommands() {
	commands[ApplyWind].category = Category::AffectedByWind;
	commands[ApplyWind].action = derivedAction<Entity>([=](Entity& e, sf::Time dt) {
		if(e.isAffectedByWind())
			e.velocity += sf::Vector2f(wind * 2 * dt.asSeconds(), 0);
	});

	commands[ApplyParticleWind].category = Category::ParticleSystem;
	commands[ApplyParticleWind].action = derivedAction<ParticleNode>([=](ParticleNode& p, sf::Time dt) {
		p.applyWind(wind, dt);
	});

	commands[DestroyOutside].category = Category::Pickup;
	commands[DestroyOutside].action = derivedAction<Entity>([this](Entity& e, sf::Time) {
		if(!sf::FloatRect(0, -100, Application::settings.windowSize.x + 100, Application::settings.windowSize.y + 100).intersects(e.getBoundingRect()))
			e.destroy();
	});
}

void World::buildScene() {
	for(int i = 0; i < LayerCount; i++) {
		Category::Category category = (i == Lower ? Category::SceneAirLayer : Category::None);

		SceneNode::Ptr layer(new SceneNode(category));
		sceneLayers[i] = layer.get();
		sceneGraph.attachChild(std::move(layer));
	}

	// Make soundplayer
	sceneLayers[Lower]->attachChild(SceneNode::Ptr(new SoundNode(*context.soundPlayer)));
	
	//ScreenShakeNode
	sceneLayers[Background]->attachChild(SceneNode::Ptr(new ScreenShakeNode(&seaView, &aboveSeaView, &seaReflectionView)));

	// Make cliff
	CliffNode* cliff = new CliffNode(*context.textures, SEALINE);
	sceneLayers[Upper]->attachChild(SceneNode::Ptr(cliff));

	// Make player
	player = new Eagle(context, cliff, SEALINE, *sceneLayers[Upper]);
	player->setPosition(90, 120);
	sceneLayers[Upper]->attachChild(SceneNode::Ptr(player));

	// Make water effect
	sceneLayers[Upper]->attachChild(SceneNode::Ptr(new WaterSplashNode(*context.textures, SEALINE)));

	// Make particle nodes
	SceneNode::Ptr p(new ParticleNode(Particle::Snow, *context.textures));
	sceneLayers[Lower]->attachChild(std::move(p));
	
	p.reset(new ParticleNode(Particle::Blood, *context.textures));
	sceneLayers[Lower]->attachChild(std::move(p));

	p.reset(new ParticleNode(Particle::Water, *context.textures));
	sceneLayers[Upper]->attachChild(std::move(p));

	sceneLayers[Upper]->attachChild(SceneNode::Ptr(new SnowNode()));
}


void World::spawnPickups(sf::Time dt) {
	if(coinClock.getElapsedTime() >= sf::seconds(0.33f)) {
		coinClock.restart();

		float pos = 180 + rand() % (Application::settings.windowSize.x - 180);
		SceneNode::Ptr c(new Pickup(Pickup::Coin, context));
		c->setPosition(pos, -50);
		sceneLayers[Lower]->attachChild(std::move(c));
	}

	if(fishClock.getElapsedTime() >= timeToFish) {
		fishClock.restart();
		timeToFish = sf::seconds(0.5f + float(rand() % 1000) / 500.f);

		float pos = 475 + rand() % 60;
		float x = 140 + rand() % 600;
		SceneNode::Ptr f(new Fish(*context.animations, sf::Vector2f(x, pos)));
		f->setPosition(Application::settings.windowSize.x + 50, 475 + rand() % 60);
		sceneLayers[Upper]->attachChild(std::move(f));
	}
}

void World::calculateWind(sf::Time dt) {
	if(windClock.getElapsedTime() >= windTime) {
		windClock.restart();

		windStart = wind;
		windTarget = rand() % 750 - 375;
		windTime = sf::milliseconds(500 + rand() % 3500);
	}

	float windProgress = std::sin((windClock.getElapsedTime() / windTime) * bzsf::PI);
	wind = windStart + (windTarget - windStart) * windProgress;
}

bool matchesCategories(SceneNode::Pair& colliders, Category::Category type1, Category::Category type2) {
	sf::Uint32 category1 = colliders.first->getCategory();
	sf::Uint32 category2 = colliders.second->getCategory();

	// Make sure first pair entry has category type1 and second has type2
	if(type1 & category1 && type2 & category2)
		return true;
	else if(type1 & category2 && type2 & category1) {
		std::swap(colliders.first, colliders.second);
		return true;
	} else
		return false;
}

void World::handleCollisions() {
	std::set<SceneNode::Pair> colls;
	sceneGraph.checkSceneCollision(sceneGraph, colls);

	for(SceneNode::Pair p : colls) {
 		if(matchesCategories(p, Category::Player, Category::Pickup)) {
			Eagle& player = static_cast<Eagle&>(*p.first);
			Pickup& pickup = static_cast<Pickup&>(*p.second);

			pickup.apply(player);
			pickup.destroy();
			
			Command command(Category::SoundEffect, derivedAction<SoundNode>([](SoundNode& s, sf::Time) {
				s.playSound(SoundEffect::Pickup);
			}));
			commandQueue.push(command);
		}
	}
}



CommandQueue& World::getCommandQueue() {
	return commandQueue;
}

Eagle& World::getPlayer() {
	return *player;
}

void World::update(sf::Time dt) {
	sceneGraph.clean();
	spawnPickups(dt);

	calculateWind(dt);
	cloudPos += (wind / 2) * dt.asSeconds();
	clouds.setTextureRect(sf::IntRect(-cloudPos, 0, 1280, 450));

	commandQueue.push(commands[DestroyOutside]);
	commandQueue.push(commands[ApplyWind]);
	commandQueue.push(commands[ApplyParticleWind]);

	while(!commandQueue.isEmpty())
		sceneGraph.onCommand(commandQueue.pop(), dt);

	sf::Clock pClock;
	handleCollisions();

	sceneGraph.update(dt, commandQueue);


	context.soundPlayer->removeStoppedSounds();
}


void World::draw(sf::RenderTarget& target, sf::RenderStates states) {
	const sf::View& prevView = target.getView();

	renderTexture.clear(bgColor);
	
	renderTexture.draw(seaLine);
	renderTexture.draw(clouds);

	sceneGraph.draw(renderTexture, states);
	
	renderTexture.display();

	bg.setColor(sf::Color(255, 255, 255));
	bg.setScale(1, 1);
	bg.setTexture(renderTexture.getTexture());
	
	if(PostEffect::isSupported()) {
		sceneTexture.clear();
		
		sceneTexture.setView(aboveSeaView);
		sceneTexture.draw(bg);

		bg.setColor(sf::Color(255, 255, 255));
		bg.setScale(1, -1);
		sceneTexture.setView(seaReflectionView);
		sceneTexture.draw(bg, sf::BlendNone);
		sceneTexture.draw(tintOverlay);
		
		sceneTexture.setView(seaView);
		sceneGraph.drawSea(sceneTexture, states);

		sceneTexture.display();
		
		
		bloomEffect.apply(sceneTexture, target);
	} else {
		target.setView(aboveSeaView);
		target.draw(bg);

		bg.setColor(sf::Color(255, 255, 255));
		bg.setScale(1, -1);
		target.setView(seaReflectionView);
		target.draw(bg, sf::BlendNone);
		target.draw(tintOverlay);

		target.setView(seaView);
		sceneGraph.drawSea(target, states);
	}

	target.setView(prevView);
}