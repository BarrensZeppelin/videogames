#ifndef LD31_COMMAND
#define LD31_COMMAND

#include <functional>
#include <SFML/System.hpp>
#include "Category.hpp"

class SceneNode;

class Command {
public:
	typedef std::function<void(SceneNode&, sf::Time)> CommandFunction;

	sf::Uint32 category;
	CommandFunction action;

	Command();
	Command(sf::Uint32 category, CommandFunction action);
};


template <typename GameObject>
Command::CommandFunction derivedAction(std::function<void(GameObject&, sf::Time)> fn) {
	return [=](SceneNode& node, sf::Time dt) {
		// Check if cast is safe
		assert(dynamic_cast<GameObject*>(&node) != nullptr);

		// Downcast node and invoke function on it
		fn(static_cast<GameObject&>(node), dt);
	};
}

#endif