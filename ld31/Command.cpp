#include "Command.hpp"

Command::Command() : action(), category(Category::None) {}

Command::Command(sf::Uint32 category, CommandFunction action) : action(action), category(category) {}