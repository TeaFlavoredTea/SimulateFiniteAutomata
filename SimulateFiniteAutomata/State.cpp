#include "State.h"

State::State() { this->stateID = -1; }

State::State(int stateNumber)
{
	this->stateID = stateNumber;
}

void State::addTransition(char c, State *endState)
{
	// Are there transitions for this character?
	auto transIter = this->transitions.find(c);
	if (transIter == this->transitions.end()) {	// No, create the transitions vector then add the transition.
		std::vector<State*> destinations;
		destinations.push_back(endState);
		this->transitions.emplace(c, destinations);
	}
	else {	// Yes, add the transition.
		transIter->second.push_back(endState);
	}
}

const std::vector<State*>& State::getTransitons(char c)
{
	// Are there transitions for this character?
	auto transIter = this->transitions.find(c);
	if (transIter == this->transitions.end()) {	// No, return an empty vector
		return std::vector<State*>();
	}
	else {	// Yes, return a reference to these transitions
		return transIter->second;
	}
}

