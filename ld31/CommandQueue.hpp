#ifndef LD31_COMMANDQUEUE
#define LD31_COMMANDQUEUE

#include <queue>
#include "Command.hpp"


class CommandQueue {
private:
	std::queue<Command> queue;

public:
	void push(const Command& command);
	Command pop();
	Command peek();
	bool isEmpty();
};

#endif