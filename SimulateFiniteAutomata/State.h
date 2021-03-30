#pragma once
#include <vector>
#include <map>
#include <unordered_map>


class State {
public:
	State();
	State(int);

	void addTransition(char, State*);
	const std::vector<State*>& getTransitons(char c);

private:
	int stateID;
	std::unordered_map<char, std::vector<State*>> transitions;
};