#pragma once
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <unordered_set>
#include <memory>
#include "State.h"

class FiniteAutomata {
public:
	FiniteAutomata();
	FiniteAutomata(const std::string&);

	bool testString(const std::string&);

	static const std::vector< std::vector< std::string>> getLinesFromFile(const std::string&);

private:
	std::unordered_set<State*> getEpsilonTransitions(const std::unordered_set<State*>&);

	std::unordered_map<int, std::unique_ptr<State>> states;
	std::unordered_set<State*> acceptStates;
};


