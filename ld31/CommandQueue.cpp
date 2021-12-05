#include "CommandQueue.hpp"

void CommandQueue::push(const Command& cm) {
	queue.push(cm);
}


Command CommandQueue::pop() {
	Command cm = queue.front();
	queue.pop();
	return cm;
}

Command CommandQueue::peek() {
	Command cm = queue.front();
	return cm;
}

bool CommandQueue::isEmpty() {
	return queue.size() == 0;
}